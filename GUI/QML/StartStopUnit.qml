import QtQuick 2.15
import "fap.js" as Fap

UnitPropItem {
    id: contItem
    width: 20
    height: width
    property alias mouseArea: mouseArea
    property color colorRun: Fap.run
    property color colorStopReady: Fap.ready
    property color colorManual: Fap.manual
    property color colorStartCommand: "Lime"
    property color colorStopCommand: "Lime"
    property bool st: false
    property bool std: false
    property bool manual: false

    signal s_start()
    signal s_stop()

    function started() {
        //startComand()
        std = true
        manual = false
        backgroundColor = colorRun
    }
    function stoped() {
        //stopComand()
        std = false
        manual = false
        backgroundColor = colorStopReady
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
        backgroundColor = colorManual
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if (mouse.button & Qt.RightButton) {
                openSettings()
            } else if (mouse.button & Qt.LeftButton) {
                notify = false
                alarmNotify = false
            }
        }
        onDoubleClicked: {
            if(st || std)
                s_stop()
            else
                s_start()
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/

