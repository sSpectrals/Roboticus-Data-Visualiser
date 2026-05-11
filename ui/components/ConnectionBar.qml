import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts
import com.Roboticus.DataVisualiser
import QtQuick.Controls.Material

Rectangle {
    id: connectionBar

    width: monitor.width
    height: 50
    color: "transparent"

    required property var portManager

    anchors {
        left: parent.left
        leftMargin: 20
        rightMargin: 20
    }


    Component.onCompleted: {
        portManager.setBaudRate(baudSelection.model[baudSelection.currentIndex])
        if (portManager.availablePortsList.length > 0)
            portManager.setComPort(portManager.availablePortsList[0])
    }

    // com | baud | start button
    RowLayout {
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
            leftMargin: 10
            rightMargin: 10
        }
        height: parent.height
        spacing: 20

        StyledComboBox {
            id: comSelection
            Layout.preferredWidth: 150
            Layout.preferredHeight: connectionBar.height * 0.6
            Layout.fillWidth: true
            model: portManager.availablePortsList.length > 0 ? portManager.availablePortsList : ["No COM Port found"]
            currentIndex: 0

            Connections {
                target: portManager
                function onAvailablePortsChanged() {
                    if (portManager.availablePortsList.length > 0) {
                        comSelection.currentIndex = 0
                        portManager.setComPort(portManager.availablePortsList[0])
                    }
                }
            }

            onActivated: (index) => {
                portManager.setComPort(model[index])
                focus = false
            }
        }

        StyledComboBox {
            id: baudSelection
            Layout.preferredWidth: 110
            Layout.preferredHeight: connectionBar.height * 0.6
            Layout.fillWidth: true
            model: [38400, 57600, 115200, 230400, 460800, 921600]
            currentIndex: 2

            onActivated: (index) => {
                portManager.setBaudRate(model[index])
                focus = false
            }
        }

        Button {
            id: startMonitor
            Layout.preferredWidth: 150
            Layout.fillWidth: true
            Layout.preferredHeight: connectionBar.height * 0.6
            Layout.alignment: Qt.AlignVCenter

            Material.accent: "#98FF98"
            Material.foreground: "#98FF98"
            Material.roundedScale: Button.None
            Material.elevation: startMonitor.hovered ? 3 : 1

            Text {
                anchors.centerIn: parent
                color: "#98FF98"
                text: portManager.isConnected ? "Stop Monitor" : "Start Monitor"
                font.bold: true
                font.pixelSize: 14
                minimumPixelSize: 8
            }

            background: Rectangle {
                anchors.fill: parent
                color: "#0f0f0f"
                border.width: 2
                border.color: startMonitor.hovered ? "#98FF98" : "#333333"
                radius: 4

                // Pressed effect
                Rectangle {
                    anchors.fill: parent
                    color: "#1a1a1a"
                    radius: 4
                    opacity: startMonitor.down ? 0.2 : 0
                    Behavior on opacity {
                        NumberAnimation { duration: 100 }
                    }
                }
            }

            onClicked: {
                if (portManager.isConnected)
                    portManager.disconnectPort()
                else
                    portManager.connectToPort()
            }

            HoverHandler {
                cursorShape: Qt.PointingHandCursor
            }
        }
    }

    // Green line below to divide the connection bar
    Rectangle {
        width: parent.width - 20
        height: 2
        color: "#98FF98"
        opacity: 0.6
        anchors {
            left: parent.left
            bottom: parent.bottom
            leftMargin: 10
        }
    }
}