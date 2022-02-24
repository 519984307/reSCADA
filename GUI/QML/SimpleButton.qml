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
    property bool checked: true
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
    onPressedChanged: renewColor()
    signal s_checkedUserChanged(variant Checked)
    signal s_on()
    signal s_off()
    function on(){
        checked = true
        color = pressCheckColor
    }
    function off(){
        checked = false
        color = unPressCheckColor
    }
    function renewColor(){
        if( checkable ){
            if( checked ) color = pressCheckColor
            else color = unPressCheckColor
        }
        else if( pressed )color = pressCheckColor
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
                //checked = !checked
                s_checkedUserChanged(!checked)
                if( checked )
                    s_on()
                else
                    s_off()
            }
        }
        onPressedChanged: {
            if( !checkable ){
                if(pressed)
                    s_on();
                else
                    s_off();
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2}
}
##^##*/

