import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: mainPage

    property var lastInferenceResult: ({})

    Connections {
        target: inferenceWorker
        function onInferenceResultReady(result) {
            lastInferenceResult = result;
            updateDisplay();
        }
    }

    function updateDisplay() {
        var res = lastInferenceResult;
        classLabel.text = res.class_name ? res.class_name : "—";
        confidenceLabel.text = res.confidence ? (res.confidence * 100).toFixed(1) + "%" : "—";
        timeLabel.text = res.inference_time_ms ? res.inference_time_ms + " ms" : "—";
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: contentColumn.height

        Column {
            id: contentColumn
            width: parent.width
            spacing: Theme.paddingLarge

            PageHeader {
                title: "ML Sensor Monitor"
            }

            SectionHeader { text: "Текущие показания" }

            Label {
                id: accelLabel
                text: "Акселерометр: —"
                color: Theme.highlightColor
                anchors { left: parent.left; right: parent.right; margins: Theme.horizontalPageMargin }
            }

            Label {
                id: lightLabel
                text: "Освещённость: —"
                color: Theme.highlightColor
                anchors { left: parent.left; right: parent.right; margins: Theme.horizontalPageMargin }
            }

            SectionHeader { text: "Результат инференса" }

            Label {
                text: "Класс активности:"
                color: Theme.secondaryColor
                anchors { left: parent.left; right: parent.right; margins: Theme.horizontalPageMargin }
            }
            Label {
                id: classLabel
                text: "—"
                font.pixelSize: Theme.fontSizeExtraLarge
                color: Theme.primaryColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                text: "Уверенность:"
                color: Theme.secondaryColor
                anchors { left: parent.left; right: parent.right; margins: Theme.horizontalPageMargin }
            }
            Label {
                id: confidenceLabel
                text: "—"
                color: Theme.primaryColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                text: "Время инференса:"
                color: Theme.secondaryColor
                anchors { left: parent.left; right: parent.right; margins: Theme.horizontalPageMargin }
            }
            Label {
                id: timeLabel
                text: "—"
                color: Theme.primaryColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: "Показать логи"
                onClicked: pageStack.push(Qt.resolvedUrl("LogsPage.qml"))
                anchors.horizontalCenter: parent.horizontalCenter
            }

            // Таймер для обновления показаний сенсоров (из QML-контекста напрямую)
            Timer {
                interval: 200
                running: true
                repeat: true
                onTriggered: {
                    if (sensorCollector) {
                        // Здесь можно получать последние значения через свойства,
                        // но проще добавить сигнал от SensorCollector с последним батчем.
                        // Для простоты показываем заглушку.
                        accelLabel.text = "Акселерометр: данные поступают...";
                        lightLabel.text = "Освещённость: данные поступают...";
                    }
                }
            }
        }
    }
    // Более удобный способ отображения сырых данных – подключить сигнал от SensorCollector
    Connections {
        target: sensorCollector
        function onNewDataBatch(batch) {
            if (batch.accelerometer.length >= 3) {
                accelLabel.text = "Акселерометр: X=" + batch.accelerometer[0].toFixed(2) +
                                  ", Y=" + batch.accelerometer[1].toFixed(2) +
                                  ", Z=" + batch.accelerometer[2].toFixed(2);
            }
            lightLabel.text = "Освещённость: " + batch.light.toFixed(1) + " lux";
        }
    }
}