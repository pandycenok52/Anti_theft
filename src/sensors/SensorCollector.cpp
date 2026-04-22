#include "SensorCollector.h"
#include "utils/ThreadLogger.h"
#include <QCoreApplication>

SensorCollector::SensorCollector(QObject *parent)
    : BaseWorker(parent)
    , m_accel(new QAccelerometer(this))
    , m_light(new QAmbientLightSensor(this))
    , m_gyro(new QGyroscope(this))
    , m_lastLight(0.0f)
    , m_running(false)
{
    // Настройка таймера для пакетной отправки (10 Гц)
    m_batchTimer.setInterval(100);
    connect(&m_batchTimer, &QTimer::timeout, this, &SensorCollector::emitBatch);
}

SensorCollector::~SensorCollector()
{
    stopCollection();
}

void SensorCollector::startCollection()
{
    LOG_THREAD_ID("SensorCollector::startCollection");

    if (!m_accel->start()) {
        emit errorOccurred("Failed to start accelerometer");
        return;
    }
    if (!m_light->start()) {
        emit errorOccurred("Failed to start light sensor");
        return;
    }
    if (!m_gyro->start()) {
        qWarning() << "Gyroscope not available";
    }

    // Подключаем сигналы напрямую
    connect(m_accel, &QAccelerometer::readingChanged,
            this, &SensorCollector::onAccelerometerReadingChanged, Qt::DirectConnection);
    connect(m_light, &QAmbientLightSensor::readingChanged,
            this, &SensorCollector::onLightSensorReadingChanged, Qt::DirectConnection);
    connect(m_gyro, &QGyroscope::readingChanged,
            this, &SensorCollector::onGyroscopeReadingChanged, Qt::DirectConnection);

    m_running = true;
    m_batchTimer.start();
    qDebug() << "SensorCollector started.";
}

void SensorCollector::stopCollection()
{
    LOG_THREAD_ID("SensorCollector::stopCollection");
    m_running = false;
    m_batchTimer.stop();
    m_accel->stop();
    m_light->stop();
    m_gyro->stop();
    emit finished();
}

void SensorCollector::onAccelerometerReadingChanged()
{
    auto reading = m_accel->reading();
    if (reading) {
        m_lastAccel.setX(reading->x());
        m_lastAccel.setY(reading->y());
        m_lastAccel.setZ(reading->z());
    }
}

void SensorCollector::onLightSensorReadingChanged()
{
    auto reading = m_light->reading();
    if (reading) {
        m_lastLight = reading->lightLevel();
    }
}

void SensorCollector::onGyroscopeReadingChanged()
{
    auto reading = m_gyro->reading();
    if (reading) {
        m_lastGyro.setX(reading->x());
        m_lastGyro.setY(reading->y());
        m_lastGyro.setZ(reading->z());
    }
}

void SensorCollector::emitBatch()
{
    if (!m_running) return;

    SensorDataBatch batch;
    batch.timestamp = QDateTime::currentMSecsSinceEpoch();
    batch.accelerometer = {m_lastAccel.x(), m_lastAccel.y(), m_lastAccel.z()};
    batch.gyroscope = {m_lastGyro.x(), m_lastGyro.y(), m_lastGyro.z()};
    batch.light = m_lastLight;

    emit newDataBatch(batch);
}