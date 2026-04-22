TARGET = ru.template.untitled2

CONFIG += auroraapp c++17

INCLUDEPATH += $$PWD/3rdparty/tensorflow
LIBS += -L$$PWD/3rdparty/tensorflow/lib -ltensorflow-lite

QMAKE_CXXFLAGS += -pthread
LIBS += -pthread

SOURCES += \
    src/main.cpp \
    src/sensors/SensorCollector.cpp \
    src/inference/InferenceWorker.cpp \
    src/logging/LogWriter.cpp \

HEADERS += \
    src/sensors/SensorCollector.h \
    src/sensors/SensorDataTypes.h \
    src/inference/InferenceWorker.h \
    src/logging/LogWriter.h \
    src/utils/ThreadLogger.h \
    src/utils/BaseWorker.h

DISTFILES += \
    rpm/ru.template.untitled2.spec \
    src/inference/model.tflite

model_files.files = src/inference/model.tflite
model_files.path = $$OUT_PWD
INSTALLS += model_files

AURORAAPP_ICONS = 86x86 108x108 128x128 172x172

CONFIG += auroraapp_i18n

TRANSLATIONS += \
    translations/ru.template.untitled2.ts \
    translations/ru.template.untitled2-ru.ts