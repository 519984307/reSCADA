import QtQuick 2.15
import QtQuick.Window 2.15

UnitPropItem {
    id: contItem
    width: 40
    height: width
    property alias regValve: regVal
    property alias valvePosition: mangWin.value
    property alias valveMaxRange: mangWin.valueMax
    property alias valveMinRange: mangWin.valueMin
    backgroundColor: "black"

    function setMinRange(value) {
        maxRange = Number(value)
    }
    function setMaxRange(value) {
        minRange  = Number(value)
    }
    function setValvePosition(value) {
        valvePosition = Number(value)
    }

    signal s_openLtl( variant OpenLtl )
    signal s_closeLtl( variant CloseLtl )
    signal s_valvePosChanged( variant ValvePos )
    onValvePositionChanged: s_valvePosChanged( valvePosition )

    Component.onCompleted: {
        regVal.position = valvePosition
    }
    RegulValve {
        id: regVal
        anchors.fill: parent
        borderColor: borderCurrentColor
        backgroundColor: backgroundCurrentColor
        nameText.text: name
        //position: mangWin.value
    }
    RegPersentWin {
        id: mangWin
        onValueChanged: regVal.position = value
        sepCorBtn: true
        readOnly: true
        onS_moreVal: s_openLtl( More )
        onS_lessVal: s_closeLtl( Less )
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

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/

