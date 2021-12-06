import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import LineComponent 1.0


Rectangle {
    id: rectCol
    width: 107
    height: 360
    radius: 20
    border.width: 1

    property alias name: name.text


    gradient: Gradient {
        orientation: Gradient.Horizontal
        GradientStop {
            position: 0
            color: "#d3d3d3"
        }

        GradientStop {
            position: 0.047
            color: "#c7c7c7"
        }

        GradientStop {
            position: 0.46981
            color: "#efefef"
        }

        GradientStop {
            position: 0.91171
            color: "#d3d3d3"
        }

        GradientStop {
            position: 0.97623
            color: "#bcbcbc"
        }
    }
    ProgressBar {
        id: progressBar
        x: -19
        y: 197
        width: 220
        height: 17
        clip: false
        rotation: 270
        layer.enabled: true
        value: 0.9
    }
    Text {
        id: name
        x: 8
        y: 54
        width: 66
        height: 62
        text: qsTr("name")
        wrapMode: Text.Wrap
        maximumLineCount: 2
        font.bold: true
        font.pointSize: 18
    }

    Rectangle {
        id: rectangle
        width: parent.width + parent.width*0.05
        height: parent.height * 0.01
        color: "#65379b"
        border.width: 1
        anchors.top: parent.top
        anchors.topMargin: parent.radius

        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter

        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                position: 0
                color: "#6c6c6c"
            }

            GradientStop {
                position: 0.61413
                color: "#bdbbc3"
            }

            GradientStop {
                position: 1
                color: "#6c6c6c"
            }
        }
    }

    Rectangle {
        id: rectangle2
        x: 0
        width: parent.width + parent.width*0.05
        height: parent.height * 0.01
        color: "#6c6c6c"
        border.width: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.radius
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#6c6c6c"
            }

            GradientStop {
                position: 0.61413
                color: "#bdbbc3"
            }

            GradientStop {
                position: 1
                color: "#6c6c6c"
            }
            orientation: Gradient.Horizontal
        }
        anchors.horizontalCenter: parent.horizontalCenter
    }
}







/*##^##
Designer {
    D{i:0;formeditorZoom:1.1;height:629;width:107}
}
##^##*/
