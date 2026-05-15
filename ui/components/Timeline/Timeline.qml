import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import com.roboticus.DataVisualiser

Rectangle {
    id: timeline

    color: "#2b2a2a"
    radius: 12
    border.color: Qt.darker(color, 1.2)
    height: 80

    anchors {
        right: parent.right
        bottom: parent.bottom
        left: parent.left
        leftMargin: 20
        rightMargin: 20
        bottomMargin: 20
    }

    // Used to get the amount of snapshots taken
    required property var appController

    property int currentFrame: 0
    property int maxFrames: 0
    property real currentTime: 0.0
    property real maxTime: 60.0

    function updateTimelineProps() {
        maxFrames = appController.snapshotCount > 0 ? appController.snapshotCount - 1 : 0
        currentTime = appController.snapshotCount > 0 ? appController.timestampAt(currentFrame) / 1000.0 : 0.0
        maxTime = appController.snapshotCount > 0 ? appController.timestampAt(appController.snapshotCount - 1) / 1000.0 : 60.0
    }

    Component.onCompleted: updateTimelineProps()

    // back | front | progress bar | save | load
    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        // Back button
        StyledIconButton {
            enabled: timeline.currentFrame > 0
            onClicked: timeline.currentFrame--

            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter


            Image {
                anchors.centerIn: parent
                width: parent.width * 0.6
                height: parent.height * 0.6
                fillMode: Image.PreserveAspectFit
                source: parent.enabled ? "../../../assets/SVG/arrow_left_green.svg" : "../../../assets/SVG/arrow_left_grey.svg"
            }
        }

        // Forward button
        StyledIconButton {
            enabled: timeline.currentFrame < timeline.maxFrames
            onClicked: timeline.currentFrame++

            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter


            Image {
                anchors.centerIn: parent
                width: parent.width * 0.6
                height: parent.height * 0.6
                fillMode: Image.PreserveAspectFit
                source: parent.enabled ? "../../../assets/SVG/arrow_right_green.svg" : "../../../assets/SVG/arrow_right_grey.svg"
            }
        }

        // Progress bar
        TimelineProgressBar {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignVCenter
            currentFrame: timeline.currentFrame
            maxFrames: timeline.maxFrames
            currentTime: timeline.currentTime
            maxTime: timeline.maxTime
            onScrubbed: (frame) => timeline.currentFrame = frame
        }

        // Save button
        StyledIconButton {
            ToolTip.text: "Save data"
            ToolTip.visible: hovered
            ToolTip.delay: 500
            onClicked: saveFileDialog.open()

            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter


            Image {
                anchors.centerIn: parent
                width: parent.width * 0.5
                height: parent.height * 0.5
                fillMode: Image.PreserveAspectFit
                source: "../../../assets/SVG/download.svg"
            }

            FileDialog {
                id: saveFileDialog
                fileMode: FileDialog.SaveFile
                title: "Save Serial Data"
                nameFilters: ["JSON Files (*.json)"]
                defaultSuffix: "json"
                modality: Qt.ApplicationModal
                onAccepted: {
                    if (selectedFile)
                        appController.saveToFile(selectedFile)
                }
            }
        }

        // Load button
        StyledIconButton {
            ToolTip.text: "Load data"
            ToolTip.visible: hovered
            ToolTip.delay: 500
            onClicked: loadFileDialog.open()

            Layout.preferredWidth: 44
            Layout.preferredHeight: 44
            Layout.alignment: Qt.AlignVCenter


            Image {
                anchors.centerIn: parent
                width: parent.width * 0.5
                height: parent.height * 0.5
                fillMode: Image.PreserveAspectFit
                source: "../../../assets/SVG/upload.svg"
            }

            FileDialog {
                id: loadFileDialog
                fileMode: FileDialog.OpenFile
                flags: FileDialog.ReadOnly
                title: "Load Serial Data"
                nameFilters: ["JSON Files (*.json)"]
                modality: Qt.ApplicationModal
                onAccepted: {
                    if (selectedFile) {
                        appController.loadFromFile(selectedFile)
                        updateTimelineProps()
                    }
                }
            }
        }
    }
}