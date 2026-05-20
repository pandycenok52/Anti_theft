#include <auroraapp.h>
#include <QtQuick>
#include <QThread>
#include "sensors/SensorCollector.h"
#include "inference/InferenceWorker.h"
#include "logging/LogWriter.h"
#include "sensors/SensorDataTypes.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.template"));
    application->setApplicationName(QStringLiteral("untitled2"));

    // Регистрируем типы для использования в сигналах/слотах
    qRegisterMetaType<SensorDataBatch>("SensorDataBatch");
    qRegisterMetaType<QVariantMap>("QVariantMap");

    // Создаём воркеров
    SensorCollector *collector = new SensorCollector();
    InferenceWorker *inference = new InferenceWorker();
    LogWriter *logger = new LogWriter();

    // Создаём потоки
    QThread *sensorThread = new QThread();
    QThread *inferenceThread = new QThread();
    QThread *logThread = new QThread();

    collector->moveToThread(sensorThread);
    inference->moveToThread(inferenceThread);
    logger->moveToThread(logThread);

    // Управление жизненным циклом потоков
    QObject::connect(sensorThread, &QThread::started, collector, &SensorCollector::startCollection);
    QObject::connect(sensorThread, &QThread::finished, collector, &QObject::deleteLater);
    QObject::connect(collector, &SensorCollector::finished, sensorThread, &QThread::quit);
    QObject::connect(collector, &SensorCollector::errorOccurred,
                     [](const QString &err){ qCritical() << "SensorCollector error:" << err; });

    QObject::connect(inferenceThread, &QThread::started, inference, &InferenceWorker::runInferenceLoop);
    QObject::connect(inferenceThread, &QThread::finished, inference, &QObject::deleteLater);
    QObject::connect(inference, &InferenceWorker::finished, inferenceThread, &QThread::quit);
    QObject::connect(inference, &InferenceWorker::errorOccurred,
                     [](const QString &err){ qCritical() << "InferenceWorker error:" << err; });

    QObject::connect(logThread, &QThread::started, logger, &LogWriter::start);
    QObject::connect(logThread, &QThread::finished, logger, &QObject::deleteLater);
    QObject::connect(logger, &LogWriter::finished, logThread, &QThread::quit);
    QObject::connect(logger, &LogWriter::errorOccurred,
                     [](const QString &err){ qCritical() << "LogWriter error:" << err; });

    // Связываем данные
    // Сенсор -> Инференс и Логгер
    QObject::connect(collector, &SensorCollector::newDataBatch,
                     inference, &InferenceWorker::processNewData);
    QObject::connect(collector, &SensorCollector::newDataBatch,
                     logger, &LogWriter::writeRawData);
    // Инференс -> Логгер и UI
    QObject::connect(inference, &InferenceWorker::inferenceResultReady,
                     logger, &LogWriter::writeInferenceResult);

    // Создаём QML view
    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    QQmlContext *context = view->rootContext();

    // Передаём объекты в QML (только для чтения результатов)
    context->setContextProperty("inferenceWorker", inference);
    context->setContextProperty("sensorCollector", collector);

    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/untitled2.qml")));
    view->show();

    // Запускаем потоки
    sensorThread->start();
    inferenceThread->start();
    logThread->start();

    int ret = application->exec();

    // Остановка потоков
    sensorThread->quit();
    inferenceThread->quit();
    logThread->quit();
    sensorThread->wait();
    inferenceThread->wait();
    logThread->wait();

    return ret;
}