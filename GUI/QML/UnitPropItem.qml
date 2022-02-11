import QtQuick 2.15
import QtQuick.Controls 2.15
import "fap.js" as Fap

UnitItem {
    id: unitProp
    property alias propertyWindow: propWindow
    function openSettings() {
        if (propWindow.visible == false) {
            var absolutePos = mapToGlobal(0, 0)
            propWindow.x = absolutePos.x
            propWindow.y = absolutePos.y
            propWindow.show()
        }
        propWindow.requestActivate()
    }
    PropertyWindow3 {
        id: propWindow
        objectName: "propWindow"
        winTitle: name
        onResetAlarm: unitProp.s_resetAlarm()
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/

