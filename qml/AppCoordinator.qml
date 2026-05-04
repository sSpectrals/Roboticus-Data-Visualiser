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

    function onTimelineFrameChanged(serialPortManager, serialParser, sensorPanel, timelineBar) {
        if (!serialPortManager.isConnected) {
            sensorPanel.clearGraph()
            serialParser.restoreToIndex(timelineBar.currentFrame)
        }
        timelineBar.updateTimelineProps()
    }

    function onSerialConnectionChanged(serialPortManager, sensorController, vectorController, monitor) {
        if (serialPortManager.isConnected) {
            sensorController.setActiveLayer(monitor.selectedSensorLayer)
            vectorController.setActiveLayer(monitor.selectedVectorLayer)
        }
    }
}
