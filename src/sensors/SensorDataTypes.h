#ifndef SENSORDATATYPES_H
#define SENSORDATATYPES_H

#include <QVector>
#include <QDateTime>

struct SensorDataBatch {
    qint64 timestamp;                    
    QVector<float> accelerometer;        
    QVector<float> gyroscope;             
    float light;                
};

Q_DECLARE_METATYPE(SensorDataBatch)

#endif // SENSORDATATYPES_H