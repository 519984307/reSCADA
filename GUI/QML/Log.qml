import QtQuick 2.12
import QtQuick.Controls 1.4

Item {
    property int counter: 0
    id: root
    function addMessage(date, time, unit, action){
        dataModel.append({"date": date, "time":time, "unit": unit, "action": action})
        view.positionViewAtRow(dataModel.count -1, ListView.Contain)
        if(dataModel.count > 200){
            dataModel.remove(dataModel.count -1, 1)
            //counter ++
        }
    }
    ListModel {
        id: dataModel
//        ListElement {
//            date: "13.12.14"
//            time: "12:46:16"
//            unit: "T33.1"
//            action: "Авария датчик подпора"
//        }
    }
    TableView {
        id: view
        selectionMode: SelectionMode.NoSelection
        anchors.fill: parent
        alternatingRowColors: false
        backgroundVisible: false
        model: dataModel
        clip: true

        TableViewColumn {
            width: 70
            title: "Дата"
            role: "date"
            resizable: true
            horizontalAlignment: Text.AlignHCenter
        }
        TableViewColumn {
            width: 70
            title: "Время"
            role: "time"
            resizable: true
            horizontalAlignment: Text.AlignHCenter
        }
        TableViewColumn {
            width: 300
            title: "Объект"
            role: "unit"
            resizable: true
            horizontalAlignment: Text.AlignHCenter
        }
        TableViewColumn {
            width: view.width-70*3
            title: "Событие"
            role: "action"
            resizable: true
            horizontalAlignment: Text.AlignHCenter
        }

        itemDelegate: Item {
            Text {
                anchors.left: parent.left
                renderType: Text.NativeRendering
                text: styleData.value
                color: "red"
            }
        }
    }
    Button{
        height: 20
        width: 60
        anchors.margins: 2
        anchors.right: parent.right
        anchors.top: parent.top
        text: "Очистить"
        tooltip: "Очистить журнал"
        onClicked: dataModel.clear()
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
