import QtQuick 2.12
import QtQuick.Controls 2.12
import MTriangle 1.0

Item {
    visible: true
    width: 640
    height: 480

    MTriangle {
        NumberAnimation on t { from: -1; to: 1; duration: 4000; loops: Animation.Infinite }
    }
}
