import QtQuick
import QtGraphs
import QtQuick.Effects

ScatterSeries {
    id: vectorArrow

    property string vectorName: ""
    property real vectorRotation: 0
    property real vectorScale: 1
    property color vectorColor: "white"
    property real vecX: 0
    property real vecY: 0

    Component.onCompleted: {
        append(Qt.point(vecX, vecY))
    }

    pointDelegate: Item {
        id: vectorItem
        width: chart.width * 0.1 * vectorScale
        height: chart.height * 0.1 * vectorScale

        Image {
            id: vectorImage
            anchors.fill: parent
            source: "../../assets/SVG/arrow.svg"
            sourceSize: Qt.size(parent.width * 2, parent.height * 2)
            smooth: true
            visible: false
        }

        MultiEffect {
            anchors.fill: vectorImage
            source: vectorImage
            colorizationColor: vectorColor
            rotation: vectorRotation
            antialiasing: true
            colorization: 1
        }
    }
}
