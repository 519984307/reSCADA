import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
RectificationColumn0 {
    id: root
    width: 70
    height: 520
    property alias cl1: cl1
    property int vWaterTopMarg: -200
    property int vSteamBottomMarg: -100

    //tWater.x: -250
    //tWater.y: -123//pipeOutWtr7.y + pipeOutWtr7.height

    Pipe {
        id: pipeV6
        width: parent.pipePassThin
        visible: fullView
        anchors.left: pipeV1.right
        anchors.top: upAng1.bottom
        anchors.bottom: pipeH2.top
        anchors.bottomMargin: - parent.pipePassBorderWidth
        anchors.leftMargin: parent.pipePassThin / 4
        z: 0
        nActiveColor: parent.pipePassColor
        horOrVert: false
        borderWidth: parent.pipePassBorderWidth
    }

    Pipe {
        id: pipeV4
        x: 134
        width: parent.pipePassThin
        visible: fullView
        anchors.top: cl2.bottom
        anchors.bottom: pipeH2.bottom
        anchors.bottomMargin: 35
        anchors.horizontalCenter: cl2.horizontalCenter
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
        z: 0
        anchors.topMargin: -1
    }
    Pipe {
        id: pipeH2
        y: 13
        height: parent.pipePassThin
        visible: fullView
        anchors.left: upAng5.right
        anchors.right: pipeAngBot5.left
        anchors.bottom: parent.top
        anchors.bottomMargin: -17
        nActiveColor: parent.pipePassColor
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        z: 0
        borderWidth: parent.pipePassBorderWidth
    }

    Rectangle {
        id: rectTop
        x: 27
        width: parent.width / 4
        color: "#d3d3d3"
        border.width: parent.pipePassBorderWidth
        anchors.top: cl4.verticalCenter
        anchors.bottom: parent.top
        anchors.topMargin: 0
        anchors.bottomMargin: - tank.borderWidth
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                position: 0
                color: "#d3d3d3"
            }

            GradientStop {
                position: 0.4073
                color: "#d3d3d3"
            }

            GradientStop {
                position: 0.54213
                color: "#ffffff"
            }

            GradientStop {
                position: 1
                color: "#d3d3d3"
            }
        }
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Rectangle {
        id: cl1
        width: parent.colderDiametr
        height: width
        color: backgroundColor
        radius: width / 2
        border.width: parent.pipePassBorderWidth
        anchors.bottom: parent.top
        anchors.bottomMargin: width / 50
        z: 1
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Tank {
        id: colder
        y: -70
        z: 10
        width: parent.width / 3
        height: parent.width
        radius: 5
        anchors.left: pipeV7.right
        anchors.bottom: parent.top
        anchors.bottomMargin: 0
        anchors.leftMargin: parent.pipePassThin / 4
        nameText.text: ""
        showLevel: false
        mainGradientColor: backgroundColor
        showAlarmLevel: false
    }

    Pipe {
        id: pipe1
        y: -32
        height: parent.pipePassThin
        visible: fullView
        anchors.verticalCenter: cl1.verticalCenter
        anchors.left: cl1.horizontalCenter
        anchors.right: upAng.left
        anchors.verticalCenterOffset: 26
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
        z: 0
    }



    PipeAngle90 {
        id: upAng
        x: 46
        width: 10
        height: 10
        visible: fullView
        anchors.right: pipeV1.right
        anchors.top: pipe1.top
        anchors.rightMargin: 0
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: parent.pipePassBorderWidth
        z: 0
    }


    Rectangle {
        id: cl2
        y: -22
        width: colder.width
        height: width
        visible: fullView
        color: backgroundColor
        radius: width / 2
        border.width: parent.borderWidth / 2
        anchors.left: colder.right
        anchors.bottom: colder.bottom
        anchors.leftMargin: 18
        anchors.bottomMargin: -1
        z: 1
    }


    Pipe {
        id: pipe2
        height: parent.pipePassThin
        visible: fullView
        anchors.verticalCenter: cl2.verticalCenter
        anchors.left: colder.horizontalCenter
        anchors.right: cl2.left
        anchors.leftMargin: 0
        anchors.rightMargin: -cl2.border.width
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        z: 0
    }



    PipeAngle90 {
        id: pipeAngBot5
        width: 10
        height: 10
        visible: fullView
        anchors.left: cl2.right
        anchors.top: pipe2.top
        anchors.topMargin: 0
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: parent.pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }


    Pipe {
        id: pipeV5
        x: 134
        width: parent.pipePassThin
        height: cl2.width / 4
        visible: fullView
        anchors.bottom: cl2.top
        nActiveColor: parent.pipePassColor
        anchors.horizontalCenter: cl2.horizontalCenter
        horOrVert: false
        z: 0
        anchors.bottomMargin: 0
        borderWidth: parent.pipePassBorderWidth
    }


    Rectangle {
        id: cl3
        width: parent.colderDiametr
        height: width
        color: backgroundColor
        radius: width / 2
        border.width: parent.pipePassBorderWidth
        anchors.bottom: cl1.top
        anchors.bottomMargin: width / 50
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        z: 1
    }

    Pipe {
        id: pipe5
        y: -32
        height: parent.pipePassThin
        visible: fullView
        anchors.verticalCenter: cl3.verticalCenter
        anchors.left: cl3.horizontalCenter
        anchors.right: upAng1.left
        anchors.verticalCenterOffset: 26
        nActiveColor: parent.pipePassColor
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        z: 0
        borderWidth: parent.pipePassBorderWidth
    }


    PipeAngle90 {
        id: upAng1
        x: 85
        width: 10
        height: 10
        visible: fullView
        anchors.right: pipeV6.right
        anchors.top: pipe5.top
        anchors.rightMargin: 0
        nActiveColor: parent.pipePassColor
        anchors.topMargin: 0
        pipeThin: parent.pipePassThin
        rotation: 270
        z: 0
        borderWidth: parent.pipePassBorderWidth
    }

    Rectangle {
        id: cl4
        y: -163
        width: parent.colderDiametr
        height: width
        color: backgroundColor
        radius: width / 2
        border.width: parent.pipePassBorderWidth
        anchors.bottom: cl3.top
        anchors.bottomMargin: width / 50
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 0
        z: 1
    }


    Pipe {
        id: pipe6
        y: -32
        height: parent.pipePassThin
        visible: fullView
        anchors.verticalCenter: cl4.verticalCenter
        anchors.left: cl4.horizontalCenter
        anchors.right: upAng2.left
        anchors.verticalCenterOffset: 26
        nActiveColor: parent.pipePassColor
        anchors.rightMargin: 0
        borderWidth: parent.pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    PipeAngle90 {
        id: upAng2
        x: 87
        width: 10
        height: 10
        visible: fullView
        anchors.right: pipeV7.right
        anchors.top: pipe6.top
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        anchors.rightMargin: 0
        borderWidth: parent.pipePassBorderWidth
        rotation: 270
        anchors.topMargin: 0
        z: 0
    }

    Pipe {
        id: pipeSt3
        width: parent.width / 10
        height: parent.pipeSteamThin
        anchors.left: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height / 15
        borderWidth: parent.pipePassBorderWidth
        anchors.leftMargin: 0
        nActiveColor: parent.pipeSteamColor
        z: 0
    }

    PipeAngle90 {
        id: pipeStAng1
        width: 7
        height: 7
        anchors.left: pipeSt3.right
        anchors.top: pipeSt3.top
        borderWidth: parent.pipePassBorderWidth
        anchors.leftMargin: 0
        anchors.topMargin: 0
        pipeThin: parent.pipeSteamThin
        nActiveColor: parent.pipeSteamColor
        rotation: 270
        z: 0
    }

    Pipe {
        id: pipeSteam
        width: parent.pipeSteamThin
        anchors.right: pipeStAng1.right
        anchors.top: pipeStAng1.bottom
        anchors.bottom: regulValveSteam.top
        anchors.bottomMargin: 0
        borderWidth: parent.pipePassBorderWidth
        anchors.topMargin: 0
        horOrVert: false
        anchors.rightMargin: 0
        nActiveColor: parent.pipeSteamColor
        z: 0
    }

    Pipe {
        id: pipeOutWtr4
        y: -126
        height: parent.pipeOutWaterThin
        anchors.left: pipeOutWtrAng3.right
        anchors.right: pipeOutWtrAng2.left
        anchors.bottom: cl4.top
        anchors.bottomMargin: parent.pipePassThin * 2
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        borderWidth: pipeBorderWidth
        nActiveColor: parent.pipeOutWaterColor
    }

    Pipe {
        id: pipeOutWtr3
        x: 75
        width: parent.pipeOutWaterThin
        anchors.top: regulValveWater.bottom
        anchors.bottom: pipeOutWtrAng1.top
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        horOrVert: false
        anchors.horizontalCenter: regulValveWater.horizontalCenter
        z: 0
        anchors.horizontalCenterOffset: 0
        borderWidth: pipeBorderWidth
        nActiveColor: parent.pipeOutWaterColor
    }

    PipeAngle180 {
        id: pipeOutWtrAng1
        x: 95
        y: -1
        width: 26
        height: 13
        anchors.right: pipeOutWtr3.right
        anchors.bottom: pipeOutWtr6.bottom
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        z: 0
        borderWidth: pipeBorderWidth
        rotation: 0
        pipeThin: parent.pipeSteamThin
        nActiveColor: parent.pipeOutWaterColor
    }

    PipeAngle90 {
        id: pipeOutWtrAng2
        width: 7
        height: 7
        anchors.right: pipeOutWtr5.right
        anchors.top: pipeOutWtr4.top
        anchors.rightMargin: 0
        pipeThin: parent.pipeSteamThin
        rotation: 270
        z: 0
        borderWidth: pipeBorderWidth
        nActiveColor: parent.pipeOutWaterColor
        anchors.topMargin: 0
    }

    Pipe {
        id: pipeOutWtr5
        x: 77
        width: parent.pipeOutWaterThin
        anchors.top: pipeOutWtrAng2.bottom
        anchors.bottom: colder.top
        anchors.topMargin: 0
        anchors.bottomMargin: 0
        anchors.horizontalCenter: colder.horizontalCenter
        z: 0
        borderWidth: pipeBorderWidth
        anchors.horizontalCenterOffset: 0
        nActiveColor: parent.pipeOutWaterColor
        horOrVert: false
    }

    Pipe {
        id: pipeV8
        x: 123
        width: parent.pipePassThin
        visible: fullView
        anchors.right: pipeAngBot5.right
        anchors.top: pipeAngBot5.bottom
        anchors.bottom: pipeAngBot6.top
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
        horOrVert: false
        z: 10
    }

    PipeAngle90 {
        id: pipeAngBot6
        y: 17
        width: 10
        height: 10
        visible: fullView
        anchors.left: cl2.right
        anchors.bottom: pipeH2.bottom
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        borderWidth: parent.pipePassBorderWidth
        pipeThin: parent.pipePassThin
        rotation: 0
        nActiveColor: parent.pipePassColor
        z: 0
    }
    Pipe {
        id: pipeV1
        z: 100
        width: parent.pipePassThin
        visible: fullView
        anchors.left: parent.right
        anchors.top: upAng.bottom
        anchors.bottom: upAng5.top
        anchors.topMargin: 0
        anchors.leftMargin: parent.pipePassThin / 4
        anchors.bottomMargin:  - parent.pipePassBorderWidth
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
        horOrVert: false
    }
    Pipe {
        id: pipeV7
        width: parent.pipePassThin
        visible: fullView
        anchors.left: pipeV6.right
        anchors.top: upAng2.bottom
        anchors.bottom: pipeH2.top
        anchors.bottomMargin: - parent.pipePassBorderWidth
        anchors.topMargin: 0
        anchors.leftMargin: parent.pipePassThin / 4
        z: 100
        nActiveColor: parent.pipePassColor
        horOrVert: false
        borderWidth: parent.pipePassBorderWidth
    }

    Triangle {
        x: 91
        y: -28
        width: parent.pipePassThin * 2.5
        height: width
        visible: fullView
        anchors.bottom: pipeV5.top
        anchors.horizontalCenter: pipeV5.horizontalCenter
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
        anchors.bottomMargin: -5
    }


    Pipe {
        id: pipe7
        y: -32
        height: parent.pipePassThin
        anchors.verticalCenter: cl1.verticalCenter
        anchors.left: cl1.right
        anchors.right: upAng.left
        nActiveColor: parent.pipePassColor
        anchors.rightMargin: 0
        borderWidth: parent.pipePassBorderWidth
        anchors.leftMargin: -3
        z: 0
    }

    PipeAngle90 {
        id: upAng5
        y: 30
        z: 0
        width: 10
        height: 10
        visible: fullView
        anchors.left: pipeV1.left
        anchors.bottom: pipeH2.bottom
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        rotation: 90
        pipeThin: parent.pipePassThin
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
    }

    PipeAngle90 {
        id: pipeOutWtrAng3
        width: 7
        height: 7
        anchors.left: parent.tWater.left
        anchors.top: pipeOutWtr4.top
        anchors.leftMargin: 32
        borderWidth: pipeBorderWidth
        z: 0
        anchors.topMargin: 0
        nActiveColor: parent.pipeOutWaterColor
        rotation: 180
        pipeThin: parent.pipeSteamThin
    }

    Pipe {
        id: pipeOutWtr7
        width: parent.pipeOutWaterThin
        height: 82
        anchors.left: pipeOutWtrAng3.left
        anchors.top: pipeOutWtrAng3.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 0
        horOrVert: false
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: pipeBorderWidth
        z: 0

    }
    Triangle{
        x: -57
        width: 10
        height: 10
        anchors.top: pipeOutWtr7.bottom
        anchors.horizontalCenter: pipeOutWtr7.horizontalCenter
        anchors.topMargin: 0
        borderWidth: 1
        nActiveColor: pipeOutWaterColor
    }

    RegulValveUnit {
        id: regulValveSteam
        name: root.name + ".КП"
        width: 20
        height: 20
        anchors.bottom: parent.bottom
        regValve.substanceColor: pipeSteamColor
        regValve.position: 30
        z: 50
        objectName: root.objectName + ".vSteam"
        anchors.bottomMargin: vSteamBottomMarg
        regValve.nameOnTop: true
        anchors.horizontalCenter: pipeSteam.horizontalCenter
        regValve.nameOnLeft: false
        regValve.nameTextPixSize: valveNameSize
    }

    RegulValveUnit {
        id: regulValveWater
        name: root.name + ".КВ"
        x: 109
        width: 20
        height: 20
        anchors.top: parent.top
        anchors.topMargin: vWaterTopMarg
        regValve.substanceColor: pipeOutWaterColor
        regValve.position: 30
        z: 10
        objectName: root.objectName + ".vWater"
        regValve.nameOnTop: true
        regValve.nameOnLeft: false
        regValve.nameTextPixSize: parent.valveNameSize
    }
}






/*##^##
Designer {
    D{i:0;formeditorZoom:2}
}
##^##*/
