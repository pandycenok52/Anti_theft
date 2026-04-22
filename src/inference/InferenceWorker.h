#ifndef INFERENCEWORKER_H
#define INFERENCEWORKER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QVariantMap>
#include "sensors/SensorDataTypes.h"
#include "utils/BaseWorker.h"

// Forward declaration для TFLite
namespace tflite {
    class FlatBufferModel;
    class Interpreter;
}

class InferenceWorker : public BaseWorker
{
    Q_OBJECT
public:
    explicit InferenceWorker(QObject *parent = nullptr);
    ~InferenceWorker();

public slots:
    void processNewData(const SensorDataBatch &batch);
    void runInferenceLoop();
    void stopInference();

signals:
    void inferenceResultReady(const QVariantMap &result);

private:
    bool loadModel(const QString &modelPath);
    QVariantMap runInference(const SensorDataBatch &batch);

    QQueue<SensorDataBatch> m_dataQueue;
    QMutex m_queueMutex;
    QWaitCondition m_queueNotEmpty;
    bool m_running;

    // TFLite объекты
    std::unique_ptr<tflite::FlatBufferModel> m_model;
    std::unique_ptr<tflite::Interpreter> m_interpreter;
};

#endif // INFERENCEWORKER_H