import QtQuick 2.15
//import QtGraphicalEffects 1.15
import QtQuick.Controls 2.15

Rectangle {
    id: rectBody
    width: 100
    height: 50
    radius: 5
    property  alias mouseArea: mA
    property  alias nameText: nameText

    property bool checkable: true
    property bool checked: false
    property alias pressed: mA.pressed
    property color unPressCheckColor: "Gray"
    property color pressCheckColor: Qt.hsla(unPressCheckColor.hslHue,
                                            unPressCheckColor.hslSaturation,
                                            unPressCheckColor.hslLightness * 0.5,
                                            unPressCheckColor.a)
    border.color: "black"
    border.width: 1

    Component.onCompleted: renewColor()
    onCheckedChanged: renewColor()
    onCheckableChanged: renewColor()
    onPressCheckColorChanged: renewColor()
    onUnPressCheckColorChanged: renewColor()

    function renewColor(){
        if( checkable ){
            if( checked ) color = pressCheckColor
            else color = unPressCheckColor
        }
        else color = unPressCheckColor
    }

    Text {
        id: nameText
        text: qsTr("A")
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.WordWrap
        anchors.rightMargin: Math.max(radius / 2, parent.width/ 20)
        anchors.leftMargin: Math.max(radius / 2, parent.width/ 20)
        minimumPointSize: 5
        minimumPixelSize: 5
        font.pixelSize: 300
        font.bold: true
        fontSizeMode: Text.Fit
    }
    MouseArea{
        id: mA
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if((mouse.button & Qt.LeftButton) && checkable) checked = !checked
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

