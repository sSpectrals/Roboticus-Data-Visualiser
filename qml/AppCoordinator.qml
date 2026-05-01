import QtQuick

QtObject {
    function onSensorAdded(sensorPanel, monitor, id, layer, x, y, isTriggered) {
        if (layer.toLowerCase() === monitor.selectedSensorLayer.toLowerCase()) {
            sensorPanel.addPointToGraph(id, x, y, isTriggered)
        }
    }

    function onSensorUpdated(sensorPanel, monitor, id, layer, x, y, isTriggered) {
        if (layer.toLowerCase() === monitor.selectedSensorLayer.toLowerCase()) {
            sensorPanel.updatePointOnGraph(id, x, y, isTriggered)
        }
    }

    function onVectorAdded(sensorPanel, monitor, id, rotation, scale, color, layer, x, y) {
        if (layer.toLowerCase() === monitor.selectedVectorLayer.toLowerCase()) {
            sensorPanel.addArrowToGraph(id, rotation, scale, color, x, y)
        }
    }

    function onVectorUpdated(sensorPanel, monitor, id, rotation, scale, color, layer, x, y) {
        if (layer.toLowerCase() === monitor.selectedVectorLayer.toLowerCase()) {
            sensorPanel.updateArrowOnGraph(id, rotation, scale, color, x, y)
        }
    }

    function onTimelineFrameChanged(serialParser, sensorPanel, timelineBar) {
        if (!serialParser.isConnected) {
            sensorPanel.clearGraph()
            serialParser.restoreToIndex(timelineBar.currentFrame)
        }
        timelineBar.updateTimelineProps()
    }

    function onSerialConnectionChanged(serialParser, sensorController, vectorController, monitor) {
        if (serialParser.isConnected) {
            sensorController.setActiveLayer(monitor.selectedSensorLayer)
            vectorController.setActiveLayer(monitor.selectedVectorLayer)
        }
    }
}
