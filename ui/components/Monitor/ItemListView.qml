import QtQuick
import QtQuick.Controls.Material

ListView {
    id: root

    property string title: ""
    property var layers: []
    property string selectedLayer: layers.length > 0 ? layers[0] : "Layer 1"

    signal layerChanged(string layer)

    width: parent.width
    height: contentHeight
    interactive: false
    spacing: 0
    clip: true

    onCountChanged: {
        if (count > 0)
            positionViewAtEnd()
    }

    header: Rectangle {
        width: parent.width
        height: 50
        color: "transparent"

        Row {
            anchors {
                left: parent.left
                leftMargin: 10
                verticalCenter: parent.verticalCenter
            }
            spacing: 24

            Text {
                text: root.title
                width: 80
                font.pixelSize: 20
                font.weight: Font.DemiBold
                color: "white"
                anchors.verticalCenter: parent.verticalCenter
            }

            StyledComboBox {
                model: root.layers
                visible: root.layers.length > 1
                currentIndex: 0
                width: 140
                height: 36
                anchors.verticalCenter: parent.verticalCenter

                onActivated: (index) => {
                    root.selectedLayer = currentText
                    root.layerChanged(currentText)
                    focus = false
                }
            }
        }
    }
}