import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

AnalogSignalVar1 {
    id: analogSignalVar1
    width: 60
    height: 15
    //property alias minLvl: minLvl
    property alias maxLvl: maxLvl
    property double limitStep: 1
    //property alias valueMinLvl: minLvl.valueReal
    property alias valueMaxLvl: maxLvl.valueReal
    property bool maxShow: true
    property bool minShow: true
    property int oldZ: 0

    signal s_maxLimitChanged( variant Limit )
    function setMaxLimit( Limit ){ maxLvl.setValue( Limit ) }

    Component.onCompleted: maxLvl.oldZ = z

    //    mouseArea.onClicked: {
    //        if ( mouse.button & Qt.RightButton) {
    //            maxLvl.visible = !maxLvl.visible
    //            //minLvl.visible = !minLvl.visible
    //            if(maxLvl.visible){
    //                oldZ = z
    //                z = 100
    //            }
    //            else z = oldZ
    //        }
    //    }

    MFUnit {
        id: maxLvl
        property int oldZ: 0
        width: parent.width * 1.2
        height: parent.height
        anchors.bottom: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: - parent.borderWidth
        backgroundColor: "#c36b6b"
        tooltip: "Max"
        readOnly: false
        visible: true
        correctingButtons: true
        limited: false
        onValueChanged: s_maxLimitChanged( Value )
        mantissa: parent.mantissa
//        step: limitStep
        disappear: true
        body.onVisibleChanged:{
            if(visible){
                oldZ = z
                parent.z = 100
            }
            else parent.z = oldZ
        }
    }
    //    MFUnit {
    //        id: minLvl
    //        height: parent.height
    //        anchors.left: parent.left
    //        anchors.right: parent.right
    //        anchors.top: parent.bottom
    //        anchors.topMargin: - parent.borderWidth
    //        backgroundColor: "#6e9ec8"
    //        tooltip: "Min"
    //        readOnly: false
    //        visible: false
    //        correctingButtons: true
    //        limited: false
    //    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
