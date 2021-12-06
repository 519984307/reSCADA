import QtQuick 2.15
import QtQuick.Window 2.12

UnitItem{
    id: contItem
    width: 40
    height: 40
    RegulValve{
        anchors.fill: parent
        borderColor: borderCurrentColor
        backgroundColor: backgroundCurrentColor
    }


}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/
