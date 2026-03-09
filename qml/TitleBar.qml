import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts
import com.Roboticus.ControlCenter
import QtQuick.Controls.Material

Rectangle {
    id: title
    width: monitor.width
    height: 50
    color: "transparent"

    required property var serialParser

    anchors {
        left: parent.left
        bottomMargin: 20
        leftMargin: 20
        rightMargin: 20
    }

    RowLayout {
        id: contentRow
        width: parent.width - 20
        height: parent.height
        anchors {
            left: parent.left
            leftMargin: 10
            verticalCenter: parent.verticalCenter
        }
        spacing: 20

        ComboBox {
            id: comSelection
            Layout.preferredWidth: 150
            Layout.preferredHeight: title.height * 0.6
            Layout.alignment: Qt.AlignVCenter
            model: serialParser.availablePortsList.length > 0 ? serialParser.availablePortsList : ["No COM Port found"]
            Layout.fillWidth: true

            Material.accent: "#98FF98"
            Material.foreground: "#98FF98"

            currentIndex: 0

            Connections {
                target: serialParser
                function onAvailablePortsChanged() {
                    if (serialParser.availablePortsList.length > 0) {
                        comSelection.currentIndex = 0;
                        serialParser.setComPort(serialParser.availablePortsList[0]);
                    }
                }
            }

            Component.onCompleted: {
                // Initialize baud rate on startup
                serialParser.setBaudRate(baudSelection.model[baudSelection.currentIndex]);
                // Initialize COM port if available
                if (serialParser.availablePortsList.length > 0) {
                    serialParser.setComPort(serialParser.availablePortsList[0]);
                }
            }

            background: Rectangle {
                color: "#0f0f0f"
                border.color: comSelection.hovered ? "#98FF98" : "#333333"
                border.width: 2
                radius: 4

                Behavior on border.color {
                    ColorAnimation {
                        duration: 150
                    }
                }
            }

            popup: Popup {
                y: comSelection.height
                width: comSelection.width
                height: implicitHeight
                padding: 3
                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: comSelection.popup.visible ? comSelection.delegateModel : null
                    currentIndex: comSelection.highlightedIndex

                    ScrollIndicator.vertical: ScrollIndicator {}
                }

                background: Rectangle {
                    color: "#1a1a1a"
                    border.color: "#98FF98"
                    border.width: 2
                    radius: 4
                }
            }

            delegate: ItemDelegate {
                width: comSelection.width
                hoverEnabled: true

                contentItem: Text {
                    text: modelData
                    color: parent.highlighted || parent.hovered ? "#98FF98" : "#888888"
                }

                highlighted: comSelection.highlightedIndex === index

                background: Rectangle {
                    color: parent.highlighted || parent.hovered ? "#0f0f0f" : "transparent"

                    Behavior on color {
                        ColorAnimation {
                            duration: 100
                        }
                    }
                }
            }

            onActivated: function (index) {
                serialParser.setComPort(model[index]);
                focus = false;
            }
        }

        ComboBox {
            id: baudSelection
            Layout.preferredWidth: 110
            Layout.preferredHeight: title.height * 0.6
            Layout.alignment: Qt.AlignVCenter
            model: [38400, 57600, 115200, 230400, 460800, 921600]
            currentIndex: 2

            Material.accent: "#98FF98"
            Material.foreground: "#98FF98"
            Layout.fillWidth: true

            background: Rectangle {
                color: "#0f0f0f"
                border.color: baudSelection.hovered ? "#98FF98" : "#333333"
                border.width: 2
                radius: 4

                Behavior on border.color {
                    ColorAnimation {
                        duration: 150
                    }
                }
            }

            popup: Popup {
                y: baudSelection.height
                width: baudSelection.width
                height: implicitHeight
                padding: 3
                contentItem: ListView {
                    clip: true
                    implicitHeight: contentHeight
                    model: baudSelection.popup.visible ? baudSelection.delegateModel : null
                    currentIndex: baudSelection.highlightedIndex

                    ScrollIndicator.vertical: ScrollIndicator {}
                }

                background: Rectangle {
                    color: "#1a1a1a"
                    border.color: "#98FF98"
                    border.width: 2
                    radius: 4
                }
            }

            delegate: ItemDelegate {
                width: baudSelection.width
                hoverEnabled: true

                contentItem: Text {
                    text: modelData
                    color: parent.highlighted || parent.hovered ? "#98FF98" : "#888888"
                }

                highlighted: baudSelection.highlightedIndex === index

                background: Rectangle {
                    color: parent.highlighted || parent.hovered ? "#0f0f0f" : "transparent"

                    Behavior on color {
                        ColorAnimation {
                            duration: 100
                        }
                    }
                }
            }

            onActivated: function (index) {
                serialParser.setBaudRate(model[index]);
                focus = false;
            }
        }

        Button {
            id: startMonitor
            Layout.preferredWidth: 150
            Layout.fillWidth: true
            Layout.preferredHeight: title.height * 0.6
            Layout.alignment: Qt.AlignVCenter

            Material.accent: "#98FF98"
            Material.foreground: "#98FF98"
            Material.roundedScale: Button.None
            Material.elevation: startMonitor.hovered ? 3 : 1

            Text {
                text: serialParser.isConnected ? "Stop Monitor" : "Start Monitor"
                anchors.centerIn: parent
                color: "#98FF98"
                font.bold: true
                minimumPixelSize: 8
                font.pixelSize: 14
            }

            background: Rectangle {

                anchors.fill: parent
                color: "#0f0f0f"
                border.color: startMonitor.hovered ? "#98FF98" : "#333333"
                border.width: 2
                radius: 4

                // Pressed effect
                Rectangle {
                    anchors.fill: parent
                    color: "#1a1a1a"
                    radius: 4
                    opacity: startMonitor.down ? 0.2 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }

            onClicked: {
                if (serialParser.isConnected) {
                    serialParser.disconnectPort();
                } else {
                    serialParser.connectToPort();
                }
            }
            HoverHandler {
                cursorShape: startMonitor.hovered ? Qt.PointingHandCursor : Qt.defaultHandCursor
            }
        }
    }

    Rectangle {
        id: greenLine
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
