import QtQuick
import QtGraphs

Rectangle {
    id: sensorPanel
    width: parent.width / 2
    height: monitor.height
    color: "#1a1a1a"
    radius: 15
    border.width: 2
    border.color: "#98FF98"
    anchors {
        left: monitor.right
        top: parent.top
        right: parent.right
        margins: 20
    }

    GraphsView {
        id: chart
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 20
        }

        axisX: axisX
        axisY: axisY

        theme: GraphsTheme {
            labelBorderVisible: false
            grid {
                mainColor: "grey"
                mainWidth: 2
            }
            gridVisible: true
        }

        ValueAxis {
            id: axisX
            min: -20
            max: 20
            zoom: 1.0
        }

        ValueAxis {
            id: axisY
            min: axisX.min
            max: axisX.max
            zoom: axisX.zoom
        }

        // ! Bug: zooming out breaks screen pixel to chart point mapper (in SinglePointSeries.qml and VectorArrow.qml)

        // WheelHandler {
        //     target: chart
        //     acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

        //     onWheel: function (event) {
        //         let delta = event.angleDelta.y
        //         let zoomFactor = delta > 0 ? 1.1 : 0.9

        //         axisX.zoom *= zoomFactor

        //         axisX.zoom = Math.max(0.5, Math.min(4, axisX.zoom))
        //     }
        // }
    }

    property var pointSeriesMap: ({})
    property var arrowSeriesMap: ({})

    function addPointToGraph(id, x, y, trig) {
        var component = Qt.createComponent("SinglePointSeries.qml");
        if (component.status === Component.Ready) {
            var series = component.createObject(chart, {
                "sensorId": id,
                "pointX": x,
                "pointY": y,
                "isTriggered": trig
            });

            chart.addSeries(series);
            pointSeriesMap[id] = series;
        }
    }

    function removePointFromGraph(id) {
        var series = pointSeriesMap[id];
        if (series) {
            chart.removeSeries(series);

            delete pointSeriesMap[id];

            series.destroy(100);
        }
    }

    function updatePointOnGraph(id, x, y, trig) {
        var series = pointSeriesMap[id];
        if (series) {
            series.pointX = x;
            series.pointY = y;
            series.isTriggered = trig;
        }
    }

    function addArrowToGraph(id, rotation, scale, arrowColor, x, y) {
        var component = Qt.createComponent("VectorArrow.qml");
        if (component.status === Component.Ready) {
            var series = component.createObject(chart, {
                "vectorId": id,
                "vecX": x,
                "vecY": y,
                "arrowRotation": rotation,
                "arrowScale": scale,
                "arrowColor": arrowColor
            });

            chart.addSeries(series);
            arrowSeriesMap[id] = series;
        }
    }

    function removeArrowFromGraph(id) {
        var series = arrowSeriesMap[id];
        if (series) {
            chart.removeSeries(series);

            delete arrowSeriesMap[id];

            series.destroy(100);
        }
    }

    function updateArrowOnGraph(id, rotation, scale, arrowColor, x, y) {
        var series = arrowSeriesMap[id];
        if (series) {
            series.vecX = x;
            series.vecY = y;
            series.arrowRotation = rotation;
            series.arrowScale = scale;
            series.arrowColor = arrowColor;
        }
    }

    function clearPoints() {
        for (var id in pointSeriesMap) {
            var series = pointSeriesMap[id];
            if (series) {
                chart.removeSeries(series);
                series.destroy(100);
            }
        }
        pointSeriesMap = {};
    }

    function clearArrows() {
        for (var id in arrowSeriesMap) {
            var series = arrowSeriesMap[id];
            if (series) {
                chart.removeSeries(series);
                series.destroy(100);
            }
        }
        arrowSeriesMap = {};
    }

    function clearGraph() {
        clearPoints();
        clearArrows();
    }
}
