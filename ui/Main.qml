import QtQuick
import "./components"
import QtQuick.Controls 2.15
import QtQuick.Controls.Material
import com.Roboticus.ControlCenter

//TODO: note to self, column.selectedSensor/onClicked is unused, possible to use for something else? idk
Window {
    id: window
    visibility: Window.Maximized
    minimumWidth: 950
    minimumHeight: 480
    title: qsTr("Roboticus Control Center")
    color: "#1a1a1a"

    Material.theme: Material.Dark
    Material.accent: "#98FF98"

    AppController {
        id: appController
        Component.onCompleted: setModels(sensorController.model, vectorController.model)


        onErrorOccurred: function (message) {
            errorPopup.show(message);
        }
    }

    SensorController {
        id: sensorController

        onSensorRemoved: function (id) {
            sensorController.setActiveLayer(monitor.selectedSensorLayer);
        }

        onClearChartSeries: function () {
            sensorPanel.clearSensorsOnGraph();
        }

        onErrorOccurred: function (message) {
            errorPopup.show(message);
        }
    }

    VectorController {
        id: vectorController

        onVectorRemoved: function (id) {
            vectorController.setActiveLayer(monitor.selectedVectorLayer);
        }

        onClearChartSeries: function () {
            sensorPanel.clearVectorsOnGraph();
        }

        onErrorOccurred: function (message) {
            errorPopup.show(message);
        }
    }


    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#1a1a1a"
            }
            GradientStop {
                position: 1.0
                color: "#0f0f0f"
            }
        }
        opacity: 0.8
    }

    SensorPanel {
        id: sensorPanel
        anchors.bottom: timelineBar.top
        anchors.bottomMargin: 10

        Connections {
            target: sensorController

            function onSensorAdded(id, name, input, threshold, isTriggered, layer, x, y) {
                if (layer.toLowerCase() === monitor.selectedSensorLayer.toLowerCase()) {
                    sensorPanel.addSensorToGraph(name, input, threshold, isTriggered, x, y)
                }
            }

            function onSensorUpdated(id, name, input, threshold, isTriggered, layer, x, y) {
                if (layer.toLowerCase() === monitor.selectedSensorLayer.toLowerCase()) {
                    sensorPanel.updateSensorOnGraph(name, input, threshold, isTriggered, x, y)
                }
            }

            function onClearChartSeries() {
                sensorPanel.clearSensorsOnGraph()
            }
        }

        Connections {
            target: vectorController

            function onVectorAdded(id, name, rotation, scale, color, layer, x, y) {
                if (layer.toLowerCase() === monitor.selectedVectorLayer.toLowerCase()) {
                    sensorPanel.addVectorToGraph(name, rotation, scale, color, x, y)
                }
            }

            function onVectorUpdated(id, name, rotation, scale, color, layer, x, y) {
                if (layer.toLowerCase() === monitor.selectedVectorLayer.toLowerCase()) {
                    sensorPanel.updateVectorOnGraph(name, rotation, scale, color, x, y)
                }
            }

            function onClearChartSeries() {
                sensorPanel.clearVectorsOnGraph()
            }
        }
    }

    TitleBar {
        id: title

        portManager: appController.portManager
    }

    MonitoringPanel {
        id: monitor

        anchors {
            left: parent.left
            top: title.bottom
            bottom: timelineBar.top
            bottomMargin: 20
            leftMargin: 20
            rightMargin: 20
        }

        sensorController: sensorController
        vectorController: vectorController
    }

    Timeline {
        id: timelineBar
        anchors {
            right: parent.right
            bottom: parent.bottom
            left: parent.left
            leftMargin: 20
            rightMargin: 20
            bottomMargin: 20
        }
        height: 80
        width: sensorPanel.width

        appController: appController

        onCurrentFrameChanged: {
            sensorPanel.clearGraph()
            appController.restoreToIndex(timelineBar.currentFrame)
            timelineBar.updateTimelineProps()
        }

        Connections {
            target: appController

            function onSnapshotsChanged() {
                timelineBar.updateTimelineProps();
            }
        }


        Connections {
            target: appController.portManager

            function onConnectionChanged() {
                if (appController.portManager.isConnected) {
                    sensorController.setActiveLayer(monitor.selectedSensorLayer)
                    vectorController.setActiveLayer(monitor.selectedVectorLayer)
                }
            }
        }
    }

    Error {
        id: errorPopup
    }

    // AddItem {
    //     id: addSensorButton
    //     anchors {
    //         left: parent.left
    //         bottom: parent.bottom
    //         leftMargin: 20
    //         bottomMargin: 20
    //     }
    //     height: 70
    //     width: (parent.width) / 2

    //     onAddSensorRequested: {
    //         sensorController.addSensor("Sensor name", 0, 100, false, "Layer 2", 0.0, 0.0);
    //     }
    //     onAddVectorRequested: vectorController.addVector("Vector name", 0.0, 1, "white", 1, 0.0, 0.0)
    // }
}
