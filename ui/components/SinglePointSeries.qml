import QtQuick
import QtGraphs

ScatterSeries {
    id: series

    property var sensorId: 0
    property real pointX: 0
    property real pointY: 0
    property bool isTriggered: true

    onPointXChanged: replace(0, pointX, pointY)
    onPointYChanged: replace(0, pointX, pointY)

    Component.onCompleted: {
        append(Qt.point(pointX, pointY))
    }

    pointDelegate: Rectangle {
        id: pointItem
        width: 15
        height: 15
        radius: width / 2
        color: isTriggered ? "red" : "lime"

        DragHandler {
            id: dragHandler
            target: pointItem

            // Model won't be updated due to it being overwritten by the next json serial anyways, so this is just visual and doesn't need an update
            onGrabChanged: (transition, point) => {
                               switch (transition) {
                                   case PointerDevice.GrabExclusive:
                                   break
                                   case PointerDevice.UngrabExclusive:
                                   let centerPoint = pointItem.mapToItem(
                                       chart, pointItem.width / 2,
                                       pointItem.height / 2)

                                   let newX = series.pixelToX(centerPoint.x)
                                   let newY = series.pixelToY(centerPoint.y)

                                   newX = Math.max(chart.axisX.min,
                                                   Math.min(newX,
                                                            chart.axisX.max))
                                   newY = Math.max(chart.axisY.min,
                                                   Math.min(newY,
                                                            chart.axisY.max))

                                   series.replace(0, newX, newY)

                                   break
                               }
                           }
        }

        HoverHandler {
            cursorShape: dragHandler.active ? Qt.ClosedHandCursor : Qt.OpenHandCursor
        }
    }

    function pixelToX(px) {
        let plotArea = chart.plotArea
        let axisX = chart.axisX

        let xRange = axisX.max - axisX.min

        return axisX.min + (px - plotArea.x) * xRange / plotArea.width
    }

    function pixelToY(py) {
        let plotArea = chart.plotArea
        let axisY = chart.axisY
        let yRange = axisY.max - axisY.min

        return axisY.max - (py - plotArea.y) * yRange / plotArea.height
    }
}
