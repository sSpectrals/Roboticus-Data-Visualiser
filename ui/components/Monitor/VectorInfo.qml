import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: vectorInfo
    property string vectorName: "No Name Set"
    property double rotationValue: 1.0
    property double scale: 1.0
    property color vectorColor: "white"
    property real xLocation: 0
    property real yLocation: 0
    property bool selected: false

    signal clicked

    color: "black"
    radius: 10
    border {
        width: 2
        color: hoverHandler.hovered || selected ? "#98FF98" : "#333333"
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

    // left accent bar, color could possible be used to signify something
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

        // Column 1: Vector Name
        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 0
            clip: true
            elide: Text.ElideRight

            color: "white"
            text: vectorName
            font.bold: true
            font.pixelSize: 14
        }

        //column 2: (x,y)
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

        // Column 3: rotation
        Text {
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 0
            clip: true
            elide: Text.ElideRight

            color: "white"
            font.pixelSize: 14
            text: "rotation: " + (((rotationValue % 360) + 540) % 360 - 180).toFixed(1)
        }

        // Column 4: Color
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredWidth: 0
            radius: 10
            color: vectorColor
        }
    }
}
