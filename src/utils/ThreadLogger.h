#ifndef THREADLOGGER_H
#define THREADLOGGER_H

#include <QThread>
#include <QDebug>

#define LOG_THREAD_ID(funcName) \
    qDebug() << "[" << QThread::currentThreadId() << "] " << funcName

#endif // THREADLOGGER_H