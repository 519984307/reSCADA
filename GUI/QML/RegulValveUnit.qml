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

    //    onPositionChanged: {
    //        positionChanged(String(position))
    //        if (Number(mangWin.readValue) / 100 != position)
    //            mangWin.setValue(position * 100)
    //    }
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
        //        onPressAndHold: {
        //            setLvl( position + 0.05 ) test
        //        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/

