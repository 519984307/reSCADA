import QtQuick 2.0


Item {
    width: 100
    height: 50
    visible: true
    id: root
    property string name: "unknown"
    Rectangle{
        MouseArea {
            anchors.fill: parent
            onClicked: {
                prop.show()
                prop.requestActivate()
                prop.setX(root.x)
                prop.setY(root.y)
            }
        }
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "green"
        border.color: "black"
        border.width: 2
        visible: parent.visible
    }
}
