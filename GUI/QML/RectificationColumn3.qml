import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.13

//import LineComponent 1.0
RectificationColumn0 {
    id: rc3
    width: 50
    height: 450
    property alias cl1: cl1

    pipeOutWtr1.anchors.verticalCenter: cl4.verticalCenter
    pipeOutWtr1.anchors.verticalCenterOffset: 13
    pipeOutWtr1.width: 65
    //    Rectangle {
    //        y: -27
    //        width: 50
    //        height: 5
    //        anchors.verticalCenter: cl4.verticalCenter
    //        anchors.right: parent.horizontalCenter
    //        anchors.rightMargin: 0
    //    }
    Rectangle {
        id: rectTop
        x: 27
        width: parent.width / 4
        border.width: 2
        anchors.top: cl4.verticalCenter
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
        y: -70
        z: 10
        width: parent.width / 3
        height: parent.width
        radius: 10
        anchors.left: pipeV7.right
        anchors.bottom: parent.top
        anchors.bottomMargin: 0
        anchors.leftMargin: rc3.pipePassThin
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
        pipeThin: rc3.pipePassThin
        z: 0
    }

    Pipe {
        id: pipe1
        y: -32
        height: rc3.pipePassThin
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
        width: rc3.pipePassThin
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
        anchors.leftMargin: -rc3.pipePassThin
        rotation: 90
        nActivColor: pipePassColor
        pipeThin: 3
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
        pipeThin: rc3.pipePassThin
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
        pipeThin: rc3.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        z: 0
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
        anchors.leftMargin: rc3.pipePassThin * 2
        z: 1
    }

    Pipe {
        id: pipe2
        height: rc3.pipePassThin
        anchors.verticalCenter: cl2.verticalCenter
        anchors.left: colder.horizontalCenter
        anchors.right: cl2.left
        anchors.leftMargin: 0
        anchors.rightMargin: -cl2.border.width
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
        anchors.leftMargin: -rc3.pipePassThin
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
            pipeThin: rc3.pipePassThin
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
            pipeThin: rc3.pipePassThin
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
        pipeThin: rc3.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    Pipe {
        id: pipeV2
        x: 150
        width: rc3.pipePassThin
        anchors.right: pipeAngBot5.right
        z: 0
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
        width: rc3.pipePassThin
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
        anchors.leftMargin: -rc3.pipePassThin
        anchors.rightMargin: 0
        anchors.topMargin: 0
        PipeAngle {
            id: butAngl3
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            pipeThin: rc3.pipePassThin
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
            pipeThin: rc3.pipePassThin
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
        pipeThin: rc3.pipePassThin
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
        pipeThin: rc3.pipePassThin
        rotation: 270
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    Pipe {
        id: pipeV4
        x: 134
        width: rc3.pipePassThin
        height: 80
        anchors.top: cl2.bottom
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
        width: rc3.pipePassThin
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
        y: 77
        width: 15
        height: rc3.pipePassThin
        anchors.left: parent.right
        anchors.leftMargin: 0
        nActivColor: pipePassColor
        z: 0
        borderWidth: pipePassBorderWidth
    }

    Pipe {
        id: pipe4
        y: 101
        height: rc3.pipePassThin
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
        height: rc3.pipePassThin
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
        width: rc3.pipePassThin
        anchors.left: pipeV1.right
        anchors.top: upAng1.bottom
        anchors.bottom: ang180_2.top
        anchors.leftMargin: rc3.pipePassThin / 2
        nActivColor: pipePassColor
        anchors.topMargin: 0
        horOrVert: false
        borderWidth: pipePassBorderWidth
    }

    PipeAngle {
        id: pAng4
        y: 33
        width: 7
        height: 7
        anchors.left: parent.right
        nActivColor: pipePassColor
        pipeThin: rc3.pipePassThin
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
        pipeThin: rc3.pipePassThin
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
        anchors.leftMargin: -rc3.pipePassThin
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
            pipeThin: rc3.pipePassThin
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
            pipeThin: rc3.pipePassThin
            anchors.bottomMargin: 0
            borderWidth: pipePassBorderWidth
        }
    }

    Rectangle {
        id: cl4
        y: -163
        width: parent.width * 0.7
        height: width
        color: backgroundCurrentColor
        radius: width / 2
        border.width: 2
        anchors.bottom: cl3.top
        anchors.bottomMargin: width / 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 0
        z: 1
    }

    PipeAngle {
        id: pAng5
        y: 51
        width: 7
        height: 7
        anchors.left: parent.right
        nActivColor: pipePassColor
        pipeThin: rc3.pipePassThin
        borderWidth: pipePassBorderWidth
        rotation: 270
        anchors.leftMargin: 0
        z: 0
    }

    Row {
        id: ang180_3
        height: width / 2
        anchors.left: pAng5.right
        anchors.right: pipeV7.right
        anchors.top: pAng5.bottom
        anchors.leftMargin: -3
        anchors.rightMargin: 0
        PipeAngle {
            id: butAngl7
            width: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            pipeThin: rc3.pipePassThin
            anchors.bottomMargin: 0
            borderWidth: pipePassBorderWidth
            rotation: 90
            anchors.topMargin: 0
            anchors.leftMargin: 0
            z: 0
        }

        PipeAngle {
            id: butAngl8
            width: height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            nActivColor: pipePassColor
            pipeThin: rc3.pipePassThin
            anchors.bottomMargin: 0
            anchors.rightMargin: 0
            borderWidth: pipePassBorderWidth
            anchors.topMargin: 0
        }
        anchors.topMargin: 0
    }

    Pipe {
        id: pipeV7
        width: rc3.pipePassThin
        anchors.left: pipeV6.right
        anchors.top: upAng2.bottom
        anchors.bottom: ang180_3.top
        anchors.leftMargin: rc3.pipePassThin
        nActivColor: pipePassColor
        anchors.bottomMargin: 0
        horOrVert: false
        borderWidth: pipePassBorderWidth
        anchors.topMargin: 0
    }

    Pipe {
        id: pipe6
        y: -32
        height: rc3.pipePassThin
        anchors.verticalCenter: cl4.verticalCenter
        anchors.left: cl4.right
        anchors.right: upAng2.left
        anchors.verticalCenterOffset: 0
        nActivColor: pipePassColor
        anchors.rightMargin: 0
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        z: 0
    }

    PipeAngle {
        id: upAng2
        x: 87
        width: 10
        height: 10
        anchors.right: pipeV7.right
        anchors.top: pipe6.top
        nActivColor: pipePassColor
        pipeThin: rc3.pipePassThin
        anchors.rightMargin: 0
        borderWidth: pipePassBorderWidth
        rotation: 270
        anchors.topMargin: 0
        z: 0
    }

    Pipe {
        id: pipeSt3
        width: parent.width / 10
        height: rc3.pipeSteamThin
        anchors.left: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height / 15
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        nActivColor: rc3.pipeSteamColor
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
        pipeThin: rc3.pipeSteamThin
        nActivColor: rc3.pipeSteamColor
        rotation: 270
        z: 0
    }

    Pipe {
        id: pipeSteam
        width: rc3.pipeSteamThin
        anchors.right: pipeStAng1.right
        anchors.top: pipeStAng1.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -45
        borderWidth: pipePassBorderWidth
        anchors.topMargin: 0
        horOrVert: false
        anchors.rightMargin: 0
        nActivColor: rc3.pipeSteamColor
        z: 0
    }

    RegulValveUnit {
        id: regulValveSteam
        name: "КП"
        width: 20
        height: 20
        anchors.top: parent.bottom
        anchors.horizontalCenterOffset: 0
        regValve.nameOnTop: true
        regValve.nameTextPixSize: parent.valveNemeSize
        regValve.nameOnLeft: false
        objectName: parent.objectName + ".VS"
        anchors.topMargin: 32
        backgroundColor: element.colorSteam
        anchors.horizontalCenter: pipeSteam.horizontalCenter
        regValve.level: 0.3
        z: 50
    }

    RegulValveUnit {
        id: regulValveWater
        name: "КВ"
        x: 68
        width: 20
        height: 20
        anchors.top: cl4.top
        anchors.topMargin: 0
        anchors.horizontalCenterOffset: 0
        regValve.nameTextPixSize: parent.valveNemeSize
        regValve.level: 0.3
        regValve.nameOnTop: true
        regValve.nameOnLeft: false
        backgroundColor: pipeOutWaterColor
        objectName: parent.objectName + ".VW"
        anchors.horizontalCenter: cl2.horizontalCenter
    }

    Pipe {
        id: pipeOutWtr4
        height: parent.pipeOutWaterThin
        anchors.left: parent.horizontalCenter
        anchors.right: pipeOutWtrAng2.left
        anchors.top: cl4.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        borderWidth: pipeBorderWidth
        nActivColor: parent.pipeOutWaterColor
        anchors.topMargin: 0
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
        nActivColor: parent.pipeOutWaterColor
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
        anchors.rightMargin: 0
        z: 0
        borderWidth: pipeBorderWidth
        rotation: 0
        pipeThin: rc3.pipeSteamThin
        nActivColor: rc3.pipeOutWaterColor
    }

    PipeAngle {
        id: pipeOutWtrAng2
        width: 7
        height: 7
        anchors.right: pipeOutWtr5.right
        anchors.top: pipeOutWtr4.top
        anchors.rightMargin: 0
        pipeThin: rc3.pipeSteamThin
        rotation: 270
        z: 0
        borderWidth: pipeBorderWidth
        nActivColor: rc3.pipeOutWaterColor
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
        nActivColor: parent.pipeOutWaterColor
        horOrVert: false
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
        nActivColor: parent.pipeOutWaterColor
        borderWidth: pipeBorderWidth
    }

    PipeAngle {
        id: upAng3
        width: 10
        height: 10
        anchors.left: pipeV4.left
        anchors.top: pipeV4.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 0
        borderWidth: pipePassBorderWidth
        rotation: 90
        z: 0
        nActivColor: pipePassColor
        pipeThin: rc3.pipePassThin
    }

    Pipe {
        id: pipe7
        y: 65
        width: 15
        height: rc3.pipePassThin
        anchors.left: upAng3.right
        anchors.bottom: upAng3.bottom
        anchors.bottomMargin: 0
        borderWidth: pipePassBorderWidth
        anchors.leftMargin: 0
        nActivColor: pipePassColor
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.1}
}
##^##*/

