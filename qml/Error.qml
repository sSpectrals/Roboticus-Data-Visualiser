import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts

Popup {
    id: root
    width: Math.min(420, parent.width - 60)
    height: contentLayout.implicitHeight + 30
    x: parent.width - width - 30
    y: 30
    modal: false
    closePolicy: Popup.NoAutoClose
    padding: 0

    property string errorMessage: ""
    property int displayDuration: 3000

    function show(message) {
        errorMessage = message;
        root.open();
        autoDismissTimer.restart();
    }

    function dismiss() {
        autoDismissTimer.stop();
        root.close();
    }

    Timer {
        id: autoDismissTimer
        interval: root.displayDuration
        onTriggered: root.dismiss()
    }

    enter: Transition {
        NumberAnimation {
            property: "opacity"
            from: 0.0
            to: 1.0
            duration: 250
            easing.type: Easing.InOutQuad
        }
    }
    exit: Transition {
        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0.0
            duration: 250
            easing.type: Easing.InOutQuad
        }
    }

    background: Rectangle {
        color: "#1a1a1a"
        radius: 10
        border.width: 2
        border.color: "#FF6B6B"

        // Red accent bar on the left edge
        Rectangle {
            width: 4
            height: parent.height - 30
            radius: 2
            color: "#FF6B6B"
            anchors {
                left: parent.left
                verticalCenter: parent.verticalCenter
                leftMargin: 10
            }
        }
    }

    contentItem: RowLayout {
        id: contentLayout
        anchors.fill: parent
        anchors {
            leftMargin: 24
            rightMargin: 12
            topMargin: 12
            bottomMargin: 12
        }
        spacing: 12

        Column {
            Layout.fillWidth: true
            spacing: 4

            Text {
                text: "\u26A0 Error"
                font.pixelSize: 14
                font.weight: Font.DemiBold
                color: "#FF6B6B"
            }

            Text {
                text: root.errorMessage
                font.pixelSize: 13
                color: "#cccccc"
                wrapMode: Text.WordWrap
                width: parent.width
            }
        }

        // Close button
        Rectangle {
            Layout.preferredWidth: 28
            Layout.preferredHeight: 28
            Layout.alignment: Qt.AlignTop
            radius: 6
            color: "transparent"

            Behavior on color {
                ColorAnimation {
                    duration: 150
                }
            }

            Text {
                anchors.centerIn: parent
                text: "\u2715"
                font.pixelSize: 14
                color: closeArea.containsMouse ? "#FF6B6B" : "#888888"

                Behavior on color {
                    ColorAnimation {
                        duration: 150
                    }
                }
            }

            MouseArea {
                id: closeArea
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: root.dismiss()
            }
        }
    }
}
