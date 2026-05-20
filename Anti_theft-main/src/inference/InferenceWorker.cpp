#include "InferenceWorker.h"
#include "utils/ThreadLogger.h"
#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QDebug>

// Подключаем заголовки TensorFlow Lite
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/model.h"

InferenceWorker::InferenceWorker(QObject *parent)
    : BaseWorker(parent)
    , m_running(false)
{
    // Путь к модели
    QString modelPath = QCoreApplication::applicationDirPath() + "/model.tflite";
    if (!loadModel(modelPath)) {
        emit errorOccurred("Failed to load TFLite model");
    }
}

InferenceWorker::~InferenceWorker()
{
    stopInference();
}

bool InferenceWorker::loadModel(const QString &modelPath)
{
    m_model = tflite::FlatBufferModel::BuildFromFile(modelPath.toStdString().c_str());
    if (!m_model) {
        qCritical() << "Failed to load model from" << modelPath;
        return false;
    }

    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*m_model, resolver);
    builder(&m_interpreter);
    if (!m_interpreter) {
        qCritical() << "Failed to construct interpreter";
        return false;
    }

    if (m_interpreter->AllocateTensors() != kTfLiteOk) {
        qCritical() << "Failed to allocate tensors";
        return false;
    }

    qDebug() << "TFLite model loaded successfully. Inputs:" << m_interpreter->inputs().size()
             << "Outputs:" << m_interpreter->outputs().size();
    return true;
}

void InferenceWorker::processNewData(const SensorDataBatch &batch)
{
    QMutexLocker locker(&m_queueMutex);
    m_dataQueue.enqueue(batch);
    m_queueNotEmpty.wakeOne();
}

void InferenceWorker::runInferenceLoop()
{
    LOG_THREAD_ID("InferenceWorker::runInferenceLoop");
    m_running = true;

    while (m_running) {
        SensorDataBatch batch;
        {
            QMutexLocker locker(&m_queueMutex);
            if (m_dataQueue.isEmpty()) {
                // Ожидаем до 500 мс
                m_queueNotEmpty.wait(&m_queueMutex, 500);
                if (m_dataQueue.isEmpty() && !m_running) break;
                if (m_dataQueue.isEmpty()) continue;
            }
            batch = m_dataQueue.dequeue();
        }

        QVariantMap result = runInference(batch);
        if (!result.isEmpty()) {
            emit inferenceResultReady(result);
        }
    }

    emit finished();
}

void InferenceWorker::stopInference()
{
    LOG_THREAD_ID("InferenceWorker::stopInference");
    m_running = false;
    m_queueNotEmpty.wakeAll();
}

QVariantMap InferenceWorker::runInference(const SensorDataBatch &batch)
{
    if (!m_interpreter) {
        return QVariantMap();
    }

    QElapsedTimer timer;
    timer.start();

    int input_tensor_idx = m_interpreter->inputs()[0];
    TfLiteTensor* input_tensor = m_interpreter->tensor(input_tensor_idx);
    float* input_data = input_tensor->data.f;

    // Копируем данные акселерометра
    if (batch.accelerometer.size() >= 3) {
        input_data[0] = batch.accelerometer[0];
        input_data[1] = batch.accelerometer[1];
        input_data[2] = batch.accelerometer[2];
    } else {
        // если данных нет, заполняем нулями
        input_data[0] = 0.0f;
        input_data[1] = 0.0f;
        input_data[2] = 0.0f;
    }

    // Запуск инференса
    if (m_interpreter->Invoke() != kTfLiteOk) {
        qWarning() << "Inference failed";
        return QVariantMap();
    }

    // Получаем выходной тензор
    int output_tensor_idx = m_interpreter->outputs()[0];
    TfLiteTensor* output_tensor = m_interpreter->tensor(output_tensor_idx);
    float* output_data = output_tensor->data.f;
    // Количество классов
    int num_classes = output_tensor->dims->data[1];
    QVector<float> probabilities;
    int predicted_class = 0;
    float max_prob = 0.0f;
    for (int i = 0; i < num_classes; ++i) {
        float prob = output_data[i];
        probabilities.append(prob);
        if (prob > max_prob) {
            max_prob = prob;
            predicted_class = i;
        }
    }

    qint64 inference_time = timer.elapsed();

    QVariantMap result;
    result["timestamp"] = batch.timestamp;
    result["predicted_class"] = predicted_class;
    result["confidence"] = max_prob;
    result["probabilities"] = QVariant::fromValue(probabilities);
    result["inference_time_ms"] = inference_time;

    // Классификация по имени
    static const char* class_names[] = {"Rest", "Walking", "Running"};
    if (predicted_class < 3) {
        result["class_name"] = class_names[predicted_class];
    } else {
        result["class_name"] = "Unknown";
    }

    return result;
}