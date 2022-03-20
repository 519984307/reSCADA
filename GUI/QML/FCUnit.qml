import QtQuick 2.15
import "fap.js" as Fap

StartStopUnit {
    id: contItem
    width: 20
    height: width
    property alias perstWin: regPersWin
    property alias freq: regPersWin.value
    property alias reqMaxRange: regPersWin.valueMax
    property alias freqMinRange: regPersWin.valueMin

    function setMinRange(value) {
        regPersWin.setValueMinRange( value )
    }
    function setMaxRange(value) {
        regPersWin.setValueMaxRange( value )
    }
    function setFreq(value) {
        regPersWin.setValue(value)
    }
    signal s_moreLtl( variant MoreLtl )
    signal s_lessLtl( variant LessLtl )
    signal s_freqChanged( variant ValvePos )

    Rectangle {
        id: regVal
        anchors.fill: parent
        border.color: borderCurrentColor
        border.width: borderCurrentWidth
        color: backgroundCurrentColor
        Text{
            text: "ЧП"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            minimumPointSize: 3
            anchors.rightMargin: 1
            anchors.leftMargin: 1
            anchors.bottomMargin: 1
            anchors.topMargin: 1
            font.pointSize: 300
            fontSizeMode: Text.Fit
        }
    }
    RegPersentWin {
        id: regPersWin
        onValueChanged: freq = value
        title: name
        sepCorBtn: false
        readOnly: false
        onS_moreVal: s_moreLtl( More )
        onS_lessVal: s_lessLtl( Less )
        onS_valueChenged: s_freqChanged( Value )
        mainColor: "#09aad4"
        scaleColor: "#1a6b14"
        mfuCurValue.mantissa: 1
    }

    mouseArea.onClicked: {
        if (mouse.button & Qt.LeftButton) {
            regPersWin.show()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.1}
}
##^##*/

