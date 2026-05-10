import QtQuick

Item {
    id: root

    property int currentFrame: 0
    property int maxFrames: 0
    property real currentTime: 0.0
    property real maxTime: 0.0

    signal scrubbed(int frame)

    function formatTime(seconds) {
        var mins = Math.floor(seconds / 60)
        var secs = (seconds % 60).toFixed(1)
        return (mins > 0 ? mins + ":" : "") + (mins > 0 && secs < 10 ? "0" : "") + secs + "s"
    }

    Rectangle {
        id: progressTrack
        anchors {
            left: parent.left
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
        height: 28
        color: "#0f0f0f"
        radius: height / 2
        border.color: "#333333"

        // Green rogress fill
        Rectangle {
            id: progressFill
            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }
            width: root.maxFrames > 0 ? Math.max(0, (parent.width - 4) * (root.currentFrame / root.maxFrames)) : 0
            radius: height / 2
            color: "#98FF98"
            opacity: 0.7

            Behavior on width {
                NumberAnimation {
                    duration: 80
                    easing.type: Easing.OutQuad
                }
            }
        }

        // Green text layer (behind fill)
        Text {
            id: currentTimeText
            anchors { left: parent.left; leftMargin: 10; verticalCenter: parent.verticalCenter }
            text: formatTime(root.currentTime)
            color: "#98FF98"
            font.pixelSize: 11
            font.weight: Font.Bold
            z: 1
        }

        Text {
            id: frameText
            anchors.centerIn: parent
            text: "Frame %1 / %2".arg(root.currentFrame).arg(root.maxFrames)
            color: "#98FF98"
            font.pixelSize: 10
            font.weight: Font.Bold
            z: 1
        }

        Text {
            id: maxTimeText
            anchors { right: parent.right; rightMargin: 10; verticalCenter: parent.verticalCenter }
            text: formatTime(root.maxTime)
            color: "#98FF98"
            font.pixelSize: 11
            font.weight: Font.Bold
            z: 1
        }

        // Black text clipped to fill width (two-tone effect)
        Item {
            anchors { left: parent.left; top: parent.top; bottom: parent.bottom }
            width: progressFill.width + 4
            clip: true
            z: 2

            Text {
                text: currentTimeText.text
                color: "#0f0f0f"
                font.pixelSize: 11
                font.weight: Font.Bold
                x: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: frameText.text
                color: "#0f0f0f"
                font.pixelSize: 10
                font.weight: Font.Bold
                x: (progressTrack.width - width) / 2
                anchors.verticalCenter: parent.verticalCenter
            }

            Text {
                text: maxTimeText.text
                color: "#0f0f0f"
                font.pixelSize: 11
                font.weight: Font.Bold
                x: progressTrack.width - width - 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            onClicked: (mouse) => root.scrubbed(Math.round(Math.max(0, Math.min(1, mouse.x / parent.width)) * root.maxFrames))
            onPositionChanged: (mouse) => {
                if (pressed)
                    root.scrubbed(Math.round(Math.max(0, Math.min(1, mouse.x / parent.width)) * root.maxFrames))
            }
        }
    }
}