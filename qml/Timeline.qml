import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material
import QtQuick.Layouts
import com.Roboticus.ControlCenter

Rectangle {
    id: timeline
    color: "#2b2a2a"
    radius: 12
    border.color: Qt.darker(color, 1.2)
    border.width: 1

    required property var serialParser

    property int currentFrame: 0
    property int maxFrames: 0
    property real currentTime: 0.0
    property real maxTime: 60.0

    function updateTimelineProps() {
        maxFrames = serialParser.snapshotCount() > 0 ? serialParser.snapshotCount() - 1 : 0;
        currentTime = serialParser.snapshotCount() > 0 ? serialParser.timestampAt(currentFrame) / 1000.0 : 0.0;
        maxTime = serialParser.snapshotCount() > 0 ? serialParser.timestampAt(serialParser.snapshotCount() - 1) / 1000.0 : 60.0;
    }

    Component.onCompleted: updateTimelineProps()
    Connections {
        target: serialParser
        function onSnapshotsChanged() {
            updateTimelineProps();
        }
    }
    onCurrentFrameChanged: updateTimelineProps()

    signal frameBack
    signal frameForward
    signal saveRequested
    signal loadRequested

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        // Back button
        Button {
            id: backButton
            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter
            enabled: timeline.currentFrame > 0

            background: Rectangle {
                color: "#1a1a1a"
                radius: 8
                border.color: backButton.hovered ? "#98FF98" : "#333333"
                border.width: 1

                Behavior on border.color {
                    ColorAnimation {
                        duration: 150
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#98FF98"
                    radius: parent.radius
                    opacity: backButton.hovered ? 0.15 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 150
                        }
                    }
                }
                Rectangle {
                    anchors.fill: parent
                    color: "#1a1a1a"
                    radius: parent.radius
                    opacity: backButton.down ? 0.2 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }

            Image {
                source: backButton.enabled ? "./assets/SVG/arrow_left_green.svg" : "./assets/SVG/arrow_left_grey.svg"
                anchors.centerIn: parent
                width: parent.width * 0.6
                height: parent.height * 0.6
                fillMode: Image.PreserveAspectFit
            }

            onClicked: {
                if (timeline.currentFrame > 0) {
                    timeline.currentFrame--;
                    serialParser.restoreToIndex(currentFrame);
                }
            }

            HoverHandler {
                cursorShape: backButton.hovered ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }

        // Forward button
        Button {
            id: forwardButton
            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter
            enabled: timeline.currentFrame < timeline.maxFrames

            background: Rectangle {
                color: "#1a1a1a"
                radius: 8
                border.color: forwardButton.hovered ? "#98FF98" : "#333333"
                border.width: 1

                Behavior on border.color {
                    ColorAnimation {
                        duration: 150
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#98FF98"
                    radius: parent.radius
                    opacity: forwardButton.hovered ? 0.15 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 150
                        }
                    }
                }
                Rectangle {
                    anchors.fill: parent
                    color: "#1a1a1a"
                    radius: parent.radius
                    opacity: forwardButton.down ? 0.2 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }

            Image {
                source: forwardButton.enabled ? "./assets/SVG/arrow_right_green.svg" : "./assets/SVG/arrow_right_grey.svg"
                anchors.centerIn: parent
                width: parent.width * 0.6
                height: parent.height * 0.6
                fillMode: Image.PreserveAspectFit
            }

            onClicked: {
                if (timeline.currentFrame < timeline.maxFrames) {
                    timeline.currentFrame++;
                    serialParser.restoreToIndex(currentFrame);
                }
            }

            HoverHandler {
                cursorShape: forwardButton.hovered ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }

        // Center section: progress bar with text inside
        Item {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter

            // Progress bar track
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
                border.width: 1

                // Progress fill
                Rectangle {
                    id: progressFill
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                        margins: 2
                    }
                    width: timeline.maxFrames > 0 ? Math.max(0, (parent.width - 4) * (timeline.currentFrame / timeline.maxFrames)) : 0
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

                // Green text labels (always visible, behind clip layer)
                Text {
                    id: currentTimeText
                    text: formatTime(timeline.currentTime)
                    color: "#98FF98"
                    font.pixelSize: 11
                    font.weight: Font.Bold
                    z: 1
                    anchors {
                        left: parent.left
                        leftMargin: 10
                        verticalCenter: parent.verticalCenter
                    }
                }

                Text {
                    id: frameText
                    text: "Frame " + timeline.currentFrame + " / " + timeline.maxFrames
                    color: "#98FF98"
                    font.weight: Font.Bold
                    font.pixelSize: 10
                    z: 1
                    anchors.centerIn: parent
                }

                Text {
                    id: maxTimeText
                    text: formatTime(timeline.maxTime)
                    color: "#98FF98"
                    font.pixelSize: 11
                    font.weight: Font.Bold
                    z: 1
                    anchors {
                        right: parent.right
                        rightMargin: 10
                        verticalCenter: parent.verticalCenter
                    }
                }

                // Black text overlay clipped to fill width (per-character color)
                Item {
                    id: blackTextClip
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                    }
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

                // Clickable scrub area
                MouseArea {
                    anchors.fill: parent
                    onClicked: function (mouse) {
                        var ratio = Math.max(0, Math.min(1, mouse.x / parent.width));
                        timeline.currentFrame = Math.round(ratio * timeline.maxFrames);
                        serialParser.restoreToIndex(currentFrame);
                    }
                    onPositionChanged: function (mouse) {
                        if (pressed) {
                            var ratio = Math.max(0, Math.min(1, mouse.x / parent.width));
                            timeline.currentFrame = Math.round(ratio * timeline.maxFrames);
                        }
                    }
                    cursorShape: Qt.PointingHandCursor
                }
            }
        }

        // Save button
        Button {
            id: saveButton
            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter

            background: Rectangle {
                color: "#1a1a1a"
                radius: 8
                border.color: saveButton.hovered ? "#98FF98" : "#333333"
                border.width: 1

                Behavior on border.color {
                    ColorAnimation {
                        duration: 150
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#98FF98"
                    radius: parent.radius
                    opacity: saveButton.hovered ? 0.15 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 150
                        }
                    }
                }
                Rectangle {
                    anchors.fill: parent
                    color: "#1a1a1a"
                    radius: parent.radius
                    opacity: saveButton.down ? 0.2 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }

            Image {
                source: "./assets/SVG/download.svg"
                anchors.centerIn: parent
                width: parent.width * 0.5
                height: parent.height * 0.5
                fillMode: Image.PreserveAspectFit
            }

            ToolTip.text: "Save data"
            ToolTip.visible: hovered
            ToolTip.delay: 500

            onClicked: timeline.saveRequested()

            HoverHandler {
                cursorShape: saveButton.hovered ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }

        // Load button
        Button {
            id: loadButton
            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter

            background: Rectangle {
                color: "#1a1a1a"
                radius: 8
                border.color: loadButton.hovered ? "#98FF98" : "#333333"
                border.width: 1

                Behavior on border.color {
                    ColorAnimation {
                        duration: 150
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#98FF98"
                    radius: parent.radius
                    opacity: loadButton.hovered ? 0.15 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 150
                        }
                    }
                }
                Rectangle {
                    anchors.fill: parent
                    color: "#1a1a1a"
                    radius: parent.radius
                    opacity: loadButton.down ? 0.2 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 100
                        }
                    }
                }
            }

            Image {
                source: "./assets/SVG/upload.svg"
                anchors.centerIn: parent
                width: parent.width * 0.5
                height: parent.height * 0.5
                fillMode: Image.PreserveAspectFit
            }

            ToolTip.text: "Load data"
            ToolTip.visible: hovered
            ToolTip.delay: 500

            onClicked: timeline.loadRequested()

            HoverHandler {
                cursorShape: loadButton.hovered ? Qt.PointingHandCursor : Qt.ArrowCursor
            }
        }
    }

    function formatTime(seconds) {
        var mins = Math.floor(seconds / 60);
        var secs = (seconds % 60).toFixed(1);
        return (mins > 0 ? mins + ":" : "") + (mins > 0 && secs < 10 ? "0" : "") + secs + "s";
    }
}
