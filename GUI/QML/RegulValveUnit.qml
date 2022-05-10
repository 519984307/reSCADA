import QtQuick 2.15
import QtQuick.Window 2.15

UnitPropItem {
    id: contItem
    width: 40
    height: width
    property alias mFUnit: mFUnit
    property alias regValve: regVal
    property alias valvePosition: mangWin.value
    property alias valveMaxRange: mangWin.valueMax
    property alias valveMinRange: mangWin.valueMin
    property bool confmOnEnter: false
    backgroundColor: "black"

    function setMinRange(value) {
        mangWin.setValueMinRange( value )
    }
    function setMaxRange(value) {
        mangWin.setValueMaxRange( value )
    }
    function setValvePosition(value) {
        mangWin.setValue(value)
    }
    signal s_openLtl( variant OpenLtl )
    signal s_closeLtl( variant CloseLtl )
    signal s_valvePosChanged( variant ValvePos )

    Component.onCompleted: {
        regVal.position = valvePosition
    }
    RegulValve {
        id: regVal
        anchors.fill: parent
        borderColor: borderCurrentColor
        backgroundColor: backgroundCurrentColor
        borderWidth: borderCurrentWidth
        nameText.text: name
        //position: mangWin.value
    }
    RegPersentWin {
        id: mangWin
        onValueChanged: {
            regVal.position = value
            mFUnit.valueReal = value
        }
        sepCorBtn: true
        readOnly: true
        onS_moreVal: s_openLtl( More )
        onS_lessVal: s_closeLtl( Less )
        onS_valueChenged: s_valvePosChanged(Value)
        mfuCurValue.mantissa: 2
    }
    MFUnit{
        id: mFUnit
        width: 90
        height: 25
        anchors.top: parent.bottom
        anchors.topMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter
        backgroundColor: parent.backgroundColor
        textInput.color: regVal.substanceColor
        maxBtn.nameText.color: regVal.substanceColor
        minBtn.nameText.color: regVal.substanceColor
        disappear: true
        valueReal: mangWin.value
        upLimit: mangWin.valueMax
        downLimit: mangWin.valueMin
        mantissa: 2
        limited: true
        readOnly: true
        onS_more: s_openLtl( More )
        onS_less: s_closeLtl( Less )
        separCorrButtons: true
        confmOnEnter: parent.confmOnEnter
//        onValueChanged:{
//            mangWin.value  = Valve
//        }
    }

    MouseArea {
        anchors.fill: regVal
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if (mouse.button & Qt.RightButton) {
                openSettings()
            } else if (mouse.button & Qt.LeftButton) {
                mangWin.title = name
                mangWin.show()
            }
        }
    }
}



