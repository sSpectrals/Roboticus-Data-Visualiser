import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import "./assets"

Row {
    id: addItem
    spacing: 10
    property color addButtonColor: "#2b2a2a"
    property color vectorButtonColor: "#2b2a2a"
    property color textColor: "#98FF98"
    property color hoverEffectColor: "#98FF98"
    signal addSensorRequested
    signal addVectorRequested

    Button {
        id: sensorButton
        width: (addItem.width - addItem.spacing) / 2
        height: addItem.height
        Material.foreground: addItem.textColor
        Material.background: addItem.addButtonColor
        Material.roundedScale: Button.None
        Material.elevation: sensorButton.hovered ? 3 : 1

        Text {
            text: "Add Sensor"
            anchors.centerIn: parent
            color: addItem.textColor
            font.bold: true
            minimumPixelSize: 10
            font.pixelSize: 22
        }

        background: Rectangle {
            anchors.fill: parent
            color: sensorButton.Material.background
            radius: 12
            border.color: Qt.darker(sensorButton.Material.background, 1.2)
            border.width: 1
            // Hover glow effect
            Rectangle {
                anchors.fill: parent
                color: addItem.hoverEffectColor
                radius: parent.radius
                opacity: sensorButton.hovered ? 0.15 : 0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 150
                    }
                }
            }
            // Pressed effect
            Rectangle {
                anchors.fill: parent
                color: "#1a1a1a"
                radius: parent.radius
                opacity: sensorButton.down ? 0.2 : 0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }
        }
        ToolTip.text: "Add new sensor (Ctrl+1)"
        ToolTip.visible: hovered && ToolTip.text.length > 0
        ToolTip.delay: 500
        ToolTip.toolTip.y: parent.height + 10
        onClicked: {
            addItem.addSensorRequested()
        }

        HoverHandler {
            cursorShape: sensorButton.hovered ? Qt.PointingHandCursor : Qt.defaultHandCursor
        }
    }

    Button {
        id: vectorButton
        width: (addItem.width - addItem.spacing) / 2
        height: addItem.height
        Material.foreground: addItem.textColor
        Material.background: addItem.vectorButtonColor
        Material.roundedScale: Button.None
        Material.elevation: vectorButton.hovered ? 3 : 1

        Text {
            text: "Add Vector"
            anchors.centerIn: parent
            color: addItem.textColor
            font.bold: true
            minimumPixelSize: 10
            font.pixelSize: 22
        }
        background: Rectangle {
            anchors.fill: parent
            color: vectorButton.Material.background
            radius: 12
            border.color: Qt.darker(vectorButton.Material.background, 1.2)
            border.width: 1
            // Hover glow effect
            Rectangle {
                anchors.fill: parent
                color: addItem.hoverEffectColor
                radius: parent.radius
                opacity: vectorButton.hovered ? 0.15 : 0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 150
                    }
                }
            }
            // Pressed effect
            Rectangle {
                anchors.fill: parent
                color: "#1a1a1a"
                radius: parent.radius
                opacity: vectorButton.down ? 0.2 : 0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                    }
                }
            }
        }
        ToolTip.text: "Add Vector (Ctrl+2)"
        ToolTip.visible: hovered && ToolTip.text.length > 0
        ToolTip.delay: 500
        ToolTip.toolTip.y: parent.height + 10
        onClicked: {
            addItem.addVectorRequested()
        }

        HoverHandler {
            cursorShape: vectorButton.hovered ? Qt.PointingHandCursor : Qt.defaultHandCursor
        }
    }

    Shortcut {
        sequence: "Ctrl+1"
        onActivated: addItem.addSensorRequested()
    }

    Shortcut {
        sequence: "Ctrl+2"
        onActivated: addItem.addVectorRequested()
    }
}
