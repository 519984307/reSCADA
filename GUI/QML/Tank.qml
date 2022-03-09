import QtQuick 2.15
import QtGraphicalEffects 1.15

Item {
    id: contItem
    width: 50
    height:200

    property alias levelText: levelDig
    property int levelTextBottMarg: radius + seam2.height
    property int levelTextHorOffset: 0
    property alias mfuAlarmTopLevel: mfuAlarmTopLevel
    property int radius: 10
    property real level: 80
    property real levelRatio: 0.8
    property int levelMantissa: 1
    property alias nameText: nameText
    property alias nameTextPixSize: nameText.font.pixelSize
    property int nameBottMarg: height * 0.3
    property int nameTextHorOffset: 0
    property bool showSeam: true
    property bool showLevel: true
    property int borderWidth: 2
    property bool showAlarmLevel: true
    property color borderColor: "black"
    property color mainGradientColor: "#d3d3d3"
    property color contentGradientColor: "steelblue"
    property alias alarmTopLevel: mfuAlarmTopLevel.valueReal
    property alias alarmBottomLevel: mfuAlarmBottomLevel.valueReal
    property string postfix:"%"
    property double volume:0

    signal s_alarmTopLevelChanged( variant AlarmTopLevel )
    signal s_alarmBottomLevelChanged( variant AlarmTopLevel )

    signal s_enableMouseArea( bool Enable)
    function setLevel( Level ) {
        level = Level.toFixed(levelMantissa)
    }
    function setAlarmLevelTop(value) { mfuAlarmTopLevel.setValue(value) }
    function setAlarmLevelBottom(value) {mfuAlarmBottomLevel.setValue(value) }
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
                height: {
                    if(contItem.volume > 0)
                        parent.height * contItem.level/contItem.volume * levelRatio
                    else
                        parent.height * contItem.level / 100 * levelRatio
                }
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
        id: capacity
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
        width:  parent.width* 0.85
        height: parent.width

        font.pixelSize: 300
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: "ЁМКОСТЬ"
        font.bold: true
        wrapMode: Text.WordWrap
        minimumPixelSize: 4
        fontSizeMode: Text.Fit
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: parent.nameBottMarg
        anchors.horizontalCenterOffset: parent.nameTextHorOffset

    }
    Text {
        id: levelDig
        text: "<b>" + contItem.level +"</b>" + postfix

        font.pixelSize: 300
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        minimumPixelSize: 5
        fontSizeMode: Text.Fit
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin:     parent.levelTextBottMarg
        anchors.horizontalCenterOffset: parent.levelTextHorOffset
        width:  parent.width* 0.85
        height: Math.max(parent.height* 0.1, parent.width* 0.3)
        visible: showLevel
    }
    Rectangle {
        id: seam2
        width: parent.width + parent.width * 0.05
        height: Math.max(parent.height * 0.01, 3)
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
        height: Math.max(parent.height * 0.01, 3)
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
    MFUnit {
        id: mfuAlarmTopLevel
        width: 54
        objectName: "alarmTopLevel"
        height: 20
        backgroundColor: "#f03e3e"
        tooltip: "Предельный верхний уровень"
        readOnly: false
        visible: showAlarmLevel
        anchors.top: capacity.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 0
        valueFontSize.bold: false
        disappear: showAlarmLevel
        correctingButtons: true
        onValueRealChanged: s_alarmTopLevelChanged(valueReal)
        limited: true
        downLimit: 20
        mouseArea.onContainsMouseChanged:{
            s_enableMouseArea( !mainRect.visible )
        }
        onValueChanged: s_alarmTopLevelChanged( Value )
    }
    MFUnit {
        id: mfuAlarmBottomLevel
        width: 54
        height: 20
        objectName: "alarmBottomLevel"
        backgroundColor: "#f03e3e"
        tooltip: "Предельный нижний уровень"
        readOnly: false
        visible: false//NOTE пока закрыл
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 0
        valueFontSize.bold: false
        disappear: false//NOTE пока закрыл
        correctingButtons: true
        onValueRealChanged: s_alarmBottomLevelChanged(valueReal)
        limited: true
        upLimit: mfuAlarmTopLevel.valueReal
        downLimit: 0
                mouseArea.onContainsMouseChanged:{
            s_enableMouseArea( !mainRect.visible )
        }
        onValueChanged:  s_alarmBottomLevelChanged( Value )
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:3}
}
##^##*/

