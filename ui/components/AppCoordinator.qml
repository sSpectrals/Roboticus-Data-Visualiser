import QtQuick

QtObject {
    function onSensorAdded(sensorPanel, monitor, name, input, threshold, isTriggered, x, y, layer) {
        if (layer.toLowerCase() === monitor.selectedSensorLayer.toLowerCase()) {
            sensorPanel.addSensorToGraph(name, input, threshold, isTriggered, x, y)
        }
    }

    function onSensorUpdated(sensorPanel, monitor, name, input, threshold, isTriggered, x, y, layer) {
        if (layer.toLowerCase() === monitor.selectedSensorLayer.toLowerCase()) {
            sensorPanel.updateSensorOnGraph(name, input, threshold, isTriggered, x, y)
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

    function onTimelineFrameChanged(appController, sensorPanel, timelineBar) {
        sensorPanel.clearGraph()
        appController.restoreToIndex(timelineBar.currentFrame)
        timelineBar.updateTimelineProps()
    }

    function onSerialConnectionChanged(appController, sensorController, vectorController, monitor) {
        if (appController.portManager.isConnected) {
            sensorController.setActiveLayer(monitor.selectedSensorLayer)
            vectorController.setActiveLayer(monitor.selectedVectorLayer)
        }
    }
}
