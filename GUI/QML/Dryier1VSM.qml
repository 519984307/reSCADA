import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "fap.js" as Fap

Item {
    id: root
    objectName: "dr"
    width: 232
    height: 276

    property bool st: false
    property bool std: false
    property alias alarm: prop.alarm
    property alias alarmNotify: prop.alarmNotify
    property bool alarmBlink: false
    property bool blocked: false
    property bool available: prop.available
    property bool linked: prop.linked
    property bool manual: false
    property bool levelReverse: false
    property bool cleaning: false

    property alias fillColor: rectangleFill.color
    property color borderColor: "pink"

//    onStChanged:          Fap.changeColor(root)
//    onStdChanged:         Fap.changeColor(root)
    onAlarmChanged:       { Fap.changeColor(root); }
    onAlarmNotifyChanged: Fap.changeColor(root)
    onAlarmBlinkChanged:  Fap.changeColor(root)
//    onBlockedChanged:     Fap.changeColor(root)
    onAvailableChanged:   Fap.changeColor(root)
//    onLinkedChanged:      Fap.changeColor(root)
//    onManualChanged:      Fap.changeColor(root)

    signal stop()
    signal start()
    signal cooling()

    //signal resetAlarm()

    function started(){
        startComand()
        std=true
        manual=false
        cleaning=false
        Fap.changeColor(root)
    }
    function stoped(){
        stopComand()
        std=false
        manual=false
        cleaning=false
        Fap.changeColor(root)
    }
    function startComand(){
        st=true
        manual=false
        cleaning=false
        Fap.changeColor(root)
    }
    function stopComand(){
        st=false
        manual=false
        cleaning=false
        Fap.changeColor(root)
    }
    function manualWork(){
        manual=true
        Fap.changeColor(root)
    }

    function cleaningWork(){
        cleaning=true
        Fap.changeColor(root)
    }

    function setTempHL(value){ tempHL.setLableSilent(value) }
    function setTempML(value){ tempML.setLableSilent(value) }
    function setTempLL(value){ tempLL.setLableSilent(value) }
    function setTempEX(value){ tempEX.setLableSilent(value) }


    function setOverTempHL(value){ overTempHL.setLableSilent(value) }
    function setOverTempML(value){ overTempML.setLableSilent(value) }
    function setOverTempLL(value){ overTempLL.setLableSilent(value) }
    function setOverTempEx(value){ overTempEX.setLableSilent(value) }

    signal overTempHLChanged(variant value);
    signal overTempMLChanged(variant value);
    signal overTempLLChanged(variant value);
    signal overTempExChanged(variant value);

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            if(prop.visible==false){
                var absolutePos = Fap.getAbsolutePosition(parent);
                prop.setX(absolutePos.x+mouseX)
                prop.setY(absolutePos.y+mouseY)
                prop.show()
            }
        }
    }

    Rectangle {
        id: rectangleFill
        x: 6
        y: 6
        color: root.alarm?Fap.alarm:"gray"
        anchors.fill: parent
        border.width: 5
        border.color: root.borderColor
    }

    Button {
        id: button
        x: 8
        y: 6
        width: 65
        height: 30
        checkable: true
        Text {
            id: text_1
            text: qsTr("Авт.зак.")
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            color: parent.checked?"white":"black"
        }
        Text {
            text: qsTr("З 02.05")
            anchors.top: text_1.bottom
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            color: parent.checked?"white":"black"
        }
        style: ButtonStyle {
            background: Rectangle {
                color: control.checked?"blue":"lightblue"
                radius: 3
                border.color: "#000000"
                border.width: 1
            }
        }
        onClicked: bun.autoGate(checked)
    }

    RegExpValidator {
        id: intreg
        regExp: /[0-9]+/
    }
    RegExpValidator {
        id: floatreg
        regExp: /[0-9]+[.][0-9]/
    }

//            Canvas {
//                id: canvas
//                anchors.fill: parent
//                visible: parent.visible
    //        onPaint: {
    //            var context = getContext("2d");
    //            context.clearRect(0,0,width,height);
    //            context.strokeStyle = "black";
    //            context.fillStyle = root.alarm?Fap.alarm:"gray"
    //            context.lineWidth = borderWidth*2;
    //            context.beginPath();
    //            context.moveTo(canvasOffset, height-canvasOffset);
    //            context.lineTo(width-canvasOffset, height-canvasOffset);
    //            context.lineTo(width-canvasOffset, canvasOffset+height/5);
    //            context.lineTo(width-width/3-canvasOffset, canvasOffset);
    //            context.lineTo(canvasOffset+width/3, canvasOffset);
    //            context.lineTo(canvasOffset, canvasOffset+height/5);
    //            context.lineTo(canvasOffset, height-canvasOffset);
    //            context.closePath;
    //            context.stroke();
    //            context.fill();
    //        }

//            }

    Row {
        id: rowBr
        x: 6
        y: 132
        width: 65
        height: 72
        spacing: -8
        Fan {
            id: br
            name: "Гр."
            x: 20
            y: 40
            width: 40
            height: 45
            //textX: 22
            //textY:
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            objectName: root.objectName + ".br"
            mount: false
            function setTemp(value){
                tempBr.setLableSilent(value)
            }
            function setTargetTemp(value){
                targetTempBr.setLableSilent(value)
            }
            function setOverTemp(value){
                overTempBr.setLableSilent(value)
            }
            signal targetTempChanged(variant value)
            signal overTempChanged(variant value)
            Text {
                x: 19
                y: -24
                width: 33
                height: 29
                color: parent.std ? "#ff0000" : "#737373"
                text: "i"
                verticalAlignment: Text.AlignVCenter
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }
            fontSize: 8
        }
    }

    Row {
        id: rowButton
        x: 4
        y: 240
        width: 204
        height: 30
        spacing: 0
        property int count: 3
        Button {
            width: parent.width/ parent.count
            height: parent.height
            text: "Пуск"
            style: ButtonStyle {
                background: Rectangle {
                    border.width: 1
                    border.color: "black"
                    color: control.pressed?"green":"lightgreen"
                    radius: 6
                }
            }
             onClicked: root.start()
        }

        Button {
            width: parent.width/ parent.count
            height: parent.height
            text: "Стоп"
            style: ButtonStyle {
                background: Rectangle {
                    border.width: 1
                    border.color: "black"
                    color: control.pressed?"blue":"lightblue"
                    radius: 6
                }
            }
            onClicked: root.cooling()
        }

        Button {
            width: parent.width/ parent.count
            height: parent.height
            text: "ПОЖАР!"
            style: ButtonStyle {
                background: Rectangle {
                    border.width: 1
                    border.color: "black"
                    color: control.pressed?"firebrick":"red"
                    radius: 6
                }
            }
            onClicked: root.stop()
        }
    }

    Row {
        id: rowUT
        x: 6
        y: 206
        width: 202
        height: 30
        Transporter{
            id: ut
            name: "Выгрузной стол"
            width: 102

            height: 28
            objectName: root.objectName + ".ut"
            signal speedChanged(variant value)
            x: 0
            y: 1
            borderWidth: 1
            function setSpeed(value){
                utspeed.setLableSilent(value)
            }
        }

        MFUnit{
            id: utspeed
            name: "Частота ЧП (Гц)"
            width: parent.width - ut.width
            height: parent.height
            backgroundColor: "#5fd5fe"
            tooltip: "Частота частотного преобразователя выгрузного стола (Гц)"
            correctingButtons: true
            readOnly: false
            checkLimit: true
            objectName: root.objectName + ".ut.mf"
            onValueChanged: ut.speedChanged(value)
            borderWidth: 1
            regexp: floatreg
        }
    }

    Column{
        id: columnCF
        spacing: -8
        x: 151
        y: 74
        width: 59
        height: 137
        Fan{
            name: "ЦФ1"
            width: 50
            height: 50
            rotation: 30
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf1"
        }

        Fan {
            name: "ЦФ2"
            width: 50
            height: 50
            rotation: 30
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf2"
        }

        Fan {
            name: "ЦФ3"
            x: 0
            width: 50
            height: 50
            rotation: 30
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf3"
        }
    }

    Item{
        id: bun
        objectName: root.objectName + ".bun"

        signal addToCurrentRoteFull()
        signal addToCurrentRoteEmpty()
        signal autoGate(bool state)

        x: 212
        y: 6
        width: 15
        height: 264

        function activateHL(){   level.activateHL()  }
        function deactivateHL(){ level.deactivateHL()}
        function activateML(){   level.activateML()  }
        function deactivateML(){ level.deactivateML()}
        function activateLL(){   level.activateLL()  }
        function deactivateLL(){ level.deactivateLL()}


        Indicator2{
            id: level
            anchors.fill: parent
            multicolor: true
            visible: true
            objectName: root.objectName + ".level"
        }
        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if(mouse.modifiers & Qt.ShiftModifier){
                    parent.addToCurrentRoteEmpty()
                }else if(mouse.modifiers & Qt.ControlModifier){
                    parent.addToCurrentRoteFull()
                }else if(mouse.button & Qt.RightButton) {
                    if(bunProp.visible==false){
                        var absolutePos = Fap.getAbsolutePosition(parent);
                        bunProp.setX(absolutePos.x+mouseX)
                        bunProp.setY(absolutePos.y+mouseY)
                        bunProp.show()
                    }
                    bunProp.requestActivate()
                }
            }
        }

        PropertyWindow2 {
            id: bunProp
            objectName: "prop"
            winTitle: "Свойства бункера сушилки"
            onExNameChanged: {textField.text = bunProp.exName}
            buttonsType: 4
        }

    }

    Column {
        id: culumnHL
        x: 77
        y: 6
        width: 65
        height: 60
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "верх t°C"
            visible: true
        }

        MFUnit {
            id: tempHL
            name: ""
            width: parent.width
            height: parent.height / 3
            blinkColor: "#ff0000"
            backgroundColor: "#afffb4"
            visible: true
            objectName: root.objectName + ".tempHL"
            tooltip: "t°C продукта в верхней зоне"
        }

        MFUnit {
            id: overTempHL
            name: ""
            width: parent.width
            height: parent.height / 3
            blinkColor: "#ff0000"
            downLimit: 20
            backgroundColor: "#c2f65f"
            visible: true
            checkLimit: true
            objectName: root.objectName + ".overTempHL"
            tooltip: "t°C перегрева продукта в верхней зоне"
            regexp: floatreg
            readOnly: false
            correctingButtons: true
            onValueChanged: root.overTempHLChanged(value)
        }
        spacing: 1
    }


    Column {
        id: columnML
        x: 77
        y: 74
        width: 65
        height: 60
        spacing: 1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "сред. t°C"
            visible: true
        }

        MFUnit {
            id: tempML
            name: ""
            width: parent.width
            height: parent.height / 3
            backgroundColor: "#afffb4"
            tooltip: "t°C продукта в средней зоне"
            objectName: root.objectName + ".tempML"
            visible: true
            blinkColor: "#ff0000"

        }

        MFUnit {
            id: overTempML
            name: ""
            width: parent.width
            height: parent.height / 3
            backgroundColor: "#c2f65f"
            tooltip: "t°C перегрева продукта в середине"
            objectName: root.objectName + ".overTempML"
            readOnly: false
            visible: true
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: root.overTempMLChanged(value)
            checkLimit: true
            downLimit: 20
        }
    }

    Column {
        id: columnLL
        x: 77
        y: 141
        width: 65
        height: 60
        spacing: 1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "t°C ЗО"
            visible: true
        }

        MFUnit {
            id: tempLL
            name: ""
            width: parent.width
            height: parent.height/ 3
            backgroundColor: "#afffb4"
            tooltip: "t°C продукта в зоне охлаждения"
            objectName: root.objectName + ".tempLL"
            visible: true
            blinkColor: "#ff0000"
        }

        MFUnit {
            id: overTempLL
            name: ""
            width: parent.width
            height: parent.height/ 3
            backgroundColor: "#c2f65f"
            tooltip: "t°C перегрева продукта в зоне охлаждения"
            objectName: root.objectName + ".overTempLL"
            readOnly: false
            visible: true
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: root.overTempLLChanged(value)
            checkLimit: true
            downLimit: 20
        }

    }



    Column {
        id: columnBr
        x: 6
        y: 48
        width: 65
        height: 80
        z: 1
        spacing: 1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 4
            text: "вх. t°C"
            visible: true
        }


        MFUnit {
            id: tempBr
            name: ""
            width: parent.width
            height: parent.height / 4
            backgroundColor: "#afffb4"
            tooltip: "t°C теплоносителя после горелки"
            objectName: root.objectName + ".tempBr"
            visible: true
            blinkColor: "#ff0000"
        }
        MFUnit {
            id: targetTempBr
            name: ""
            width: parent.width
            height: parent.height/ 4
            backgroundColor: "#ff5d5d"
            tooltip: "Уставка t°C теплоносителя после горелки"
            objectName: root.objectName + ".targetTempBr"
            readOnly: false
            visible: true
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: br.targetTempChanged(value)
            checkLimit: true
            downLimit: 20
            upLimit: 150
        }
        MFUnit {
            id: overTempBr
            name: ""
            width: parent.width
            height: parent.height/ 4
            backgroundColor: "#c2f65f"
            tooltip: " t°C  перегрева теплоносителя после горелки"
            objectName: root.objectName + ".overTempBr"
            readOnly: false
            visible: true
            blinkColor: "#ff0000"
            correctingButtons: true
            regexp: floatreg
            onValueChanged: br.overTempChanged(value)
            checkLimit: true
            downLimit: 20
            upLimit: 150
        }

    }

    Column {
        id: columnEX
        x: 148
        y: 6
        width: 60
        height: 60
        spacing: 1

        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "вых. t°C "
            visible: true
        }

        MFUnit {
            id: tempEX
            name: ""
            width: parent.width
            height: parent.height/ 3
            backgroundColor: "#afffb4"
            tooltip: "t°C теплоносителя на выходе"
            objectName: root.objectName + ".tempEX"
            visible: true
            blinkColor: "#ff0000"
        }

        MFUnit {
            id: overTempEX
            name: ""
            width: parent.width
            height: parent.height/ 3
            backgroundColor: "#c2f65f"
            tooltip: "t°C перегрева теплоносителя на выходе"
            objectName: root.objectName + ".overTempEX"
            readOnly: false
            visible: true
            blinkColor: "#ff0000"
            regexp: floatreg
            correctingButtons: true
            onValueChanged: root.overTempExChanged(value)
            checkLimit: true
            downLimit: 20
        }

    }
    PropertyWindow2 {
        id: prop
        width: 370
        objectName: "prop"
        winTitle: "Свойства Сушилки"
        onStart: root.run()
        onStop: root.stop()
        buttonsType: 4
    }
}

