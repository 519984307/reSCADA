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
    property bool confmOnEnter: false
    property bool maxMinOnRMBCl: false
    property bool _usrAct: false

    signal s_maxLimitChanged( variant Limit )
    function setMaxLimit( Limit ){ maxLvl.setValue( Limit ) }

    signal s_minLimitChanged( variant Limit )
    function setMinLimit( Limit ){ minLvl.setValue( Limit ) }

    mouseArea.onClicked: {
        if ( mouse.button && Qt.RightButton && maxMinOnRMBCl) _usrAct = !_usrAct
    }
    function _onComExited(){
        if( _usrAct && !maxMinOnRMBCl)
            _usrAct = maxLvl.mouseArea.containsMouse || minLvl.mouseArea.containsMouse
    }
    mouseArea.onEntered: {
        if( !maxMinOnRMBCl ) {
            _usrAct = true
        }
    }
    mouseArea.onExited: _onComExited()

    on_UsrActChanged: {
        if( _usrAct ){
            oldZ = z
            z = 100
        }
        else z = oldZ
    }

    MFUnit {
        id: maxLvl
        enabled:  maxShow
        //property int oldZ: 0
        //property bool _usrAct: false
        width: parent.width * 1.2
        height: parent.height
        anchors.bottom: parent.top
        anchors.bottomMargin: -1
        anchors.horizontalCenter: parent.horizontalCenter
        backgroundColor: "#c36b6b"
        tooltip: "Макс."
        readOnly: false
        visible: parent._usrAct
        correctingButtons: true
        limited: false
        onValueChanged: s_maxLimitChanged( Value )
        mantissa: parent.mantissa
        disappear: !maxMinOnRMBCl
        confmOnEnter: parent.confmOnEnter
        mouseArea.onExited: {
            //_usrAct = false
            _onComExited()
        }
        mouseArea.onEntered: _usrAct = true
    }
    MFUnit {
        id: minLvl
        enabled: minShow
        //property int oldZ: 0
        //property bool _usrAct: false
        width: parent.width * 1.2
        height: parent.height
        anchors.horizontalCenter: parent.horizontalCenter
        backgroundColor: "#6b9ac3"
        tooltip: "Мин."
        readOnly: false
        visible: parent._usrAct
        anchors.top: parent.bottom
        anchors.topMargin: -2
        correctingButtons: true
        limited: false
        onValueChanged: s_minLimitChanged( Value )
        mantissa: parent.mantissa
        disappear: !maxMinOnRMBCl
        confmOnEnter: parent.confmOnEnter
        mouseArea.onExited: {
            //_usrAct = false
            _onComExited()
        }
        mouseArea.onEntered: _usrAct = true
    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:4}
}
##^##*/
