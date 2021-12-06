import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13
import LineComponent 1.0

Item{
    id: contItem
    property int radius: 30
    property real level: 0.8
    property real levelRatio: 0.2
    property alias nameText: nameText
    property alias nameTextPixSize: nameText.font.pixelSize
    property bool showSeam: true
    property bool showLevel: true
    property int borderWidth: 2
    property color borderColor: "black"
    property color mainGradientColor: "#d3d3d3"
    property color contentGradientColor: "steelblue"

    width: 146
    height: 100

    Rectangle {
        id: rectColumn
        border.width: borderWidth
        border.color: borderColor
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
            property color colorGrd2: Qt.hsla(mainGradientColor.hslHue, mainGradientColor.hslSaturation * 0.7, mainGradientColor.hslLightness *1.2,  mainGradientColor.a)
            property color colorGrd3: Qt.hsla(mainGradientColor.hslHue, mainGradientColor.hslSaturation * 0.3, mainGradientColor.hslLightness *1.7,  mainGradientColor.a)


            gradient: Gradient {
                id: mainGradient
                orientation: Gradient.Horizontal
                GradientStop {
                    position: 0
                    color: mainGradientColor
                }
                GradientStop {
                    position: 0.4
                    color: rectColor.colorGrd2
                }
                GradientStop {
                    position: 0.6
                    color: rectColor.colorGrd3
                }
                GradientStop {
                    position: 0.7
                    color: rectColor.colorGrd2
                }
                GradientStop {
                    position: 0.9
                    color: mainGradientColor
                }
            }
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource:
                Rectangle {
                    width: rectColor.width
                    height: rectColor.height
                    radius: rectColor.radius - rectColumn.border.width
                    anchors.bottom: rectColor.bottom
                    anchors.bottomMargin: 0
                }
            }
            Rectangle {
                id: rectLevel
                width: parent.width
                height: parent.height * contItem.level * levelRatio
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                visible: showLevel
                property color colorGrd2: Qt.hsla(contentGradientColor.hslHue, contentGradientColor.hslSaturation * 0.7, contentGradientColor.hslLightness *1.3,  contentGradientColor.a)
                property color colorGrd3: Qt.hsla(contentGradientColor.hslHue, contentGradientColor.hslSaturation * 0.3, contentGradientColor.hslLightness *1.5,  contentGradientColor.a)

                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop {
                        position: 0
                        color: contentGradientColor
                    }
                    GradientStop {
                        position: 0.4
                        color: rectLevel.colorGrd2
                    }
                    GradientStop {
                        position: 0.6
                        color: rectLevel.colorGrd3
                    }
                    GradientStop {
                        position: 0.7
                        color: rectLevel.colorGrd2
                    }
                    GradientStop {
                        position: 0.9
                        color: contentGradientColor
                    }
                }


            }
        }
    }
    Rectangle {
        id: capaciti
        width: parent.width / 2
        height: 3
        color: "#501212"
        anchors.left: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: - width * 0.7
        anchors.bottomMargin: parent.height * levelRatio
        visible: showLevel
    }
    Text {
        id: nameText
        anchors.right: parent.right
        anchors.rightMargin: parent.width / 20
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 20
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: qsTr("Напорный бак")
        maximumLineCount: 1
        font.bold: true
        wrapMode: Text.NoWrap
        fontSizeMode: Text.HorizontalFit
        minimumPixelSize: 1
        anchors.topMargin: parent.height *0.4

    }
    Text {
        id: levelDig
        height: width * 0.5
        text: qsTr(contItem.level*100 + "%")
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        maximumLineCount: 0
        fontSizeMode: Text.Fit
        anchors.leftMargin: rectColumn.radius
        anchors.rightMargin: rectColumn.radius
        anchors.bottomMargin: rectColumn.border.width
        visible: showLevel
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



