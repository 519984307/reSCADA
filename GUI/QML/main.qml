import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
//import PromCore 1.0
import "fap.js" as Fap

//Test
Window {
    id: root
    width: 1450
    height: 800
    title: "reSCADA Мираторг-Кромы " + Fap.version
    objectName: "window"
    signal loaded();
    function checkLoaded(){
        root.visible = true
        root.loaded()
    }
    signal appClosed()
    signal stop();
    signal globalResetAlarm();
    signal showTags();
    signal test();

    onVisibleChanged: {
        if (!visible){
            root.appClosed()
        }
    }

//    Connections{
//        target: promCore
//        onS_loaded:{
//            dr2vis.promCoreLinc = promCore
//            dr2vis.dryer = dr2
//        }
//    }

    Rectangle{
        id: areaHead
        x: 0
        y: 0
        z: 10
        width: parent.width
        height: 40
        color: Fap.headBackground
        Row{
            id: buttons
            property int buttonWidth: 170
            height: parent.height
            width: parent.width
            spacing: 5

//            Button {
//                id: stopButton
//                height: parent.height
//                width: parent.buttonWidth
//                text: qsTr("СТОП")
//                //                style: ButtonStyle{
//                background: Rectangle{
//                    color: parent.pressed?"firebrick":"red"
//                    radius: 3
//                }
//                //                }
//                onClicked: root.stop()
//            }
            Button {
                id: alarmButton
                width: 250
                height: parent.height
                text: qsTr("Сбросить ошибки")
                font.bold: true
                font.pointSize: 15
                font.capitalization: Font.AllUppercase
                onClicked: root.globalResetAlarm()
            }
            Button {
                id: tagButton
                width: 200
                height: parent.height
                text: qsTr("Тэги")
                font.bold: true
                font.pointSize: 15
                font.capitalization: Font.AllUppercase
                //font.pixelSize: 12
                onClicked: root.showTags();
            }
            Button {
                id: testButton
                width: 200
                height: parent.height
                text: qsTr("Тест")
                font.bold: true
                font.pointSize: 15
                font.capitalization: Font.AllUppercase
                //font.pixelSize: 12
                onClicked: {
                    //promCore.moverAlarm()
                    root.test()
                }
            }
            Button {
                id: aboutButton
                width: 200
                height: parent.height
                text: qsTr("О программе")
                font.bold: true
                font.pointSize: 15
                font.capitalization: Font.AllUppercase
                //font.pixelSize: 12
                Dialog{
                    id: aboutDialog
                    Text{
                        text: "reSCADA " + Fap.version + "\n\nЯрослав Булгаков 89155399441\nНикита Плынский 89192697533"
                        anchors.centerIn: parent
                    }
                }
                onClicked: {
                    aboutDialog.visible = true
                }
            }
        }
    }
    Flickable  {
        id: sview
        anchors.top: areaHead.bottom
        anchors.bottom: log.top
        anchors.left: parent.left
        anchors.right: parent.right
        ScrollBar.horizontal: ScrollBar { id: hbar; active: vbar.active }
        ScrollBar.vertical: ScrollBar { id: vbar; active: hbar.active }
        clip: true
        focus: true
        Keys.onUpPressed: ScrollBar.decrease()
        Keys.onDownPressed: ScrollBar.increase()
        Item {
            //objectName: "element"
            id: element
            width: 1100
            height: 640
            anchors.fill: parent
            Rectangle {
                id: rectangle
                color: "#2b5876"
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: "#2b5876"
                    }

                    GradientStop {
                        position: 1
                        color: "#4e4376"
                    }
                }
            }
            Dryier3VSMTB {
                id: dr1
                numb: "1"
                //name: "Сушилка 1"
                x: 40
                y: 25
                levelReverse: true
                objectName: "dr1"
                z: 5
                humidity: false
                _EBberner: false
            }

            Dryier3VSMTB {
                id: dr2vis
                numb: "2"
                //name: "Сушилка 2"
                x: 705
                y: 25
                z: 5
                objectName: "dr2"
                //promCoreLinc: promCore
                //dryer: dr2
            }
        }
    }
    Log{
        id: log
        objectName: "log"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: 100
    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:0.33}D{i:3}D{i:4}D{i:5}D{i:7}D{i:6}D{i:2}D{i:1}D{i:13}D{i:17}
D{i:18}D{i:12}D{i:9}D{i:19}
}
##^##*/
