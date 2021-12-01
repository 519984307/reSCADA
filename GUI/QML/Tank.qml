import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13
import LineComponent 1.0

Item{
    id: contItem
    property int radius: 20
    property real level: 0.8
    property real levelRatio: 0.2
    property alias nameText: nameText
    property bool showSeam: true
    property bool showLevel: true
    property int borderWidth: 2
    Rectangle {
        id: rectColumn
        border.width: borderWidth
        anchors.fill: parent
        radius: parent.radius
        Rectangle {
            id: rectColor
            radius: parent.radius - parent.border.width
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.rightMargin: parent.border.width
            anchors.leftMargin: parent.border.width
            anchors.topMargin: parent.border.width
            anchors.bottomMargin: parent.border.width

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

            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource:
                    Rectangle {
                    width: rectColor.width
                    height: rectColor.height
                    radius: rectColor.radius - rectColumn.border.width
                    anchors.bottom: rectColor.top
                }

            }
            Rectangle {
                id: capaciti
                width: parent.width / 2
                height: ((parent.height / 150) < 1 ? 4 : parent.height / 150 )
                color: "#501212"
                anchors.left: parent.right
                anchors.bottom: parent.bottom
                anchors.leftMargin: - width * 0.7
                anchors.bottomMargin: parent.height * levelRatio
            }
            Rectangle {
                id: rectLevel
                width: parent.width
                height: parent.height * contItem.level * levelRatio
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop {
                        position: 0
                        color: "#a65121"
                    }
                    GradientStop {
                        position: 0.3145
                        color: "#ca7b4e"
                    }
                    GradientStop {
                        position: 0.46981
                        color: "#d19a7b"
                    }
                    GradientStop {
                        position: 0.67584
                        color: "#ca7b4e"
                    }
                    GradientStop {
                        position: 0.97623
                        color: "#a65121"
                    }
                }


            }
        }
    }

    Text {
        id: nameText
        anchors.right: parent.right
        anchors.rightMargin: parent.width / 20
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 20
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("name")
        maximumLineCount: 2
        font.bold: true
        font.pointSize: 18
        wrapMode: Text.Wrap
        anchors.topMargin: parent.height / 2

    }
    Text {
        id: levelDig
        text: qsTr(contItem.level*100 + "%")
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: seam2.bottom
        anchors.bottom: parent.bottom
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        lineHeight: 1.1
        fontSizeMode: Text.VerticalFit
        anchors.leftMargin: rectColumn.radius
        anchors.rightMargin: rectColumn.radius
        anchors.topMargin: rectColumn.border.width
        anchors.bottomMargin: rectColumn.border.width
    }
    Rectangle {
        id: seam2
        width: parent.width + parent.width*0.05
        height: parent.height * 0.01
        visible: showSeam
        border.width: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: rectColumn.radius
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

    Rectangle {
        id: seam1
        width: parent.width + parent.width*0.05
        height: parent.height * 0.01
        visible: showSeam
        border.width: 1
        anchors.top: parent.top
        anchors.topMargin: rectColumn.radius

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
}


/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.9;height:629;width:107}D{i:19}
}
##^##*/
