import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
RectificationColumn0 {
    id: rc2
    width: 50
    height: 450

    pipeOutWtr1.anchors.verticalCenter: cl3.verticalCenter
    pipeOutWtr1.anchors.verticalCenterOffset: 13
    pipeOutWtr1.width: 65

    Rectangle {
        id: rectTop
        x: 27
        width: parent.width / 4
        border.width: 2
        anchors.top: cl3.verticalCenter
        anchors.bottom: parent.top
        anchors.topMargin: 0
        anchors.bottomMargin: -2
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
        width: parent.width * 0.7
        height: width
        color: backgroundCurrentColor
        radius: width / 2
        border.width: 2
        anchors.bottom: parent.top
        anchors.bottomMargin: width / 10
        z: 1
        anchors.horizontalCenter: parent.horizontalCenter
    }

    Tank {
        id: colder
        z: 10
        width: parent.width / 3
        height: width * 3
        radius: 10
        anchors.left: upAng1.right
        anchors.bottom: parent.top
        anchors.leftMargin: 2
        nameText.text: ""
        showLevel: false
    }

    PipeAngle {
        y: 30
        id: butAngl
        width: 7
        height: 7
        anchors.left: pAng1.right
        anchors.bottom: pAng1.bottom
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        borderWidth: pipePassBorderWidth
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        z: 0
    }

    Pipe {
        id: pipe1
        y: -32
        height: rc2.pipePassThin
        anchors.verticalCenter: cl1.verticalCenter
        anchors.left: cl1.right
        anchors.right: upAng.left
        anchors.rightMargin: 0
        anchors.leftMargin: -3
        borderWidth: pipePassBorderWidth
        nActivColor: pipePassColor
        z: 0
    }

    Pipe {
        id: pipeV1
        x: 90
        width: rc2.pipePassThin
        anchors.right: butAngl.right
        anchors.top: upAng.bottom
        anchors.bottom: butAngl.top
        anchors.bottomMargin: 0
        anchors.rightMargin: 0
        anchors.topMargin: 0
        borderWidth: pipePassBorderWidth
        nActivColor: pipePassColor
        horOrVert: false
    }

    PipeAngle {
        id: pAng1
        width: 7
        height: 7
        anchors.left: pAng2.right
        anchors.top: pAng2.bottom
        anchors.topMargin: 0
        anchors.leftMargin: -rc2.pipePassThin
        rotation: 90
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        borderWidth: pipePassBorderWidth
        z: 0
    }

    PipeAngle {
        id: pAng2
        y: 20
        width: 7
        height: 7
        anchors.left: parent.right
        anchors.leftMargin: 0
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        borderWidth: pipePassBorderWidth
        rotation: 270
        z: 0
    }

    PipeAngle {
        id: upAng
        x: 85

        width: 10
        height: 10
        anchors.right: butAngl.right
        anchors.top: pipe1.top
        anchors.rightMargin: 0
        anchors.topMargin: 0
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        z: 0
    }

    //    Rectangle {
    //        id: cl2
    //        width: colder.width * 0.7
    //        height: width
    //        color: backgroundCurrentColor
    //        radius: width / 2
    //        border.width: parent.borderWidth / 2
    //        anchors.verticalCenter: colder.verticalCenter
    //        anchors.left: colder.right
    //        anchors.leftMargin: pipeV1.width / 2
    //        z: 1
    //    }
    Row {
        id: ang180
        height: width / 2
        anchors.left: pipeAngBot3.right
        anchors.right: pipeV2.right
        anchors.top: pipeAngBot3.bottom
        anchors.leftMargin: -pipeAngBot3.pipeThin
        anchors.topMargin: 0
        anchors.rightMargin: 0
        PipeAngle {
            id: butAngl2
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            nActivColor: pipePassColor
            pipeThin: rc2.pipePassThin
            borderWidth: pipePassBorderWidth
            rotation: 90
            z: 0
        }
        PipeAngle {
            id: butAngl1
            width: height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: 0
            anchors.bottomMargin: 0
            anchors.rightMargin: 0
            nActivColor: pipePassColor
            pipeThin: rc2.pipePassThin
            borderWidth: pipePassBorderWidth
        }
    }
    PipeAngle {
        id: pipeAngBot3

        width: 7
        height: 7
        anchors.left: pipe4.right
        anchors.top: pipe4.top
        anchors.topMargin: 0
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    Pipe {
        id: pipeV2
        x: 150
        width: rc2.pipePassThin
        anchors.right: pipeAngBot5.right
        z: 50
        anchors.top: pipeAngBot5.bottom
        anchors.bottom: ang180.top
        anchors.topMargin: 0
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        nActivColor: pipePassColor
        borderWidth: pipePassBorderWidth
        horOrVert: false
    }

    Pipe {
        id: pipeV3
        x: 109
        width: rc2.pipePassThin
        anchors.top: colder.bottom
        anchors.bottom: ang180_1.top
        anchors.bottomMargin: 0
        anchors.horizontalCenter: colder.horizontalCenter
        anchors.topMargin: 0
        nActivColor: pipePassColor
        borderWidth: pipePassBorderWidth
        horOrVert: false
    }

    Row {
        id: ang180_1
        height: width / 2
        anchors.left: pipeAngBot4.right
        anchors.right: pipeV3.right
        anchors.top: pipeAngBot4.bottom
        anchors.leftMargin: -rc2.pipePassThin
        anchors.rightMargin: 0
        anchors.topMargin: 0
        PipeAngle {
            id: butAngl3
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            pipeThin: rc2.pipePassThin
            borderWidth: pipePassBorderWidth
            rotation: 90
            anchors.leftMargin: 0
            z: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0
        }

        PipeAngle {
            id: butAngl4
            width: height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            pipeThin: rc2.pipePassThin
            anchors.rightMargin: 0
            borderWidth: pipePassBorderWidth
            anchors.bottomMargin: 0
            anchors.topMargin: 0
        }
    }

    PipeAngle {
        id: pipeAngBot4
        width: 7
        height: 7
        anchors.left: pipe3.right
        anchors.top: pipe3.top
        anchors.topMargin: 0
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        anchors.leftMargin: 0
        rotation: 270
        borderWidth: pipePassBorderWidth
        z: 0
    }

    PipeAngle {
        id: pipeAngBot5
        width: 10
        height: 10
        anchors.left: cl2.right
        anchors.top: pipe2.top
        anchors.topMargin: 0
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: -2
        z: 0
    }

    Pipe {
        id: pipeV4
        x: 134
        width: rc2.pipePassThin
        anchors.top: cl2.bottom
        anchors.bottom: ang180.bottom
        anchors.bottomMargin: -rc2.pipePassThin * 1

        anchors.horizontalCenter: cl2.horizontalCenter
        nActivColor: pipePassColor
        borderWidth: pipePassBorderWidth
        horOrVert: false
        z: 0
        anchors.topMargin: -1
    }

    Pipe {
        id: pipeV5
        x: 134
        width: rc2.pipePassThin
        height: cl2.width / 4
        anchors.bottom: cl2.top
        nActivColor: pipePassColor
        anchors.horizontalCenter: cl2.horizontalCenter
        horOrVert: false
        z: 0
        anchors.bottomMargin: 0
        borderWidth: pipePassBorderWidth
    }

    Pipe {
        id: pipe3
        y: 53
        width: 15
        height: rc2.pipePassThin
        anchors.left: parent.right
        anchors.leftMargin: 0
        nActivColor: pipePassColor
        z: 0
        borderWidth: pipePassBorderWidth
    }

    Pipe {
        id: pipe4
        y: 73
        height: rc2.pipePassThin
        anchors.left: parent.right
        anchors.right: pipeV3.right
        nActivColor: pipePassColor
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        z: 0
        borderWidth: pipePassBorderWidth
    }

    Rectangle {
        id: cl3
        width: parent.width * 0.7
        height: width
        color: backgroundCurrentColor
        radius: width / 2
        border.width: 2
        anchors.bottom: cl1.top
        anchors.bottomMargin: width / 10
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        z: 1
    }

    Pipe {
        id: pipe5
        y: -32
        height: rc2.pipePassThin
        anchors.verticalCenter: cl3.verticalCenter
        anchors.left: cl3.right
        anchors.right: upAng1.left
        nActivColor: pipePassColor
        anchors.rightMargin: 0
        anchors.leftMargin: -3
        z: 0
        borderWidth: pipePassBorderWidth
    }

    Pipe {
        id: pipeV6
        x: 89
        width: rc2.pipePassThin
        anchors.right: butAngl.right
        anchors.top: upAng1.bottom
        anchors.bottom: ang180_2.top
        nActivColor: pipePassColor
        anchors.topMargin: 0
        anchors.rightMargin: -4
        horOrVert: false
        anchors.bottomMargin: 0
        borderWidth: pipePassBorderWidth
    }

    PipeAngle {
        id: pAng4
        y: 33
        width: 7
        height: 7
        anchors.left: parent.right
        nActivColor: pipePassColor
        pipeThin: rc2.pipePassThin
        anchors.leftMargin: 0
        rotation: 270
        z: 0
        borderWidth: pipePassBorderWidth
    }

    PipeAngle {
        id: upAng1
        x: 85
        width: 10
        height: 10
        anchors.right: pipeV6.right
        anchors.top: pipe5.top
        anchors.rightMargin: 0
        nActivColor: pipePassColor
        anchors.topMargin: 0
        pipeThin: rc2.pipePassThin
        rotation: 270
        z: 0
        borderWidth: pipePassBorderWidth
    }

    Row {
        id: ang180_2
        height: width / 2
        anchors.left: pAng4.right
        anchors.right: pipeV6.right
        anchors.top: pAng4.bottom
        anchors.leftMargin: -rc2.pipePassThin
        anchors.topMargin: 0
        anchors.rightMargin: 0
        PipeAngle {
            id: butAngl5
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            anchors.topMargin: 0
            pipeThin: rc2.pipePassThin
            anchors.leftMargin: 0
            rotation: 90
            z: 0
            anchors.bottomMargin: 0
            borderWidth: pipePassBorderWidth
        }

        PipeAngle {
            id: butAngl6
            width: height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            anchors.topMargin: 0
            anchors.rightMargin: 0
            pipeThin: rc2.pipePassThin
            anchors.bottomMargin: 0
            borderWidth: pipePassBorderWidth
        }
    }

    Tank {
        id: boler
        x: -41
        y: 304
        width: parent.width / 3
        height: width * 3
        radius: 10
        anchors.right: pipeSt1.left
        anchors.bottom: parent.bottom
        nameText.text: ""
        showLevel: false
        anchors.bottomMargin: -parent.width / 3
        anchors.rightMargin: 0
        z: 10
    }

    Pipe {
        id: pipeSt1
        width: parent.width / 10
        height: rc2.pipeSteamThin
        anchors.right: parent.left
        anchors.top: boler.top
        borderWidth: pipePassBorderWidth
        anchors.topMargin: boler.height / 6
        anchors.rightMargin: 0
        nActivColor: rc2.pipeSteamColor
        z: 0
    }

    Pipe {
        id: pipeSt2
        width: parent.width / 10
        height: rc2.pipeSteamThin
        anchors.right: parent.left
        anchors.bottom: parent.bottom
        borderWidth: pipePassBorderWidth
        anchors.bottomMargin: parent.tank.radius
        anchors.rightMargin: 0
        nActivColor: rc2.pipeSteamColor
        z: 10
    }

    Pipe {
        id: pipeSt3
        y: 322
        width: parent.width / 10
        height: rc2.pipeSteamThin
        anchors.verticalCenter: pipeSt1.verticalCenter
        anchors.left: parent.right
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        nActivColor: rc2.pipeSteamColor
        z: 0
    }

    PipeAngle {
        id: pipeStAng1
        width: 7
        height: 7
        anchors.left: pipeSt3.right
        anchors.top: pipeSt3.top
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        anchors.topMargin: 0
        pipeThin: rc2.pipeSteamThin
        nActivColor: rc2.pipeSteamColor
        rotation: 270
        z: 0
    }

    Pipe {
        id: pipeSteam
        width: rc2.pipeSteamThin
        anchors.right: pipeStAng1.right
        anchors.top: pipeStAng1.bottom
        anchors.bottom: boler.bottom
        anchors.bottomMargin: -15
        borderWidth: pipePassBorderWidth
        anchors.topMargin: 0
        horOrVert: false
        anchors.rightMargin: 0
        nActivColor: rc2.pipeSteamColor
        z: 0
    }

    Pipe {
        id: pipeSt4
        height: rc2.pipeSteamThin
        anchors.left: boler.horizontalCenter
        anchors.right: pipeSteam.left
        anchors.bottom: boler.bottom
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.rightMargin: -borderWidth
        nActivColor: rc2.pipeSteamColor
        z: 0
    }

    RegulValveUnit {
        id: regulValveSteam
        name: "КП"
        width: 20
        height: 20
        anchors.top: pipeSteam.bottom
        regValve.nameOnTop: true
        regValve.nameTextPixSize: parent.valveNemeSize
        regValve.nameOnLeft: false
        objectName: parent.objectName + ".VS"
        anchors.topMargin: 0
        backgroundColor: element.colorSteam
        anchors.horizontalCenter: pipeSteam.horizontalCenter
        regValve.level: 0.3
        z: 50
    }

    Rectangle {
        id: cl2
        width: colder.width
        height: width
        color: backgroundCurrentColor
        radius: width / 2
        border.width: parent.borderWidth / 2
        anchors.left: colder.right
        anchors.bottom: colder.bottom
        anchors.bottomMargin: 0
        anchors.leftMargin: parent.pipePassThin * 2
        z: 1
    }

    Pipe {
        id: pipe2
        height: parent.pipePassThin
        anchors.verticalCenter: cl2.verticalCenter
        anchors.left: colder.horizontalCenter
        anchors.right: cl2.left
        nActivColor: pipePassColor
        anchors.rightMargin: -cl2.border.width
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    RegulValveUnit {
        id: regulValveWater
        name: "КВ"
        x: 68
        width: 20
        height: 20
        anchors.top: cl3.top
        regValve.level: 0.3
        backgroundColor: pipeOutWaterColor
        objectName: parent.objectName + ".VW"
        regValve.nameOnTop: true
        anchors.topMargin: 0
        anchors.horizontalCenter: cl2.horizontalCenter
        regValve.nameOnLeft: false
        anchors.horizontalCenterOffset: 0
        regValve.nameTextPixSize: parent.valveNemeSize
    }

    Pipe {
        id: pipeOutWtr4
        height: parent.pipeOutWaterThin
        anchors.left: parent.horizontalCenter
        anchors.right: pipeOutWtrAng2.left
        anchors.top: cl3.top
        nActivColor: parent.pipeOutWaterColor
        anchors.rightMargin: 0
        anchors.topMargin: 0
        borderWidth: pipeBorderWidth
        anchors.leftMargin: 0
    }

    Pipe {
        id: pipeOutWtr3
        x: 75
        width: parent.pipeOutWaterThin
        anchors.top: regulValveWater.bottom
        anchors.bottom: pipeOutWtrAng1.top
        anchors.bottomMargin: 0
        nActivColor: parent.pipeOutWaterColor
        anchors.topMargin: 0
        anchors.horizontalCenter: regulValveWater.horizontalCenter
        horOrVert: false
        borderWidth: pipeBorderWidth
        anchors.horizontalCenterOffset: 0
        z: 0
    }

    PipeAngle {
        id: pipeOutWtrAng1
        x: 157
        y: -39
        width: 7
        height: 7
        anchors.right: pipeOutWtr3.right
        anchors.bottom: pipeOutWtr6.bottom
        anchors.bottomMargin: 0
        nActivColor: parent.pipeOutWaterColor
        pipeThin: parent.pipeSteamThin
        anchors.rightMargin: 0
        borderWidth: pipePassBorderWidth
        rotation: 0
        z: 0
    }

    PipeAngle {
        id: pipeOutWtrAng2
        width: 7
        height: 7
        anchors.right: pipeOutWtr5.right
        anchors.top: pipeOutWtr4.top
        nActivColor: parent.pipeOutWaterColor
        pipeThin: parent.pipeSteamThin
        anchors.rightMargin: 0
        anchors.topMargin: 0
        borderWidth: pipePassBorderWidth
        rotation: 270
        z: 0
    }

    Pipe {
        id: pipeOutWtr5
        x: 77
        width: parent.pipeOutWaterThin
        anchors.top: pipeOutWtrAng2.bottom
        anchors.bottom: colder.top
        anchors.bottomMargin: 0
        nActivColor: parent.pipeOutWaterColor
        anchors.topMargin: 0
        anchors.horizontalCenter: colder.horizontalCenter
        borderWidth: pipeBorderWidth
        horOrVert: false
        anchors.horizontalCenterOffset: 0
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
        nActivColor: parent.pipeOutWaterColor
        anchors.rightMargin: 0
        borderWidth: pipeBorderWidth
        anchors.leftMargin: 0
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2}
}
##^##*/

