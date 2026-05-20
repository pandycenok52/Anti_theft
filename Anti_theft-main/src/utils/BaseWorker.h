#ifndef BASEWORKER_H
#define BASEWORKER_H

#include <QObject>

class BaseWorker : public QObject
{
    Q_OBJECT
public:
    explicit BaseWorker(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void finished();
    void errorOccurred(const QString &error);
};

#endif // BASEWORKER_H