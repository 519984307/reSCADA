import QtQuick 2.15
//import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15
import "fap.js" as Fap

Rectangle {
    id: rectBody
    width: 100
    height: 50
    radius: 0
    property  alias mouseArea: mA
    property  alias nameText: nameText

    property bool checkable: false
    property bool checked: false
    property alias pressed: mA.pressed
    property color unPressCheckColor: Fap.buttonsBackground
    property color pressCheckColor: Qt.hsla(unPressCheckColor.hslHue,
                                            unPressCheckColor.hslSaturation,
                                            unPressCheckColor.hslLightness * 0.9,
                                            unPressCheckColor.a)
    border.color: Fap.buttonsBorder
    border.width: 1

    Component.onCompleted: renewColor()
    onCheckedChanged: renewColor()
    onCheckableChanged: renewColor()
    onPressCheckColorChanged: renewColor()
    onUnPressCheckColorChanged: renewColor()
    signal s_chackedUserChanged(variant Chacked)
    function renewColor(){
        if( checkable ){
            if( checked ) color = pressCheckColor
            else color = unPressCheckColor
        }
        else color = unPressCheckColor
    }

    Text {
        id: nameText
        text: qsTr("Button")
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        anchors.bottomMargin: Math.max(2, parent.height/ 30)
        anchors.topMargin: anchors.bottomMargin
        anchors.rightMargin: Math.max(parent.radius / 2, parent.width/ 20)
        anchors.leftMargin: Math.max(parent.radius / 2, parent.width/ 20)
        minimumPixelSize: 5
        font.pixelSize: 400
        fontSizeMode: Text.Fit
    }
    MouseArea{
        id: mA
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        hoverEnabled: true
        onEntered: border.width++
        onExited: border.width--
        onClicked: {
            if((mouse.button & Qt.LeftButton) && checkable) {
                checked = !checked
                s_chackedUserChanged(checked)
            }
        }
        onPressedChanged: {
            if( !checkable ){
                if( pressed )color = pressCheckColor
                else color = unPressCheckColor
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2}
}
##^##*/

