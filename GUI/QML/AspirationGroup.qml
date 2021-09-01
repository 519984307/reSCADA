import QtQuick 2.12

Column {
    id: root
    property real ratio: 0.8
    property alias aspName: asp.name
    property alias aspObjectName: asp.objectName
    property alias fgName: fg.name
    property alias fgObjectName: fg.objectName
    property alias asp: asp
    property alias fg: fg
    spacing: 4
    Item{
        id: areaAsp
		z: 10
        height: parent.height*root.ratio - spacing
        width: parent.width
        Fan{
            id: asp
            anchors.fill: parent
            mount: false
        }
    }
    Item{
        id: areaFG
		z: -10
        height: parent.height*(1-root.ratio)
        width: parent.width
        Aspiration{
            id: fg
            anchors.fill: parent
        }
    }
}
