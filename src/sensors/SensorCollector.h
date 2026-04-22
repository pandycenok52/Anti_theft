#ifndef SENSORCOLLECTOR_H
#define SENSORCOLLECTOR_H

#include <QObject>
#include <QAccelerometer>
#include <QAmbientLightSensor>
#include <QGyroscope>
#include <QTimer>
#include "SensorDataTypes.h"
#include "utils/BaseWorker.h"

class SensorCollector : public BaseWorker
{
    Q_OBJECT
public:
    explicit SensorCollector(QObject *parent = nullptr);
    ~SensorCollector();

public slots:
    void startCollection();
    void stopCollection();

signals:
    void newDataBatch(const SensorDataBatch &batch);

private slots:
    void onAccelerometerReadingChanged();
    void onLightSensorReadingChanged();
    void onGyroscopeReadingChanged();
    void emitBatch();

private:
    QAccelerometer *m_accel;
    QAmbientLightSensor *m_light;
    QGyroscope *m_gyro;
    QTimer m_batchTimer;

    // Последние считанные значения
    QVector3D m_lastAccel;
    QVector3D m_lastGyro;
    float m_lastLight;

    bool m_running;
};

#endif // SENSORCOLLECTOR_H