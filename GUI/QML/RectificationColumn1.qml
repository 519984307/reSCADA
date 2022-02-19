import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
RectificationColumn0 {
    id: root
    width: 70
    height: 350
    property int vWaterTopMarg: -180
    property int vSteamBottomMarg: -100
    tank.radius: 10
    //pipeOutWtr1.anchors.right: cl1.verticalCenter
    Rectangle {
        id: rectTop
        x: 27
        width: parent.width / 4
        border.width: parent.pipePassBorderWidth
        anchors.top: cl1.verticalCenter
        anchors.bottom: parent.top
        anchors.topMargin: 0
        anchors.bottomMargin: 0
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
        x: 11
        y: -55
        width: parent.colderDiametr
        height: width
        color: parent.backgroundColor
        radius: width / 2
        border.width: parent.pipePassBorderWidth
        anchors.bottom: parent.top
        anchors.bottomMargin: width / 50
        z: 1
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Tank {
        id: colder
        z: 10
        width: parent.width / 3
        height: width * 3
        radius: 5
        anchors.left: upAng.right
        anchors.bottom: parent.top
        showAlarmLevel: false
        anchors.leftMargin: pipeV1.width / 2
        nameText.text: ""
        showLevel: false
        mainGradientColor: parent.backgroundColor
    }

    Pipe {
        id: pipe1
        y: -32
        height: parent.pipePassThin
        visible: parent.fullView
        anchors.verticalCenter: cl1.verticalCenter
        anchors.left: cl1.right
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.leftMargin: -3
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
        z: 0
    }


    PipeAngle90 {
        id: upAng
        width: 10
        height: 10
        visible: parent.fullView
        anchors.left: pipe1.right
        anchors.top: pipe1.top
        anchors.leftMargin: 0
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: parent.pipePassBorderWidth
        z: 0
    }
    PipeAngle90 {
        id: ang90_1
        width: 10
        height: 10
        borderWidth: parent.pipePassBorderWidth
        pipeThin: parent.pipePassThin
        anchors.right: pipeV2.right
        anchors.bottom: pipe4.bottom
    }

    Pipe {
        id: pipeV2
        x: 150
        width: parent.pipePassThin
        visible: parent.fullView
        anchors.right: pipeAngBot5.right
        z: 50
        anchors.top: pipeAngBot5.bottom
        anchors.bottom: ang90_1.top
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
    }

    Pipe {
        id: pipeV3
        x: 109
        width: parent.pipePassThin
        visible: parent.fullView
        anchors.top: colder.bottom
        anchors.bottom: pipe4.top
        z: 5
        anchors.bottomMargin: - parent.pipePassBorderWidth
        anchors.horizontalCenter: colder.horizontalCenter
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
    }

    PipeAngle90 {
        id: pipeAngBot5
        width: 10
        height: 10
        visible: parent.fullView
        anchors.left: cl2.right
        anchors.top: cl2.verticalCenter
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: parent.pipePassBorderWidth
    }

    Pipe {
        id: pipeV4
        x: 134
        width: parent.pipePassThin
        visible: parent.fullView
        anchors.top: cl2.bottom
        anchors.bottom: ang90_1.top
        anchors.bottomMargin: 12
        anchors.horizontalCenter: cl2.horizontalCenter
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
        anchors.topMargin: -1
    }

    Pipe {
        id: pipeV5
        x: 134
        width: parent.pipePassThin
        height: cl2.width / 4
        visible: parent.fullView
        anchors.bottom: cl2.top
        z: 1
        nActiveColor: parent.pipePassColor
        anchors.horizontalCenter: cl2.horizontalCenter
        horOrVert: false
        borderWidth: parent.pipePassBorderWidth
    }

    Pipe {
        id: pipe4
        height: parent.pipePassThin
        visible: parent.fullView
        anchors.left: parent.right
        anchors.right: ang90_1.left
        anchors.top: parent.top
        anchors.topMargin: parent.tank.radius
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        z: 0
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
    }

    Tank {
        id: boler
        x: -41
        width: parent.width / 2
        height: 70
        anchors.right: pipeSt1.left
        anchors.top: pipeSt1.top
        anchors.topMargin: -radius
        showAlarmLevel: false
        z: 10
        radius: width /3
        showLevel: false
        showSeam: true
        nameText.text: ""
        mainGradientColor: parent.backgroundColor
    }
    Pipe {
        id: pipeSt1
        width: parent.width / 4
        height: parent.pipeSteamThin * 2
        anchors.right: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.tank.radius + 3
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipeSteamColor
        z: 0
    }

    Pipe {
        id: pipeSt3
        y: 322
        width: parent.width / 10
        height: parent.pipeSteamThin
        anchors.verticalCenter: pipeSt1.verticalCenter
        anchors.left: parent.right
        borderWidth: parent.pipeBorderWidth
        nActiveColor: parent.pipeSteamColor
        z: 0
    }

    PipeAngle90 {
        id: pipeStAng1
        width: 7
        height: 7
        anchors.left: pipeSt3.right
        anchors.top: pipeSt3.top
        pipeThin: parent.pipeSteamThin
        borderWidth: parent.pipeBorderWidth
        nActiveColor: parent.pipeSteamColor
        z: 0
        rotation: 270
    }

    Pipe {
        id: pipeSteam
        width: parent.pipeSteamThin
        height: 40
        anchors.right: pipeStAng1.right
        anchors.top: pipeStAng1.bottom
        anchors.bottom: regulValveSteam.top
        anchors.bottomMargin: 0
        borderWidth: parent.pipeBorderWidth
        nActiveColor: parent.pipeSteamColor
        z: 0
        horOrVert: false
    }

    Pipe {
        id: pipeSteam2
        height: parent.pipeSteamThin
        anchors.left: pipeStAng3.right
        anchors.right: pipeSteam.left
        anchors.bottom: boler.bottom
        anchors.bottomMargin: boler.height * 0.3
        anchors.leftMargin: 0
        anchors.rightMargin: -parent.borderWidth
        borderWidth: parent.pipeBorderWidth
        nActiveColor: parent.pipeSteamColor
        z: 0
    }

    RegulValveUnit {
        id: regulValveSteam
        name: root.name + ".КП"
        objectName: root.objectName + ".vSteam"
        z: 50
        width: 20
        height: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: vSteamBottomMarg
        anchors.horizontalCenter: pipeSteam.horizontalCenter
        regValve.nameTextPixSize: valveNameSize
        regValve.position: 30
        regValve.substanceColor: pipeSteamColor
        regValve.nameOnLeft: false
        regValve.nameOnTop: true
    }

    Rectangle {
        id: cl2
        width: colder.width
        height: width
        visible: parent.fullView
        color: parent.backgroundColor
        radius: width / 2
        border.width: parent.borderWidth / 2
        anchors.left: colder.right
        anchors.bottom: colder.bottom
        anchors.leftMargin: parent.pipePassThin * 2
        z: 1
    }

    Pipe {
        id: pipe5
        height: parent.pipePassThin
        visible: parent.fullView
        anchors.verticalCenter: cl2.verticalCenter
        anchors.left: colder.horizontalCenter
        anchors.right: cl2.left
        nActiveColor: parent.pipePassColor
        anchors.rightMargin: -cl2.border.width
        borderWidth: parent.pipePassBorderWidth
        z: 0
    }

    RegulValveUnit {
        id: regulValveWater
        name: root.name + ".КВ"
        x: 68
        width: 20
        height: 20
        anchors.top: parent.top
        z: 10
        anchors.topMargin: vWaterTopMarg
        regValve.position: 30
        regValve.substanceColor: pipeOutWaterColor
        objectName: root.objectName + ".vWater"
        regValve.nameOnTop: true
        anchors.horizontalCenter: cl2.horizontalCenter
        regValve.nameOnLeft: false
        regValve.nameTextPixSize: parent.valveNameSize
    }

    Pipe {
        id: pipeOutWtr4
        y: -77
        height: parent.pipeOutWaterThin
        visible: parent.fullView
        anchors.left: pipeOutWtrAng3.right
        anchors.right: pipeOutWtrAng2.left
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
    }

    Pipe {
        id: pipeOutWtr3
        x: 75
        width: parent.pipeOutWaterThin
        anchors.top: regulValveWater.bottom
        anchors.bottom: pipeOutWtrAng1.top
        nActiveColor: parent.pipeOutWaterColor
        anchors.horizontalCenter: regulValveWater.horizontalCenter
        horOrVert: false
        borderWidth: parent.pipeBorderWidth
        z: 0
    }

    PipeAngle90 {
        id: pipeOutWtrAng1
        x: 157
        y: -39
        width: 7
        height: 7
        anchors.right: pipeOutWtr3.right
        anchors.bottom: pipeOutWtr6.bottom
        nActiveColor: parent.pipeOutWaterColor
        pipeThin: parent.pipeOutWaterThin
        borderWidth: parent.pipeBorderWidth
        z: 0
    }

    PipeAngle90 {
        id: pipeOutWtrAng2
        width: 7
        height: 7
        anchors.right: pipeOutWtr5.right
        anchors.top: pipeOutWtr4.top
        nActiveColor: parent.pipeOutWaterColor
        pipeThin: parent.pipeOutWaterThin
        borderWidth: parent.pipeBorderWidth
        rotation: 270
        z: 0
    }

    Pipe {
        id: pipeOutWtr5
        x: 77
        width: parent.pipeOutWaterThin
        visible: parent.fullView
        anchors.top: pipeOutWtrAng2.bottom
        anchors.bottom: colder.top
        nActiveColor: parent.pipeOutWaterColor
        anchors.horizontalCenter: colder.horizontalCenter
        borderWidth: parent.pipeBorderWidth
        horOrVert: false
        z: 0
    }

    Pipe {
        id: pipeOutWtr6
        y: -35
        height: parent.pipeOutWaterThin
        anchors.verticalCenter: colder.verticalCenter
        anchors.left: colder.right
        anchors.right: pipeOutWtrAng1.left
        anchors.verticalCenterOffset: -5
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
    }

    Pipe {
        id: pipeOutWtr7
        x: 77
        width: parent.pipeOutWaterThin
        visible: parent.fullView
        anchors.top: pipeOutWtrAng2.bottom
        anchors.bottom: cl1.top
        nActiveColor: parent.pipeOutWaterColor
        anchors.horizontalCenter: parent.horizontalCenter
        borderWidth: parent.pipeBorderWidth
        horOrVert: false
        z: 0
    }

    PipeAngle90 {
        id: pipeOutWtrAng3
        width: 7
        height: 7
        visible: parent.fullView
        anchors.left: pipeOutWtr7.left
        anchors.top: pipeOutWtr4.top
        nActiveColor: parent.pipeOutWaterColor
        pipeThin: parent.pipeOutWaterThin
        borderWidth: parent.pipeBorderWidth
        rotation: 180
        z: 0
    }
    Pipe {
        id: pipeV1
        x: 90
        width: parent.pipePassThin
        visible: parent.fullView
        anchors.right: upAng.right
        anchors.top: pipe1.top
        anchors.bottom: pipe4.top
        anchors.topMargin: upAng.height
        anchors.bottomMargin: - parent.pipePassBorderWidth
        z: 100
        borderWidth: parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
        horOrVert: false
    }

    PipeAngle180 {
        id: upAng11
        x: -14
        y: -16
        width: parent.width /2
        height: width /2
        anchors.bottom: parent.top
        anchors.bottomMargin: 1

        anchors.horizontalCenterOffset: - parent.width /2
        anchors.horizontalCenter: parent.horizontalCenter
        nActiveColor: parent.pipeOutWaterColor
        pipeThin: parent.pipeOutWaterThin
        borderWidth: parent.pipeBorderWidth
        z: 0
    }

    Triangle {
        x: 91
        y: -28
        width: parent.pipePassThin * 2.5
        height: width
        anchors.bottom: pipeV5.top
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        anchors.horizontalCenter: pipeV5.horizontalCenter
        anchors.bottomMargin: -5
    }

    PipeAngle90 {
        id: pipeStAng2
        width: 7
        height: 7
        anchors.right: pipeSteam1.right
        anchors.bottom: pipeSteam1.top
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        z: 0
        pipeThin: parent.pipeSteamThin
        rotation: 270
    }

    Pipe {
        id: pipeSteam1
        x: -32
        width: parent.pipeSteamThin
        anchors.top: pipeSt1.bottom
        anchors.bottom: pipeStAng3.top
        anchors.topMargin: boler.height / 6
        anchors.horizontalCenter: pipeSt1.horizontalCenter
        horOrVert: false
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        z: 0
        anchors.bottomMargin: 0
    }

    PipeAngle90 {
        id: pipeStAng3
        y: 378
        width: 7
        height: 7
        anchors.left: pipeSteam1.left
        anchors.bottom: pipeSteam2.bottom
        anchors.leftMargin: 0
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        z: 0
        pipeThin: parent.pipeSteamThin
        rotation: 90
        anchors.bottomMargin: 0
    }

    Pipe {
        id: pipeSteam3
        height: parent.pipeSteamThin
        anchors.left: boler.right
        anchors.right: pipeStAng2.left
        anchors.top: pipeStAng2.top
        anchors.topMargin: 0
        anchors.rightMargin: 0
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    Pipe {
        id: pipeOutWtr81
        width: parent.pipeOutWaterThin
        visible: parent.fullView
        anchors.left: upAng11.left
        anchors.top: cl1.verticalCenter
        anchors.bottom: upAng11.top
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        horOrVert: false
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
        z: 0
    }

    PipeAngle180 {
        id: upAng21
        x: -5
        y: -39
        width: 24
        height: width /2
        anchors.right: pipeOutWtr81.right
        anchors.bottom: pipeOutWtr81.top
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        rotation: 180
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
        z: 0
        pipeThin: parent.pipeOutWaterThin
    }

    Triangle {
        x: 94
        width: parent.pipeOutWaterThin * 2.5
        height: width
        anchors.top: pipeOutWtr91.bottom
        anchors.topMargin: 0
        anchors.horizontalCenterOffset: 0
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
        anchors.horizontalCenter: pipeOutWtr91.horizontalCenter
    }

    Pipe {
        id: pipeOutWtr91
        width: parent.pipeOutWaterThin
        visible: parent.fullView
        anchors.left: upAng21.left
        anchors.top: upAng21.bottom
        anchors.bottom: upAng11.top
        rotation: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        horOrVert: false
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
        z: 0
        anchors.bottomMargin: 0
    }
//    tWater.shTop: false
//    tWater.shBottom: true
//    tWater.shLeft: true
//    tWater.shRight: false
}




/*##^##
Designer {
    D{i:0;formeditorZoom:2}D{i:24}
}
##^##*/
