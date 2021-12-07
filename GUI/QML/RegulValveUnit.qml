import QtQuick 2.15
import QtQuick.Window 2.15

UnitItem{
    id: contItem
    width: 40
    height: 40
    property alias regValve: regVal

    RegulValve{
        id: regVal
        anchors.fill: parent
        borderColor: borderCurrentColor
        backgroundColor: backgroundCurrentColor
        nameText.text: name
    }


}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
