import QtQuick 2.15
import QtQuick.Window 2.15

UnitPropItem{
    id: contItem
    width: 40
    height: 40
    property alias level: regVal.level
    property alias regValve: regVal
    signal minRangeChanged  ( string value )
    signal maxRangeChanged  ( string value )
    signal valveLevelChanged( string value )

    function setMinRange ( value: string ) { mangWin.setMinRange( value ) }
    function setMaxRange ( value: string ) { mangWin.setMaxRange( value ) }
    function setLvl      ( value: string ) { level = Number( value ) }

    onLevelChanged: {
        valveLevelChanged( String( level ) )
        if( Number( mangWin.readValue ) / 100 != level )
            mangWin.setValue( level * 100 )
    }
    Component.onCompleted: {
       mangWin.setValue( level * 100 )
    }

    RegulValve{
        id: regVal
        anchors.fill: parent
        borderColor: borderCurrentColor
        backgroundColor: backgroundCurrentColor
        nameText.text: name
    }
    RegPersentWin{
        id: mangWin
        onMaxRangeChanged: maxRangeChanged( value )
        onMinRangeChanged: minRangeChanged( value )
        onValueChanged: {
            level = Number( value ) / 100
        }
    }

    MouseArea{
        anchors.fill: regVal
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: {
            if( mouse.button & Qt.RightButton){
                openSettings()
            }
            else if(mouse.button & Qt.LeftButton ){
                mangWin.show()
            }
        }
//        onPressAndHold: {
//            setLvl( level + 0.05 ) test
//        }
    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
