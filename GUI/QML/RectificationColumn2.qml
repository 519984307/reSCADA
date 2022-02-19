import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
RectificationColumn0 {
    id: root
    width: 70
    height: 450
    property alias pipeV4: pipeV4
    property int vWaterTopMarg: -200
    property int vSteamBottomMarg: -200

//    tWater.shTop: false
//    tWater.shBottom: true
//    tWater.shLeft: true
//    tWater.shRight: false

    Rectangle {
        id: rectTop
        x: 27
        width: parent.width / 4
        border.width: parent.pipePassBorderWidth
        anchors.top: cl3.verticalCenter
        anchors.bottom: parent.top
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
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
    }

    Rectangle {
        id: cl1
        width: parent.colderDiametr
        height: width
        color: backgroundColor
        radius: width / 2
        border.width: parent.pipePassBorderWidth
        anchors.bottom: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: width / 50
        z: 1
    }

    Tank {
        id: colder
        z: 10
        width: parent.width / 3
        height: width * 3
        radius: 5
        anchors.left: pipeV6.right
        anchors.bottom: parent.top
        showAlarmLevel: false
        anchors.leftMargin: parent.pipePassThin / 4
        nameText.text: ""
        showLevel: false
        mainGradientColor: backgroundColor
    }

    Pipe {
        id: pipe1
        y: -32
        height: parent.pipePassThin
        anchors.verticalCenter: cl1.verticalCenter
        anchors.left: cl1.right
        anchors.right: upAng.left
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
        anchors.left: parent.right
        anchors.top: pipe1.top
        anchors.leftMargin: 0
        anchors.topMargin: 0
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: parent.pipePassBorderWidth
        z: 0
    }



    Pipe {
        id: pipeV2
        x: 150
        width: parent.pipePassThin
        anchors.right: pipeAngBot5.right
        z: 50
        anchors.top: pipeAngBot5.bottom
        anchors.bottom: pipeAngBot6.top
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        anchors.rightMargin: 0
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
    }

    Pipe {
        id: pipeV3
        x: 109
        width: parent.pipePassThin
        anchors.top: colder.bottom
        anchors.bottom: ang180_1.top
        anchors.bottomMargin: 0
        anchors.horizontalCenter: colder.horizontalCenter
        anchors.topMargin: 0
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
    }

    Row {
        id: ang180_1
        height: width / 2
        anchors.right: pipeV3.right
        anchors.leftMargin: -parent.pipePassThin
    }

    PipeAngle90 {
        id: pipeAngBot5
        width: 10
        height: 10
        anchors.left: cl2.right
        anchors.top: pipe2.top
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: parent.pipePassBorderWidth
        anchors.leftMargin: -2
        z: 0
    }

    Pipe {
        id: pipeV4
        x: 134
        width: parent.pipePassThin
        anchors.top: cl2.bottom
        anchors.bottom: pipe3.bottom
        anchors.bottomMargin: -5
        anchors.horizontalCenter: cl2.horizontalCenter
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
        z: 0
    }
    Triangle{
        x: 91
        y: -28
        width: parent.pipePassThin * 2.5
        height: width
        anchors.bottom: pipeV5.top
        anchors.bottomMargin: -5
        anchors.horizontalCenter: pipeV5.horizontalCenter
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
    }
    Pipe {
        id: pipeV5
        x: 134
        width: parent.pipePassThin
        height: cl2.width / 4
        anchors.bottom: cl2.top
        nActiveColor: parent.pipePassColor
        anchors.horizontalCenter: cl2.horizontalCenter
        horOrVert: false
        z: 0
        borderWidth: parent.pipePassBorderWidth
    }

    Pipe {
        id: pipe3
        height: parent.pipePassThin
        anchors.left: parent.right
        anchors.right: pipeAngBot6.left
        anchors.top: parent.top
        anchors.topMargin: parent.tank.radius
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        nActiveColor: parent.pipePassColor
        z: 0
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
        anchors.horizontalCenter: parent.horizontalCenter
        z: 1
    }

    Pipe {
        id: pipe5
        y: -32
        height: parent.pipePassThin
        anchors.verticalCenter: cl3.verticalCenter
        anchors.left: cl3.right
        anchors.right: upAng10.left
        nActiveColor: parent.pipePassColor
        z: 0
        borderWidth: parent.pipePassBorderWidth
    }

    Pipe {
        id: pipeV6
        width: parent.pipePassThin
        anchors.left: pipeV7.right
        anchors.top: upAng10.bottom
        anchors.bottom: pipe3.top
        anchors.bottomMargin: -parent.pipePassBorderWidth
        anchors.leftMargin: parent.pipePassThin / 4
        nActiveColor: parent.pipePassColor
        anchors.rightMargin: -4
        horOrVert: false
        borderWidth: parent.pipePassBorderWidth
    }

    PipeAngle90 {
        id: upAng10
        x: 85
        width: 10
        height: 10
        anchors.right: pipeV6.right
        anchors.top: pipe5.top
        nActiveColor: parent.pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        z: 0
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
        borderWidth: parent.pipePassBorderWidth
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
        horOrVert: false
        nActiveColor: parent.pipeSteamColor
        z: 0
    }
    Rectangle {
        id: cl2
        width: colder.width
        height: width
        color: backgroundColor
        radius: width / 2
        border.width: parent.borderWidth / 2
        anchors.left: colder.right
        anchors.bottom: colder.bottom
        anchors.leftMargin: parent.pipePassThin
        z: 1
    }
    Pipe {
        id: pipe2
        height: parent.pipePassThin
        anchors.verticalCenter: cl2.verticalCenter
        anchors.left: colder.horizontalCenter
        anchors.right: cl2.left
        nActiveColor: parent.pipePassColor
        anchors.rightMargin: -cl2.border.width
        borderWidth: parent.pipePassBorderWidth
        z: 0
    }
    Pipe {
        id: pipeOutWtr4
        height: parent.pipeOutWaterThin
        anchors.left: upAng2.right
        anchors.right: pipeOutWtrAng2.left
        anchors.top: cl3.top
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
        rotation: 0
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
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
    }
    PipeAngle90 {
        id: pipeAngBot6
        x: 99
        y: 47
        width: 10
        height: 10
        anchors.right: pipeAngBot5.right
        anchors.bottom: pipe3.bottom
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        z: 0
        pipeThin: parent.pipePassThin
    }

    PipeAngle90 {
        id: upAng2
        width: 10
        height: 10
        anchors.left: cl3.right
        anchors.top: pipeOutWtr4.top
        anchors.leftMargin: -parent.colderDiametr / 4
        anchors.topMargin: 0
        rotation: 180
        nActiveColor: parent.pipeOutWaterColor
        borderWidth: parent.pipeBorderWidth
        pipeThin: parent.pipeOutWaterThin
    }
    Pipe {
        id: pipeV7
        x: 46
        width: parent.pipePassThin
        anchors.right: upAng.right
        anchors.top: upAng.bottom
        anchors.bottom: pipe3.top
        anchors.topMargin: 0
        anchors.bottomMargin: -parent.pipePassBorderWidth
        nActiveColor: parent.pipePassColor
        borderWidth: parent.pipePassBorderWidth
        horOrVert: false
        z: 100
    }

    PipeAngle180 {
        id: upAng4
        x: -10
        width: height *2
        anchors.right: parent.horizontalCenter
        //height: parent.colderDiametr
        anchors.top: cl3.bottom
        anchors.bottom: cl1.top
        anchors.rightMargin: 0
        anchors.topMargin:  -parent.colderDiametr /8
        anchors.bottomMargin: -parent.colderDiametr /8
        rotation: 90
        borderWidth: parent.pipeBorderWidth
        pipeThin: parent.pipeOutWaterThin
        nActiveColor: parent.pipeOutWaterColor
    }

    Pipe {
        id: pipeSteam2
        height: parent.pipeSteamThin
        anchors.left: pipeStAng3.right
        anchors.right: pipeSteam.left
        anchors.bottom: boler.bottom
        anchors.bottomMargin: boler.height * 0.3
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        anchors.leftMargin: 0
        z: 0
        anchors.rightMargin: -parent.borderWidth
    }

    PipeAngle90 {
        id: pipeStAng2
        width: 7
        height: 7
        anchors.right: pipeSteam1.right
        anchors.bottom: pipeSteam1.top
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        z: 0
        pipeThin: parent.pipeSteamThin
        rotation: 270
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
    }

    Pipe {
        id: pipeSteam1
        x: -32
        width: parent.pipeSteamThin
        anchors.top: pipeSt1.bottom
        anchors.bottom: pipeStAng3.top
        anchors.topMargin: boler.height / 6
        horOrVert: false
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        z: 0
        anchors.horizontalCenter: pipeSt1.horizontalCenter
        anchors.bottomMargin: 0
    }

    PipeAngle90 {
        id: pipeStAng3
        y: 378
        width: 7
        height: 7
        anchors.left: pipeSteam1.left
        anchors.bottom: pipeSteam2.bottom
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        anchors.leftMargin: 0
        z: 0
        pipeThin: parent.pipeSteamThin
        anchors.bottomMargin: 0
        rotation: 90
    }

    Pipe {
        id: pipeSteam3
        height: parent.pipeSteamThin
        anchors.left: boler.right
        anchors.right: pipeStAng2.left
        anchors.top: pipeStAng2.top
        anchors.topMargin: 0
        nActiveColor: parent.pipeSteamColor
        borderWidth: parent.pipeBorderWidth
        anchors.leftMargin: 0
        z: 0
        anchors.rightMargin: 0
    }

    PipeAngle180 {
        id: upAng11
        x: -14
        y: -16
        width: parent.width /2
        height: width /2
        anchors.bottom: parent.top
        z: 0
        nActiveColor: parent.pipeOutWaterColor
        anchors.bottomMargin: 1
        anchors.horizontalCenter: parent.horizontalCenter
        borderWidth: parent.pipeBorderWidth
        pipeThin: parent.pipeOutWaterThin
        anchors.horizontalCenterOffset: - parent.width /2
    }

    Pipe {
        id: pipeOutWtr81
        width: parent.pipeOutWaterThin
        visible: parent.fullView
        anchors.left: upAng11.left
        anchors.top: cl1.verticalCenter
        anchors.bottom: upAng11.top
        z: 0
        horOrVert: false
        nActiveColor: parent.pipeOutWaterColor
        anchors.bottomMargin: 0
        borderWidth: parent.pipeBorderWidth
        anchors.leftMargin: 0
        anchors.topMargin: 0
    }

    PipeAngle180 {
        id: upAng21
        x: -5
        y: -39
        width: 24
        height: width /2
        anchors.right: pipeOutWtr81.right
        anchors.bottom: pipeOutWtr81.top
        z: 0
        nActiveColor: parent.pipeOutWaterColor
        anchors.bottomMargin: 0
        rotation: 180
        anchors.rightMargin: 0
        borderWidth: parent.pipeBorderWidth
        pipeThin: parent.pipeOutWaterThin
    }

    Triangle {
        x: 94
        width: parent.pipeOutWaterThin * 2.5
        height: width
        anchors.top: pipeOutWtr91.bottom
        nActiveColor: parent.pipeOutWaterColor
        anchors.horizontalCenter: pipeOutWtr91.horizontalCenter
        borderWidth: parent.pipeBorderWidth
        anchors.topMargin: 0
        anchors.horizontalCenterOffset: 0
    }

    Pipe {
        id: pipeOutWtr91
        width: parent.pipeOutWaterThin
        visible: parent.fullView
        anchors.left: upAng21.left
        anchors.top: upAng21.bottom
        anchors.bottom: upAng11.top
        z: 0
        horOrVert: false
        nActiveColor: parent.pipeOutWaterColor
        rotation: 0
        anchors.bottomMargin: 0
        borderWidth: parent.pipeBorderWidth
        anchors.leftMargin: 0
        anchors.topMargin: 0
    }

    RegulValveUnit {
        id: regulValveSteam
        name: root.name + ".КП"
        width: 20
        height: 20
        anchors.bottom: parent.bottom
        regValve.substanceColor: pipeSteamColor
        z: 50
        regValve.position: 30
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
        x: 68
        width: 20
        height: 20
        anchors.top: parent.top
        regValve.substanceColor: pipeOutWaterColor
        z: 10
        regValve.position: 30
        objectName: root.objectName + ".vWater"
        anchors.topMargin: vWaterTopMarg
        regValve.nameOnTop: true
        anchors.horizontalCenter: cl2.horizontalCenter
        regValve.nameOnLeft: false
        regValve.nameTextPixSize: parent.valveNameSize
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.66}
}
##^##*/
