import QtQuick 2.15
import QtQuick.Controls 2.15
import "fap.js" as Fap

UnitItem {
    id: unitPropItem
    property alias propertyWindow3: proWindow
    signal resetAlarm()
    function openSettings(){
        if(proWindow.visible==false){
            var absolutePos = mapToGlobal(0,0);
            proWindow.x = absolutePos.x
            proWindow.y = absolutePos.y
            proWindow.show()
        }
        proWindow.requestActivate()
    }
    PropertyWindow3 {
        id: proWindow
        objectName: parent.objectName + ".proWindow"
        winTitle: name
        onResetAlarm: parent.resetAlarm()
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
