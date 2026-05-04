import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: sensorInfo
    property string sensorName: "No Name Set"
    property var sensorID: -1
    property double inputValue: -1.0
    property double thresholdValue: 100.0
    property real xLocation: 0
    property real yLocation: 0
    property bool selected: false
    property bool isTriggered: true

    signal clicked

    color: "black"
    radius: 10
    border {
        width: 2
        color: (hoverHandler.hovered || selected) ? "#98FF98" : "#333333"
    }

    Behavior on border.color {
        ColorAnimation {
            duration: 150
        }
    }

    HoverHandler {
        id: hoverHandler
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
    }

    Rectangle {
        width: 6
        height: parent.height * 0.6
        color: "white"
        radius: 3
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
            leftMargin: 8
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 8
        columns: 3
        rows: 1

        // Column 1: Sensor Name
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            Text {
                anchors.centerIn: parent
                color: "white"
                text: sensorName
                font.bold: true
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        // Column 2: (x,y)
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"

            Text {
                color: "white"
                font.bold: true
                font.pixelSize: 14
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "(" + xLocation + ", " + yLocation + ")"
            }
        }

        // Column 3: input | / | threshold
        Rectangle {
            Layout.preferredWidth: parent.width * 0.3
            Layout.fillHeight: true
            color: "#1a1a1a"
            radius: 10
            border.color: isTriggered ? Material.color(Material.Red) : "#333333"
            border.width: 2

            RowLayout {
                anchors.centerIn: parent
                spacing: 0

                Text {
                    padding: 8
                    text: inputValue
                    color: "white"
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle {
                    width: 1
                    height: 20
                    color: "#333333"
                }

                Text {
                    padding: 8
                    text: "/"
                    color: isTriggered ? Material.color(Material.Red) : "#98FF98"
                    font.bold: true
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                Rectangle {
                    width: 1
                    height: 20
                    color: "#333333"
                }

                Text {
                    padding: 8
                    text: thresholdValue
                    color: "white"
                    font.pixelSize: 14
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
}
