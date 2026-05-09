import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: sensorInfo

    property string sensorName: "No Name Set"
    property double inputValue: -1.0
    property double thresholdValue: 100.0
    property real xLocation: 0
    property real yLocation: 0
    property bool selected: false
    property bool isTriggered: true

    signal clicked

    color: "black"
    radius: 10
    border.width: 2
    border.color: (hoverHandler.hovered || selected) ? "#98FF98" : "#333333"

    Behavior on border.color {
        ColorAnimation { duration: 150 }
    }

    HoverHandler {
        id: hoverHandler
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad
    }

    // Left accent bar
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

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        // Column 1: Sensor name
        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 0
            clip: true
            elide: Text.ElideRight

            color: "white"
            text: sensorName
            font.bold: true
            font.pixelSize: 14
        }

        // Column 2: Position
        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 0
            clip: true
            elide: Text.ElideRight

            color: "white"
            font.bold: true
            font.pixelSize: 14
            text: "(%1, %2)".arg(xLocation).arg(yLocation)
        }

        // Column 3: Input / Threshold
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 0
            color: "#1a1a1a"
            radius: 10
            border.width: 2
            border.color: isTriggered ? Material.color(Material.Red) : "#333333"

            RowLayout {
                anchors.centerIn: parent
                spacing: 0

                // Input
                Text {
                    padding: 8
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                    font.pixelSize: 14
                    text: inputValue
                }

                // |
                Rectangle {
                    width: 1
                    height: 20
                    color: "#333333"
                }


                // "/"
                Text {
                    padding: 8
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: isTriggered ? Material.color(Material.Red) : "#98FF98"
                    font.bold: true
                    font.pixelSize: 14
                    text: "/"
                }

                // |
                Rectangle {
                    width: 1
                    height: 20
                    color: "#333333"
                }

                // Threshold
                Text {
                    padding: 8
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                    font.pixelSize: 14
                    text: thresholdValue
                }
            }
        }
    }
}