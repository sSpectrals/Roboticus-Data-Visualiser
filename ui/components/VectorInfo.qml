import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Rectangle {
    id: vectorInfo
    property string vectorName: "No Name Set"
    property var vectorID: -1
    property double rotationValue: 1.0
    property double scale: 1.0
    property color vectorColor: "white"
    property real xLocation: 0
    property real yLocation: 0
    property bool selected: false

    signal clicked
    signal deleteVector

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
        columns: 4
        rows: 1

        // Column 1: Vector Name
        Rectangle {
            Layout.preferredWidth: parent.width * 0.3
            Layout.fillHeight: true
            color: "transparent"

            TextField {
                id: textInput
                color: "white"
                text: vectorName
                font.bold: true
                font.pixelSize: 14
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                selectByMouse: true
                selectionColor: "#98FF98"
                selectedTextColor: "#1a1a1a"

                background: Rectangle {
                    color: "transparent"
                    border.color: textInput.hovered ? "#98FF98" : "transparent"
                    border.width: 2
                    radius: 4

                    Behavior on border.color {
                        ColorAnimation {
                            duration: 150
                        }
                    }
                }

                // onEditingFinished: {

                // }

                // onAccepted: {
                //     vectorName = text
                //     focus = false
                // }
                onActiveFocusChanged: {
                    if (activeFocus) {

                        // Gained focus
                    } else {
                        if (text.trim() === "") {
                            vectorName = "No vector name";
                        } else {
                            vectorName = text;
                        }
                        focus = false;
                    }
                }
            }
        }

        //column 2: (x,y)
        Rectangle {
            Layout.preferredWidth: parent.width * 0.1
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

        // Column 3: rotation | color
        Rectangle {
            Layout.preferredWidth: parent.width * 0.5
            Layout.fillHeight: true
            color: "#1a1a1a"
            radius: 10

            RowLayout {
                anchors.fill: parent
                spacing: 5

                // rotation
                Rectangle {
                    Layout.preferredWidth: 20
                    Layout.fillHeight: true
                    color: "transparent"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    Text {
                        anchors.centerIn: parent
                        color: "white"
                        text: "rotation: " + (((rotationValue % 360) + 540) % 360 - 180).toFixed(1)
                        font.pixelSize: 14
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                }

                // Color
                Rectangle {
                    id: vectorColor
                    radius: 10
                    Layout.preferredWidth: 20
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    color: vectorInfo.vectorColor
                }
            }
        }

        // column 4: delete
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Material.color(Material.Red)
            radius: 10
            Image {
                source: "../../assets/SVG/trash.svg"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    deleteVector();
                }
            }
        }
    }
}
