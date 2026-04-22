#ifndef LOGWRITER_H
#define LOGWRITER_H

#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>
#include <QTextStream>
#include "sensors/SensorDataTypes.h"
#include "utils/BaseWorker.h"

class LogWriter : public BaseWorker
{
    Q_OBJECT
public:
    explicit LogWriter(QObject *parent = nullptr);
    ~LogWriter();

public slots:
    void start();
    void stop();
    void writeRawData(const SensorDataBatch &batch);
    void writeInferenceResult(const QVariantMap &result);

private slots:
    void processQueue();

private:
    QQueue<QString> m_writeQueue;
    QMutex m_queueMutex;
    QWaitCondition m_queueNotEmpty;
    QFile m_logFile;
    QTextStream m_stream;
    bool m_running;
};

#endif // LOGWRITER_H