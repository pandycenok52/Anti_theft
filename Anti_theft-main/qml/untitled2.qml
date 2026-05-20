import QtQuick 2.0
import Sailfish.Silica 1.0
import QtSensors 5.2

ApplicationWindow {
    id: appWindow
    initialPage: mainPageComponent

    Component {
        id: mainPageComponent
        MainPage { }
    }
}