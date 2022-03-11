import QtQuick 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15
//import "fap.js" as Fap

Dialog {
    id: admDlg
    width: 200
    height: 100
    title: "Смена пользователя"

    TextInput {
        id: usr
        height: 20
        text: qsTr("Админ")
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 5
        anchors.rightMargin: 5
        anchors.leftMargin: 5
    }

    TextInput {
        id: pass
        height: 20
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: usr.bottom
        verticalAlignment: Text.AlignVCenter
        anchors.topMargin: 5
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        echoMode: TextInput.Password
        cursorVisible: true
    }
    Button {
        id: testButton
        x: 49
        width: 50
        height: 20
        text: "Вход"
        anchors.right: parent.right
        anchors.top: pass.bottom
        anchors.rightMargin: 5
        anchors.topMargin: 5
        font.capitalization: Font.AllUppercase
    }
}

/*##^##
Designer {
    D{i:0;height:55;width:56}D{i:1}D{i:2}D{i:3}
}
##^##*/
