import QtQuick
import QtGraphs
import QtQuick.Effects

ScatterSeries {
    id: vectorArrow

    property var vectorId: 0
    property real vecX: 0
    property real vecY: 0
    property real arrowRotation: 0
    property real arrowScale: 1
    property color arrowColor: "white"

    Component.onCompleted: {
        append(Qt.point(vecX, vecY))
    }

    pointDelegate: Item {
        id: vectorItem
        width: chart.width * 0.1
        height: chart.height * 0.1
        scale: vectorArrow.arrowScale

        Image {
            id: arrowImage
            anchors.fill: parent
            source: "../../assets/SVG/arrow.svg"
            width: parent.width
            height: parent.height
            sourceSize: Qt.size(width * 2, height * 2)
            smooth: true
            visible: false
        }

        MultiEffect {
            anchors.fill: arrowImage
            source: arrowImage
            colorizationColor: vectorArrow.arrowColor
            rotation: vectorArrow.arrowRotation
            antialiasing: true
            colorization: 1

            DragHandler {
                id: dragHandler
                target: vectorItem

                onActiveChanged: {
                    if (!active) {
                        let centerPoint = vectorItem.mapToItem(
                                chart, vectorItem.width / 2,
                                vectorItem.height / 2)

                        let newX = vectorArrow.pixelToX(centerPoint.x)
                        let newY = vectorArrow.pixelToY(centerPoint.y)

                        vectorArrow.replace(0, newX, newY)
                    }
                }
            }

            HoverHandler {
                cursorShape: dragHandler.active ? Qt.ClosedHandCursor : Qt.OpenHandCursor
            }
        }
    }

    function pixelToX(px) {
        let plotArea = chart.plotArea
        let axisX = chart.axisX
        return axisX.min + (px - plotArea.x) * (axisX.max - axisX.min) / plotArea.width
    }

    function pixelToY(py) {
        let plotArea = chart.plotArea
        let axisY = chart.axisY
        return axisY.max - (py - plotArea.y) * (axisY.max - axisY.min) / plotArea.height
    }
}
