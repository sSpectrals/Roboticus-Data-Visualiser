import QtQuick
import QtQuick.Controls
import QtQml.Models
import QtQuick.Controls.Material
import com.Roboticus.ControlCenter

Flickable {
    id: flickable

    width: parent.width / 2
    flickableDirection: Flickable.VerticalFlick
    contentWidth: parent.width / 2
    contentHeight: column.height
    clip: true
    boundsBehavior: Flickable.StopAtBounds

    required property var sensorController
    required property var vectorController

    property string selectedSensorLayer: sensorController.layers.length
                                         > 0 ? sensorController.layers[0] : "Layer 1"
    property string selectedVectorLayer: vectorController.layers.length
                                         > 0 ? vectorController.layers[0] : "Layer 1"

    ScrollBar.vertical: ScrollBar {
        id: verticalScrollBar
        policy: ScrollBar.AsNeeded
        width: 8
        visible: flickable.contentHeight > flickable.height

        contentItem: Rectangle {
            implicitWidth: 6
            radius: 3
            color: verticalScrollBar.pressed ? "#22FF98" : "#98FF98"
            opacity: verticalScrollBar.active ? 0.8 : 0.4
            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                }
            }
        }
    }

    SortFilterProxyModel {
        id: filteredSensorModel
        model: sensorController.model
        filters: [
            ValueFilter {
                roleName: "layer"
                value: flickable.selectedSensorLayer
            }
        ]
    }

    SortFilterProxyModel {
        id: filteredVectorModel
        model: vectorController.model
        filters: [
            ValueFilter {
                roleName: "layer"
                value: flickable.selectedVectorLayer
            }
        ]
    }

    Column {
        id: column
        width: parent.width
        spacing: 12

        property var selection: null

        ListView {
            id: sensorList
            width: parent.width
            height: contentHeight
            interactive: false
            model: filteredSensorModel
            spacing: 0
            clip: true

            onCountChanged: {
                if (count > 0) {
                    positionViewAtEnd()
                }
            }

            header: Rectangle {
                width: parent.width
                height: 50
                color: "transparent"

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 24

                    Text {
                        id: sensorsText
                        text: "Sensors"
                        width: 80
                        font.pixelSize: 20
                        font.weight: Font.DemiBold
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    ComboBox {
                        id: sensorLayerSelection
                        model: sensorController.layers
                        visible: sensorController.layers.length > 1
                        currentIndex: 0

                        anchors.verticalCenter: parent.verticalCenter

                        width: 140
                        height: 36

                        Material.accent: "#98FF98"
                        Material.foreground: "#98FF98"

                        background: Rectangle {
                            color: "#0f0f0f"
                            border.color: sensorLayerSelection.hovered ? "#98FF98" : "#333333"
                            border.width: 2
                            radius: 6

                            Behavior on border.color {
                                ColorAnimation {
                                    duration: 150
                                }
                            }
                        }

                        popup: Popup {
                            y: sensorLayerSelection.height
                            width: sensorLayerSelection.width
                            height: implicitHeight
                            padding: 3
                            contentItem: ListView {
                                clip: true
                                implicitHeight: contentHeight
                                model: sensorLayerSelection.popup.visible ? sensorLayerSelection.delegateModel : null
                                currentIndex: sensorLayerSelection.highlightedIndex

                                ScrollIndicator.vertical: ScrollIndicator {}
                            }

                            background: Rectangle {
                                color: "#1a1a1a"
                                border.color: "#98FF98"
                                border.width: 2
                                radius: 6
                            }
                        }

                        delegate: ItemDelegate {
                            width: sensorLayerSelection.width
                            hoverEnabled: true

                            contentItem: Text {
                                text: modelData
                                font.pixelSize: 16
                                color: parent.highlighted
                                       || parent.hovered ? "#98FF98" : "#888888"
                            }

                            highlighted: sensorLayerSelection.highlightedIndex === index

                            background: Rectangle {
                                color: parent.highlighted
                                       || parent.hovered ? "#0f0f0f" : "transparent"

                                Behavior on color {
                                    ColorAnimation {
                                        duration: 100
                                    }
                                }
                            }
                        }

                        onActivated: function (index) {
                            flickable.selectedSensorLayer = sensorLayerSelection.currentText
                            sensorController.setActiveLayer(
                                        sensorLayerSelection.currentText)
                            focus = false
                        }
                    }
                }
            }

            delegate: SensorInfo {
                id: sensorDelegate
                width: ListView.view.width
                height: 60

                color: index % 2 === 0 ? "#1a1a1a" : "#151515"

                sensorName: model.name
                sensorID: model.id
                inputValue: model.inputValue
                thresholdValue: model.threshold
                isTriggered: model.isTriggered
                xLocation: model.x // Dragging sensors won't update this value due to model not being updated, dragging will just be visual
                yLocation: model.y

                selected: column.selection === sensorDelegate

                onClicked: {
                    column.selection = column.selection === sensorDelegate ? null : sensorDelegate
                }

                onDeleteSensor: {
                    column.selection = null
                    sensorController.removeSensor(model.id)
                }
            }
        }

        ListView {
            id: vectorList
            width: parent.width
            height: contentHeight
            interactive: false
            model: filteredVectorModel
            spacing: 0
            clip: true

            onCountChanged: {
                if (count > 0) {
                    positionViewAtEnd()
                }
            }

            header: Rectangle {
                width: parent.width
                height: 50
                color: "transparent"

                Row {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 24

                    Text {
                        id: vectorsText
                        text: "Vectors"
                        width: 80
                        font.pixelSize: 20
                        font.weight: Font.DemiBold
                        color: "white"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    ComboBox {
                        id: vectorLayerSelection
                        model: vectorController.layers
                        visible: vectorController.layers.length > 1
                        currentIndex: 0

                        anchors.verticalCenter: parent.verticalCenter

                        width: 140
                        height: 36

                        Material.accent: "#98FF98"
                        Material.foreground: "#98FF98"

                        background: Rectangle {
                            color: "#0f0f0f"
                            border.color: vectorLayerSelection.hovered ? "#98FF98" : "#333333"
                            border.width: 2
                            radius: 6

                            Behavior on border.color {
                                ColorAnimation {
                                    duration: 150
                                }
                            }
                        }

                        popup: Popup {
                            y: vectorLayerSelection.height
                            width: vectorLayerSelection.width
                            height: implicitHeight
                            padding: 3
                            contentItem: ListView {
                                clip: true
                                implicitHeight: contentHeight
                                model: vectorLayerSelection.popup.visible ? vectorLayerSelection.delegateModel : null
                                currentIndex: vectorLayerSelection.highlightedIndex

                                ScrollIndicator.vertical: ScrollIndicator {}
                            }

                            background: Rectangle {
                                color: "#1a1a1a"
                                border.color: "#98FF98"
                                border.width: 2
                                radius: 6
                            }
                        }

                        delegate: ItemDelegate {
                            width: vectorLayerSelection.width
                            hoverEnabled: true

                            contentItem: Text {
                                text: modelData
                                font.pixelSize: 16
                                color: parent.highlighted
                                       || parent.hovered ? "#98FF98" : "#888888"
                            }

                            highlighted: vectorLayerSelection.highlightedIndex === index

                            background: Rectangle {
                                color: parent.highlighted
                                       || parent.hovered ? "#0f0f0f" : "transparent"

                                Behavior on color {
                                    ColorAnimation {
                                        duration: 100
                                    }
                                }
                            }
                        }

                        onActivated: function (index) {
                            flickable.selectedVectorLayer = vectorLayerSelection.currentText
                            vectorController.setActiveLayer(
                                        vectorLayerSelection.currentText)
                            focus = false
                        }
                    }
                }
            }

            delegate: VectorInfo {
                id: vectorDelegate
                width: ListView.view.width
                height: 60

                color: index % 2 === 0 ? "#1a1a1a" : "#151515"

                vectorName: model.name
                vectorID: model.id
                rotationValue: model.rotation
                vectorColor: model.color
                xLocation: model.x
                yLocation: model.y

                selected: column.selection === vectorDelegate

                onClicked: {
                    column.selection = column.selection === vectorDelegate ? null : vectorDelegate
                }

                onDeleteVector: {
                    column.selection = null
                    vectorController.removeVector(model.id)
                }
            }
        }
    }
}
