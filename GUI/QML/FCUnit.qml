import QtQuick 2.15
import "fap.js" as Fap

UnitPropItem {
    id: contItem
    width: 20
    height: width
    property alias perstWin: regPersWin
    property alias rect: regVal
    property alias freq: regPersWin.value
    property alias reqMaxRange: regPersWin.valueMax
    property alias freqMinRange: regPersWin.valueMin

    function setMinRange(value) {
        reqMaxRange = Number(value)
    }
    function setMaxRange(value) {
        freqMinRange  = Number(value)
    }
    function setFreq(value) {
        freq = Number(value)
    }
    signal s_moreLtl( variant MoreLtl )
    signal s_lessLtl( variant LessLtl )
    signal s_freqChanged( variant ValvePos )
    onFreqChanged: s_freqChanged( freq )

    property bool st: false
    property bool std: false
    property bool manual: false

    function started() {
        startComand()
        std = true
        manual = false
        backgroundColor = "Green"
    }
    function stoped() {
        stopComand()
        std = false
        manual = false
        backgroundColor = Fap.ready
    }
    function startComand() {
        st = true
        manual = false
        backgroundColor = "Lime"
    }
    function stopComand() {
        st = false
        manual = false
        backgroundColor = "Lime"
    }
    function manualWork() {
        manual = true
        backgroundColor = "Gold"
    }

    Rectangle {
        id: regVal
        anchors.fill: parent
        border.color: borderCurrentColor
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
        mainColor: "#09aad4"
        scaleColor: "#1a6b14"
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if (mouse.button & Qt.RightButton) {
                openSettings()
            } else if (mouse.button & Qt.LeftButton) {
                regPersWin.show()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/

