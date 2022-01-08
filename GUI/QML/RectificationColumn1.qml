import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
RectificationColumn0 {
    id: rc1
    width: 70
    height: 350

    pipeOutWtr1.anchors.verticalCenter: cl1.verticalCenter
    pipeOutWtr1.anchors.verticalCenterOffset: 0
    pipeOutWtr1.width: 65

    //pipeOutWtr1.anchors.right: cl1.verticalCenter
    Rectangle {
        id: rectTop
        x: 27
        width: parent.width / 4
        border.width: 2
        anchors.top: cl1.verticalCenter
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
        x: 11
        y: -55
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
        anchors.left: upAng.right
        anchors.bottom: parent.top
        anchors.leftMargin: pipeV1.width / 2
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
        pipeThin: parent.pipePassThin
        z: 0
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
        borderWidth: pipePassBorderWidth
        nActivColor: pipePassColor
        z: 0
    }

    Pipe {
        id: pipeV1
        x: 90
        width: parent.pipePassThin
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
        anchors.leftMargin: -parent.pipePassThin
        anchors.topMargin: 0
        rotation: 90
        nActivColor: pipePassColor
        pipeThin: parent.pipePassThin
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
        pipeThin: parent.pipePassThin
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
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        z: 0
    }

    Pipe {
        id: pipe2
        height: parent.pipePassThin
        anchors.verticalCenter: colder.verticalCenter
        anchors.left: colder.right
        anchors.right: cl2.left
        anchors.rightMargin: -cl2.border.width
        anchors.leftMargin: -parent.borderWidth
        nActivColor: pipePassColor
        borderWidth: pipePassBorderWidth
        z: 0
    }
    Row {
        id: ang180
        height: width / 2
        anchors.left: pipeAngBot3.right
        anchors.right: pipeV2.right
        anchors.top: pipeAngBot3.bottom
        anchors.leftMargin: -pipeAngBot3.pipeThin
        PipeAngle {
            id: butAngl2
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            pipeThin: rc1.pipePassThin
            borderWidth: pipePassBorderWidth
            rotation: 90
        }
        PipeAngle {
            id: butAngl1
            width: height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            pipeThin: rc1.pipePassThin
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
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    Pipe {
        id: pipeV2
        x: 150
        width: parent.pipePassThin
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
        width: parent.pipePassThin
        anchors.top: colder.bottom
        anchors.bottom: ang180_1.top
        anchors.bottomMargin: 0
        anchors.horizontalCenter: colder.horizontalCenter
        anchors.topMargin: 0
        nActivColor: pipePassColor
        borderWidth: pipePassBorderWidth
        horOrVert: false
    }

    PipeAngle {
        id: pipeAngBot4
        width: 7
        height: 7
        anchors.left: pipe3.right
        anchors.top: pipe3.top
        anchors.topMargin: 0
        nActivColor: pipePassColor
        pipeThin: parent.pipePassThin
        anchors.leftMargin: 0
        rotation: 270
        borderWidth: pipePassBorderWidth
        z: 0
    }

    PipeAngle {
        id: pipeAngBot5
        width: 5
        height: 5
        anchors.left: cl2.right
        anchors.top: cl2.verticalCenter
        anchors.topMargin: 0
        nActivColor: pipePassColor
        pipeThin: parent.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: -2
        z: 0
    }

    Pipe {
        id: pipeV4
        x: 134
        width: parent.pipePassThin
        anchors.top: cl2.bottom
        anchors.bottom: ang180.bottom
        anchors.bottomMargin: -parent.pipePassThin * 5
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
        width: parent.pipePassThin
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
        height: parent.pipePassThin
        anchors.left: parent.right
        anchors.right: pAng1.right
        anchors.top: pAng1.bottom
        anchors.rightMargin: 0
        anchors.topMargin: parent.pipeThin
        anchors.leftMargin: 0
        nActivColor: pipePassColor
        z: 0
        borderWidth: pipePassBorderWidth
    }

    Pipe {
        id: pipe4
        height: parent.pipePassThin
        anchors.left: parent.right
        anchors.right: pipeV3.right
        anchors.top: ang180_1.bottom
        anchors.topMargin: parent.pipeThin
        nActivColor: pipePassColor
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        z: 0
        borderWidth: pipePassBorderWidth
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
        anchors.bottomMargin: -parent.width / 3
        anchors.rightMargin: 0
        z: 10
        showLevel: false
        nameText.text: ""
    }
    Pipe {
        id: pipeSt1
        width: parent.width / 10
        height: parent.pipeSteamThin
        anchors.right: parent.left
        anchors.top: boler.top
        anchors.topMargin: boler.height / 6
        anchors.rightMargin: 0
        borderWidth: pipePassBorderWidth
        nActivColor: parent.pipeSteamColor
        z: 0
    }

    Pipe {
        id: pipeSt2
        width: parent.width / 10
        height: parent.pipeSteamThin
        anchors.right: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.tank.radius
        borderWidth: pipeBorderWidth
        anchors.rightMargin: 0
        nActivColor: parent.pipeSteamColor
        z: 10
    }

    Pipe {
        id: pipeSt3
        y: 322
        width: parent.width / 10
        height: parent.pipeSteamThin
        anchors.verticalCenter: pipeSt1.verticalCenter
        anchors.left: parent.right
        anchors.leftMargin: 0
        borderWidth: pipeBorderWidth
        nActivColor: parent.pipeSteamColor
        z: 0
    }

    PipeAngle {
        id: pipeStAng1
        width: 7
        height: 7
        anchors.left: pipeSt3.right
        anchors.top: pipeSt3.top
        anchors.topMargin: 0
        pipeThin: parent.pipeSteamThin
        anchors.leftMargin: 0
        borderWidth: pipeBorderWidth
        nActivColor: parent.pipeSteamColor
        z: 0
        rotation: 270
    }

    Pipe {
        id: pipeSteam
        width: parent.pipeSteamThin
        height: 40
        anchors.right: pipeStAng1.right
        anchors.top: pipeStAng1.bottom
        anchors.rightMargin: 0
        anchors.topMargin: 0
        anchors.bottom: boler.bottom
        anchors.bottomMargin: -15
        borderWidth: parent.pipeBorderWidth
        nActivColor: parent.pipeSteamColor
        z: 0
        horOrVert: false
    }

    Pipe {
        id: pipeSteam2
        height: parent.pipeSteamThin
        anchors.left: boler.horizontalCenter
        anchors.right: pipeSteam.left
        anchors.bottom: boler.bottom
        anchors.rightMargin: -borderWidth
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        borderWidth: pipeBorderWidth
        nActivColor: parent.pipeSteamColor
        z: 0
    }

    Row {
        id: ang180_1
        height: width / 2
        anchors.left: pipeAngBot4.right
        anchors.right: pipeV3.right
        anchors.top: pipeAngBot4.bottom
        spacing: 0
        anchors.leftMargin: -pipeAngBot3.pipeThin
        PipeAngle {
            id: butAngl3
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            borderWidth: pipePassBorderWidth
            pipeThin: rc1.pipePassThin
            rotation: 90
        }
        PipeAngle {
            id: butAngl4
            width: height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            borderWidth: pipePassBorderWidth
            pipeThin: rc1.pipePassThin
        }
    }

    RegulValveUnit {
        id: regulValveSteam
        name: "КП"
        objectName: parent.objectName + ".VS"
        z: 50
        width: 20
        height: 20
        anchors.top: pipeSteam.bottom
        anchors.horizontalCenter: pipeSteam.horizontalCenter
        regValve.nameTextPixSize: parent.valveNemeSize
        regValve.level: 0.3
        backgroundColor: pipeSteamColor
        anchors.topMargin: 0
        regValve.nameOnLeft: false
        regValve.nameOnTop: true
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
        anchors.leftMargin: rc1.pipePassThin * 2
        z: 1
    }

    Pipe {
        id: pipe5
        height: rc1.pipePassThin
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
        anchors.top: pipeOutWtr4.top
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
        y: -59
        height: parent.pipeOutWaterThin
        anchors.left: pipeOutWtrAng3.right
        anchors.right: pipeOutWtrAng2.left
        nActivColor: parent.pipeOutWaterColor
        anchors.rightMargin: 0
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
        nActivColor: rc1.pipeOutWaterColor
        pipeThin: rc1.pipeOutWaterThin
        anchors.rightMargin: 0
        borderWidth: pipeBorderWidth
        rotation: 0
        z: 0
    }

    PipeAngle {
        id: pipeOutWtrAng2
        width: 7
        height: 7
        anchors.right: pipeOutWtr5.right
        anchors.top: pipeOutWtr4.top
        nActivColor: rc1.pipeOutWaterColor
        pipeThin: rc1.pipeOutWaterThin
        anchors.rightMargin: 0
        anchors.topMargin: 0
        borderWidth: pipeBorderWidth
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
        nActivColor: parent.pipeOutWaterColor
        anchors.rightMargin: 0
        borderWidth: pipeBorderWidth
        anchors.leftMargin: 0
    }

    Pipe {
        id: pipeOutWtr7
        x: 77
        width: parent.pipeOutWaterThin
        anchors.top: pipeOutWtrAng2.bottom
        anchors.bottom: cl1.top
        anchors.bottomMargin: 0
        nActivColor: parent.pipeOutWaterColor
        anchors.topMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter
        borderWidth: pipeBorderWidth
        horOrVert: false
        anchors.horizontalCenterOffset: 0
        z: 0
    }

    PipeAngle {
        id: pipeOutWtrAng3
        width: 7
        height: 7
        anchors.left: pipeOutWtr7.left
        anchors.top: pipeOutWtr4.top
        anchors.leftMargin: 0
        nActivColor: rc1.pipeOutWaterColor
        pipeThin: rc1.pipeOutWaterThin
        anchors.topMargin: 0
        borderWidth: pipeBorderWidth
        rotation: 180
        z: 0
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2;height:350;width:50}
}
##^##*/

