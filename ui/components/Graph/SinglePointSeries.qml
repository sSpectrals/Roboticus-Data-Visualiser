import QtQuick
import QtGraphs
import QtQuick.Controls

// SinglePointSeries is required because a normal series doesn't keep metadata like input or threshold, furthermore it can't do declaritive triggers for the isTriggered Color.
// the CustomSeries type works but is not mature enough (too many bugs)
ScatterSeries {
    id: series

    property string sensorName: ""
    property double input: -1.0
    property double threshold: -1.0
    property bool isTriggered: true

    property double posX: 0
    property double posY: 0

    Component.onCompleted: {
        append(Qt.point(posX, posY))
    }

    pointDelegate: Rectangle {
        id: pointItem
        width: 15
        height: 15
        radius: width / 2
        color: isTriggered ? "red" : "lime"

        ToolTip {
            id: toolTip
            text: "Sensor: " + (sensorName || "Unknown") + "\n" +
            "Input: " + input + "\n" +
            "Threshold: " + threshold + "\n" +
            "Triggered: " + (isTriggered ? "Yes" : "No")
            visible: hoverHandler.hovered
            delay: 500
        }
        HoverHandler {
            id: hoverHandler
            cursorShape: Qt.PointingHandCursor
        }
    }

}
