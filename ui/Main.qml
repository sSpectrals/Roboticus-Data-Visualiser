import QtQuick
import "./components"
import QtQuick.Controls 2.15
import QtQuick.Controls.Material
import com.Roboticus.ControlCenter

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


        onClearChartSeries: function () {
            sensorPanel.clearSensorsOnGraph();
        }

        onErrorOccurred: function (message) {
            errorPopup.show(message);
        }
    }

    VectorController {
        id: vectorController

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

        sensorController: sensorController
        vectorController: vectorController
    }

    Timeline {
        id: timelineBar


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

}
