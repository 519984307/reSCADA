import QtQuick 2.12

Rectangle{
    id: root
    width: 30
    height: 30
    border.color: "black"
    border.width: 1
    visible: state==""?false:true
    function activate(){
        state = "activated"
    }
    function deactivate(){
        state = "deactivated"
    }

    Rectangle{
        id: indicator
        property color coloritor: "white"

        anchors.margins: 2
        anchors.fill: parent
        radius: height / 2
        gradient: Gradient {
            GradientStop { position: 0.5; color: indicator.coloritor }
            GradientStop { position: 1; color: "white" }
        }
        border.color: "black"
        border.width: 1
    }
    states: [
        State {
            name: "activated"
            PropertyChanges { target: indicator; coloritor: "yellow" }
        },
        State {
            name: "deactivated"
            PropertyChanges { target: indicator; coloritor: "grey" }
        }
    ]
}

