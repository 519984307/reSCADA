import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

AnalogSignalVar1 {
    id: analogSignalVar1
    width: 180
    height: 40
    property alias minLvl: minLvl
    property alias maxLvl: maxLvl
    property alias valueMinLvl: minLvl.valueReal
    property alias valueMaxLvl: maxLvl.valueReal
    property bool maxShow: true
    property bool minShow: true
    property int oldZ: 0

    Component.onCompleted: oldZ = z

    mouseArea.onClicked: {
        if ( mouse.button & Qt.RightButton) {
            maxLvl.visible = !maxLvl.visible
            minLvl.visible = !minLvl.visible
            if(maxLvl.visible){
                oldZ = z
                z = 100
            }
            else z = oldZ
        }
    }
    mouseArea.acceptedButtons: Qt.RightButton

    MFUnit {
        id: maxLvl
        height: parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.top
        anchors.bottomMargin: - parent.borderWidth
        backgroundColor: "#c36b6b"
        tooltip: "Max"
        readOnly: false
        visible: false
        correctingButtons: true
        checkLimit: false
    }
    MFUnit {
        id: minLvl
        height: parent.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.bottom
        anchors.topMargin: - parent.borderWidth
        backgroundColor: "#6e9ec8"
        tooltip: "Min"
        readOnly: false
        visible: false
        correctingButtons: true
        checkLimit: false
    }
}
/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
