import QtQuick
import QtGraphs

Rectangle {
    id: root
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
        bottom: timelineBar.top
        margins: 20
        bottomMargin: 10
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

        WheelHandler {
            target: chart
            acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchPad

            onWheel: function (event) {
                let delta = event.angleDelta.y
                let zoomFactor = delta > 0 ? 1.1 : 0.9

                axisX.zoom *= zoomFactor

                axisX.zoom = Math.max(0.5, Math.min(4, axisX.zoom))
            }
        }

    }

    property var sensorSeriesMap: ({})
    property var vectorSeriesMap: ({})

    function addSensorToGraph(name, input, threshold, trig, x, y) {
        var component = Qt.createComponent("SinglePointSeries.qml");
        if (component.status === Component.Ready) {
            var series = component.createObject(chart, {
                "sensorName": name,
                "input": input,
                "threshold": threshold,
                "isTriggered": trig,
                "posX": x,
                "posY": y
            });

            chart.addSeries(series);
            sensorSeriesMap[name] = series;
        }
    }

    function removeSensorFromGraph(name) {
        var series = sensorSeriesMap[name];
        if (series) {
            chart.removeSeries(series);

            delete sensorSeriesMap[name];

            series.destroy(100);
        }
    }

    function updateSensorOnGraph(name, input, threshold, trig, x, y) {
        var series = sensorSeriesMap[name];
        if (series) {
            series.input = input;
            series.threshold = threshold;
            series.posX = x;
            series.posY = y;
            series.isTriggered = trig;
        }
    }

    function addVectorToGraph(name, rotation, scale, arrowColor, x, y) {
        var component = Qt.createComponent("VectorArrow.qml");
        if (component.status === Component.Ready) {
            var series = component.createObject(chart, {
                "vectorName": name,
                "vectorRotation": rotation,
                "vectorScale": scale,
                "vectorColor": arrowColor,
                "vecX": x,
                "vecY": y
            });

            chart.addSeries(series);
            vectorSeriesMap[name] = series;
        }
    }

    function removeVectorFromGraph(name) {
        var series = vectorSeriesMap[name];
        if (series) {
            chart.removeSeries(series);

            delete vectorSeriesMap[name];

            series.destroy(100);
        }
    }

    function updateVectorOnGraph(name, rotation, scale, color, x, y) {
        var series = vectorSeriesMap[name];
        if (series) {
            series.vecX = x;
            series.vecY = y;
            series.vectorRotation = rotation;
            series.vectorScale = scale;
            series.vectorColor = color;
        }
    }

    function clearSensorsOnGraph() {
        for (var name in sensorSeriesMap) {
            var series = sensorSeriesMap[name];
            if (series) {
                chart.removeSeries(series);
                series.destroy(100);
            }
        }
        sensorSeriesMap = {};
    }

    function clearVectorsOnGraph() {
        for (var name in vectorSeriesMap) {
            var series = vectorSeriesMap[name];
            if (series) {
                chart.removeSeries(series);
                series.destroy(100);
            }
        }
        vectorSeriesMap = {};
    }

    function clearGraph() {
        clearSensorsOnGraph();
        clearVectorsOnGraph();
    }
}
