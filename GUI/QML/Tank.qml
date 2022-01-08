import QtQuick 2.15
import QtGraphicalEffects 1.15

Item {
    id: contItem
    property int radius: 30
    property real level: 0.8
    property real levelRatio: 0.8
    property alias nameText: nameText
    property alias nameTextPixSize: nameText.font.pixelSize
    property bool showSeam: true
    property bool showLevel: true
    property int borderWidth: 2

    property color borderColor: "black"
    property color mainGradientColor: "#d3d3d3"
    property color contentGradientColor: "steelblue"
    property int nameTopMargin: height * 0.4
    property int nameTextHeight: nameText.font.pixelSize
    width: 80
    height: 200

    Rectangle {
        id: rectBody
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
            property color colorMGrd2: Qt.hsla(
                                           mainGradientColor.hslHue,
                                           mainGradientColor.hslSaturation * 0.7,
                                           mainGradientColor.hslLightness * 1.2,
                                           mainGradientColor.a)
            property color colorMGrd3: Qt.hsla(
                                           mainGradientColor.hslHue,
                                           mainGradientColor.hslSaturation * 0.3,
                                           mainGradientColor.hslLightness * 1.7,
                                           mainGradientColor.a)
            Rectangle {
                id: mSrc
                anchors.fill: parent
                radius: rectColor.radius - rectBody.border.width
                gradient: Gradient {
                    id: mainGradient
                    orientation: Gradient.Horizontal
                    GradientStop {
                        position: 0
                        color: mainGradientColor
                    }
                    GradientStop {
                        position: 0.4
                        color: rectColor.colorMGrd2
                    }
                    GradientStop {
                        position: 0.6
                        color: rectColor.colorMGrd3
                    }
                    GradientStop {
                        position: 0.7
                        color: rectColor.colorMGrd2
                    }
                    GradientStop {
                        position: 0.9
                        color: mainGradientColor
                    }
                }
            }
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: mSrc
            }

            Rectangle {
                id: rectLevel
                width: parent.width
                height: parent.height * contItem.level * levelRatio
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                visible: showLevel
                property color colorGrd2: Qt.hsla(
                                              contentGradientColor.hslHue,
                                              contentGradientColor.hslSaturation * 0.7,
                                              contentGradientColor.hslLightness * 1.3,
                                              contentGradientColor.a)
                property color colorGrd3: Qt.hsla(
                                              contentGradientColor.hslHue,
                                              contentGradientColor.hslSaturation * 0.3,
                                              contentGradientColor.hslLightness * 1.5,
                                              contentGradientColor.a)

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
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: -2
        anchors.bottomMargin: parent.height * levelRatio
        visible: showLevel
    }
    Text {
        id: nameText
        height: nameTextHeight
        anchors.right: parent.right
        anchors.rightMargin: parent.width / 20
        anchors.left: parent.left
        anchors.leftMargin: parent.width / 20
        anchors.top: parent.top
        anchors.topMargin: nameTopMargin
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: qsTr("ЁМКОСТЬ")
        font.bold: true
        wrapMode: Text.WordWrap
        minimumPixelSize: 1
    }
    Text {
        id: levelDig
        text: qsTr(contItem.level * 100 + "%")
        anchors.bottom: seam2.top
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        maximumLineCount: 0
        fontSizeMode: Text.Fit
        visible: showLevel
    }
    Rectangle {
        id: seam2
        width: parent.width + parent.width * 0.05
        height: parent.height * 0.01
        visible: showSeam
        border.width: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: rectBody.radius
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
        width: parent.width + parent.width * 0.05
        height: parent.height * 0.01
        visible: showSeam
        border.width: 1
        anchors.top: parent.top
        anchors.topMargin: rectBody.radius

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
    D{i:0;formeditorZoom:0.9}
}
##^##*/

