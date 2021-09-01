import QtQuick 2.12
import "fap.js" as Fap

Rectangle{
    id: root
    width: 10
    height: 50
    color: Fap.indicatorBackground
    visible: false
    property int counter: 0
    property bool multicolor: false


    function activateHL(){
        root.visible = true
        hl.visible = true
        hl.color = multicolor?Fap.indicatorHLActivate:Fap.indicatorLLActivate
    }
    function deactivateHL(){
        root.visible = true
        hl.visible = true
        hl.color = Fap.indicatorDeactivate
    }
    function activateML(){
        root.visible = true
        ml.visible = true
        ml.color = multicolor?Fap.indicatorMLActivate:Fap.indicatorLLActivate
    }
    function deactivateML(){
        root.visible = true
        ml.visible = true
        ml.color = Fap.indicatorDeactivate
    }
    function activateLL(){
        root.visible = true
        ll.visible = true
        ll.color = Fap.indicatorLLActivate
    }
    function deactivateLL(){
        root.visible = true
        ll.visible = true
        ll.color = Fap.indicatorDeactivate
    }

    Column{
      anchors.margins: 1
      anchors.fill: parent
      Rectangle{
          id: hl
          visible: false
          onVisibleChanged: root.counter += visible?1:-1
          height: counter === 0?0:parent.height / counter
          width: parent.width

      }
      Rectangle{
          id: ml
          visible: false
          onVisibleChanged: root.counter += visible?1:-1
          height: counter === 0?0:parent.height / counter
          width: parent.width
      }
      Rectangle{
          id: ll
          visible: false
          onVisibleChanged: root.counter += visible?1:-1
          height: counter === 0?0:parent.height / counter
          width: parent.width
      }
    }
}

