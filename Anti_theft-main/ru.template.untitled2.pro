TARGET = ru.template.untitled2

CONFIG += auroraapp c++17

QT += sensors gui core

# -------- Определение архитектуры и версии --------
arch = x86_64
contains(QMAKE_SYSTEM_PROCESSOR, "i486") {
    arch = i486
}
contains(QMAKE_SYSTEM_PROCESSOR, "arm") {
    arch = armv7l
}
contains(QMAKE_SYSTEM_PROCESSOR, "aarch64") {
    arch = aarch64
}

version = $${AURORA_TARGET_MAJOR_VERSION}
isEmpty(version) {
    version = 5
}

VENDOR_PATH = $$PWD/vendor/$${arch}_$${version}

# -------- Подключение TensorFlow Lite --------
INCLUDEPATH += $$VENDOR_PATH/include
LIBS += -L$$VENDOR_PATH/lib -ltensorflow-lite

# -------- Пути к исходникам --------
INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/sensors
INCLUDEPATH += $$PWD/src/utils
INCLUDEPATH += $$PWD/src/inference
INCLUDEPATH += $$PWD/src/logging

QMAKE_CXXFLAGS += -pthread
LIBS += -pthread

SOURCES += \
    src/main.cpp \
    src/sensors/SensorCollector.cpp \
    src/inference/InferenceWorker.cpp \
    src/logging/LogWriter.cpp \

HEADERS += \
    src/sensors/SensorCollector.h \
    src/sensors/SensorDataTypes.h \        # убедитесь, что этот файл существует
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
