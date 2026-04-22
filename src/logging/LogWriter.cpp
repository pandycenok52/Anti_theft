#include "LogWriter.h"
#include "utils/ThreadLogger.h"
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

LogWriter::LogWriter(QObject *parent)
    : BaseWorker(parent)
    , m_running(false)
{
}

LogWriter::~LogWriter()
{
    stop();
}

void LogWriter::start()
{
    LOG_THREAD_ID("LogWriter::start");

    QString logDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (logDir.isEmpty()) {
        emit errorOccurred("Cannot determine writable location");
        return;
    }

    QString fileName = QString("sensor_inference_%1.log")
                       .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    QString filePath = QDir(logDir).absoluteFilePath(fileName);

    m_logFile.setFileName(filePath);
    if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        emit errorOccurred("Failed to open log file: " + filePath);
        return;
    }

    m_stream.setDevice(&m_logFile);
    m_stream << "=== Log started at " << QDateTime::currentDateTime().toString(Qt::ISODate) << " ===\n";
    m_stream.flush();

    m_running = true;
    // Запускаем обработку очереди
    QMetaObject::invokeMethod(this, "processQueue", Qt::QueuedConnection);
}

void LogWriter::stop()
{
    LOG_THREAD_ID("LogWriter::stop");
    m_running = false;
    m_queueNotEmpty.wakeAll();
    if (m_logFile.isOpen()) {
        m_stream << "=== Log stopped at " << QDateTime::currentDateTime().toString(Qt::ISODate) << " ===\n";
        m_stream.flush();
        m_logFile.close();
    }
    emit finished();
}

void LogWriter::writeRawData(const SensorDataBatch &batch)
{
    QMutexLocker locker(&m_queueMutex);
    QJsonObject obj;
    obj["type"] = "raw";
    obj["timestamp"] = batch.timestamp;
    QJsonArray acc;
    for (float v : batch.accelerometer) acc.append(v);
    obj["accelerometer"] = acc;
    obj["light"] = batch.light;

    QJsonDocument doc(obj);
    m_writeQueue.enqueue(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    m_queueNotEmpty.wakeOne();
}

void LogWriter::writeInferenceResult(const QVariantMap &result)
{
    QMutexLocker locker(&m_queueMutex);
    QJsonObject obj;
    obj["type"] = "inference";
    obj["timestamp"] = result["timestamp"].toLongLong();
    obj["predicted_class"] = result["predicted_class"].toInt();
    obj["confidence"] = result["confidence"].toDouble();
    obj["inference_time_ms"] = result["inference_time_ms"].toInt();

    QJsonDocument doc(obj);
    m_writeQueue.enqueue(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    m_queueNotEmpty.wakeOne();
}

void LogWriter::processQueue()
{
    LOG_THREAD_ID("LogWriter::processQueue");
    while (m_running) {
        QString line;
        {
            QMutexLocker locker(&m_queueMutex);
            if (m_writeQueue.isEmpty()) {
                m_queueNotEmpty.wait(&m_queueMutex, 1000);
                if (m_writeQueue.isEmpty() && !m_running) break;
                if (m_writeQueue.isEmpty()) continue;
            }
            line = m_writeQueue.dequeue();
        }

        if (!line.isEmpty() && m_logFile.isOpen()) {
            m_stream << line << "\n";
            // Периодически сбрасываем на диск
            static int counter = 0;
            if (++counter % 10 == 0) {
                m_stream.flush();
            }
        }
    }
}