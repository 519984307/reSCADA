import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "fap.js" as Fap
Item {
    id: root
    width: 500
    height: 538
    property int borderWidth: 2
    property int canvasOffset: 10
    property alias name: lable.text
    property alias st: ut.st
    property alias alarm: prop.alarm
    property bool humidity: false
    property bool levelReverse: false
    onAlarmChanged: canvas.requestPaint()
    //property string objectName:""

    signal stop()
    signal run()
    signal runAuto()
    signal cooling()

    signal passUnload(bool status)
    signal passCircule(bool status)
    signal passLoad(bool status)

    function setUnload(status) { buttonUnload.checked = status  }
    function setCircule(status){ buttonCirc.checked = status    }
    function setLoad(status)   { buttonLoad.checked = status    }

    function setStatus(value){statusBar.setLableSilent(value)}

    function setTemp1(value){  temp1.setLableSilent(value)  }
    function setTemp2(value){  temp2.setLableSilent(value)  }
    function setTemp3(value){  temp3.setLableSilent(value)  }
    function setTemp4(value){  temp4.setLableSilent(value)  }
    function setTemp5(value){  temp5.setLableSilent(value)  }
    function setTemp6(value){  temp6.setLableSilent(value)  }
    function setTemp7(value){  temp7.setLableSilent(value)  }
    function setTemp8(value){  temp8.setLableSilent(value)  }
    function setTemp9(value){  temp9.setLableSilent(value)  }
    function setTemp10(value){ temp10.setLableSilent(value) }
    function setTemp11(value){ temp11.setLableSilent(value) }
    function setTemp12(value){ temp12.setLableSilent(value) }
    function setTemp13(value){ temp13.setLableSilent(value) }
    function setTemp14(value){ temp14.setLableSilent(value) }
    function setTemp15(value){ temp15.setLableSilent(value) }


    function setAlarmTemp1(value){  alarmTemp1.setLableSilent(value)  }
    function setAlarmTemp2(value){  alarmTemp2.setLableSilent(value)  }
    function setAlarmTemp3(value){  alarmTemp3.setLableSilent(value)  }
    function setAlarmTemp4(value){  alarmTemp4.setLableSilent(value)  }
    function setAlarmTemp5(value){  alarmTemp5.setLableSilent(value)  }
    function setAlarmTemp6(value){  alarmTemp6.setLableSilent(value)  }
    function setAlarmTemp7(value){  alarmTemp7.setLableSilent(value)  }
    function setAlarmTemp8(value){  alarmTemp8.setLableSilent(value)  }
    function setAlarmTemp9(value){  alarmTemp9.setLableSilent(value)  }
    function setAlarmTemp10(value){ alarmTemp10.setLableSilent(value) }
    function setAlarmTemp11(value){ alarmTemp11.setLableSilent(value) }
    function setAlarmTemp12(value){ alarmTemp12.setLableSilent(value) }
    function setAlarmTemp13(value){ alarmTemp13.setLableSilent(value) }
    function setAlarmTemp14(value){ alarmTemp14.setLableSilent(value) }
    function setAlarmTemp15(value){ alarmTemp15.setLableSilent(value) }


    signal alarmTemp1Changed (variant value);
    signal alarmTemp2Changed (variant value);
    signal alarmTemp3Changed (variant value);
    signal alarmTemp4Changed (variant value);
    signal alarmTemp5Changed (variant value);
    signal alarmTemp6Changed (variant value);
    signal alarmTemp7Changed (variant value);
    signal alarmTemp8Changed (variant value);
    signal alarmTemp9Changed (variant value);
    signal alarmTemp10Changed(variant value);
    signal alarmTemp11Changed(variant value);
    signal alarmTemp12Changed(variant value);
    signal alarmTemp13Changed(variant value);
    signal alarmTemp14Changed(variant value);
    signal alarmTemp15Changed(variant value);

    function setBurnersTargetTemp(value){
        brsTargetTemp.setLableSilent(value)
    }
    function setBurnersAlarmTemp(value){
        brsAlarmTemp.setLableSilent(value)
    }

    signal burnersTargetTempChanged(variant value)
    signal burnersAlamTempChanged(variant value)


    MouseArea {
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            if(prop.visible==false){
                var absolutePos = Fap.getAbsolutePosition(canvas);
                prop.setX(absolutePos.x+mouseX)
                prop.setY(absolutePos.y+mouseY)
                prop.show()
            }
        }
    }

    RegExpValidator {
        id: intreg
        regExp: /[0-9]+/
    }
    RegExpValidator {
        id: floatreg
        regExp: /[0-9]+[.][0-9]/
    }

    Canvas {
        id: canvas
        width: parent.width+canvasOffset*2
        height: parent.height+canvasOffset*2
        x: -canvasOffset
        y: -canvasOffset
        visible: parent.visible
        onPaint: {
            var context = getContext("2d");
            context.clearRect(0,0,width,height);
            context.strokeStyle = "black";
            context.fillStyle = root.alarm?Fap.alarm:"gray"
            context.lineWidth = borderWidth*2;
            context.beginPath();
            context.moveTo(canvasOffset, height-canvasOffset);
            context.lineTo(width-canvasOffset, height-canvasOffset);
            context.lineTo(width-canvasOffset, canvasOffset+height/5);
            context.lineTo(width-width/3-canvasOffset, canvasOffset);
            context.lineTo(canvasOffset+width/3, canvasOffset);
            context.lineTo(canvasOffset, canvasOffset+height/5);
            context.lineTo(canvasOffset, height-canvasOffset);
            context.closePath;
            context.stroke();
            context.fill();
        }

    }


    //    Dryier{
    //        anchors.fill: parent
    //    }

    Text{
        id: lable
        x: 203
        y: 76
        width: 81
        height: 29
        text: "unknown dryier"
        font.pixelSize: 14
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Row {
        x: 8
        y: 426
        width: 150
        height: 99
        spacing: -8
        Fan {
            id: b1
            name: "Г1"
            x: 20
            y: 43
            width: 56
            height: 55
            objectName: root.objectName + ".b1"
            mount: false
            function setTemp(value){
                temp16.setLableSilent(value)
            }
            function setTargetTemp(value) {
                targetTemp16.setLableSilent(value)
            }
            function setAlarmTemp(value) {
                alarmTemp16.setLableSilent(value)
            }
            signal alarmTempChanged(variant value)
            signal targetTempChanged(variant value)
            Text {
                x: 16
                y: -38
                width: 48
                height: 45
                color: parent.std ? "#ff0000" : "#737373"
                text: "i"
                verticalAlignment: Text.AlignVCenter
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }
            fontSize: 8
        }

        Fan {
            id: b2
            name: "Г2"
            x: 46
            y: 43
            width: 56
            height: 55
            objectName: root.objectName + ".b2"
            mount: false
            function setTemp(value){
                temp17.setLableSilent(value)
            }
            function setTargetTemp(value) {
                targetTemp17.setLableSilent(value)
            }
            function setAlarmTemp(value) {
                alarmTemp17.setLableSilent(value)
            }
            signal alarmTempChanged(variant value)
            signal targetTempChanged(variant value)
            Text {
                x: 16
                y: -38
                width: 48
                height: 45
                color: parent.std ? "#ff0000" : "#737373"
                text: "i"
                verticalAlignment: Text.AlignVCenter
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }
            fontSize: 8
        }

        Fan {
            id: b3
            name: "Г3"
            x: 94
            y: 43
            width: 56
            height: 55
            objectName: root.objectName + ".b3"
            mount: false
            function setTemp(value){
                temp18.setLableSilent(value)
            }
            function setTargetTemp(value) {
                targetTemp18.setLableSilent(value)
            }
            function setAlarmTemp(value) {
                alarmTemp18.setLableSilent(value)
            }
            signal alarmTempChanged(variant value)
            signal targetTempChanged(variant value)
            Text {
                x: 16
                y: -38
                width: 48
                height: 45
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

    Row{
        spacing: -8
        x: 338
        y: 169
        width: 152
        height: 55
        Fan{
            name: "ЦФ1"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf1"
        }

        Fan {
            name: "ЦФ2"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf2"
        }

        Fan {
            name: "ЦФ3"
            x: 100
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf3"
        }
    }

    Row{
        spacing: -8
        x: 338
        y: 236
        width: 152
        height: 55
        Fan {
            name: "ЦФ4"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf4"
        }

        Fan {
            name: "ЦФ5"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf5"
        }

        Fan {
            name: "ЦФ6"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf6"
        }
    }

    Row{
        spacing: -8
        x: 338
        y: 304
        width: 152
        height: 55
        Fan {
            name: "ЦФ7"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf7"
        }

        Fan {
            name: "ЦФ8"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf8"
        }

        Fan {
            name: "ЦФ9"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf9"
        }
    }

    Row{
        spacing: -8
        x: 338
        y: 371
        width: 152
        height: 55
        Fan {
            name: "ЦФ10"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf10"
        }

        Fan {
            name: "ЦФ11"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf11"
        }

        Fan {
            name: "ЦФ12"
            width: 56
            height: 55
            fontSize: 8
            mount: false
            objectName: root.objectName + ".cf12"
        }
    }

    Item{
        id: silo
        x: 168
        y: 8
        height: 62
        width: 150
        objectName: root.objectName + ".bun"

        signal addToCurrentRoteFull()
        signal addToCurrentRoteEmpty()

        function activateHL1(){   indicator1.activateHL()  }
        function deactivateHL1(){ indicator1.deactivateHL()}
        function activateML1(){   indicator1.activateML()  }
        function deactivateML1(){ indicator1.deactivateML()}
        function activateLL1(){   indicator1.activateLL()  }
        function deactivateLL1(){ indicator1.deactivateLL()}

        function activateHL2(){   indicator2.activateHL()  }
        function deactivateHL2(){ indicator2.deactivateHL()}
        function activateML2(){   indicator2.activateML()  }
        function deactivateML2(){ indicator2.deactivateML()}
        function activateLL2(){   indicator2.activateLL()  }
        function deactivateLL2(){ indicator2.deactivateLL()}

        function activateHL3(){   indicator3.activateHL()  }
        function deactivateHL3(){ indicator3.deactivateHL()}
        function activateML3(){   indicator3.activateML()  }
        function deactivateML3(){ indicator3.deactivateML()}
        function activateLL3(){   indicator3.activateLL()  }
        function deactivateLL3(){ indicator3.deactivateLL()}


        Indicator2{
            id: indicator1
            x: root.levelReverse?100:0
            y: 0
            width: parent.width / 3
            height: parent.height
            multicolor: true
            visible: true
            objectName: root.objectName + ".l1"
        }
        Indicator2{
            id: indicator2
            x: 50
            y: 0
            width: parent.width / 3
            height: parent.height
            multicolor: true
            visible: true
            objectName: root.objectName + ".l2"
        }
        Indicator2{
            id: indicator3
            x: root.levelReverse?0:100
            y: 0
            width: parent.width / 3
            height: parent.height
            multicolor: true
            visible: true
            objectName: root.objectName + ".l3"
        }
        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if(mouse.modifiers & Qt.ShiftModifier){
                    silo.addToCurrentRoteEmpty()
                }else if(mouse.modifiers & Qt.ControlModifier){
                    silo.addToCurrentRoteFull()
                }else if(mouse.button & Qt.RightButton) {
                    if(siloprop.visible==false){
                        var absolutePos = Fap.getAbsolutePosition(silo);
                        siloprop.setX(absolutePos.x+mouseX)
                        siloprop.setY(absolutePos.y+mouseY)
                        siloprop.show()
                    }
                    siloprop.requestActivate()
                }
            }
        }

        PropertyWindow2 {
            id: siloprop
            objectName: "prop"
            winTitle: "Свойства накопителя " + root.name
            onExNameChanged: {textField.text = siloprop.exName}
            buttonsType: 4
        }

    }





    Column {
        x: 174
        y: 166
        width: 150
        height: 60
        spacing: -1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "верх t°C продукта"
            visible: true
        }

        Row {
            width: parent.width + 2


            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: temp1
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в верхней зоне 1"
                visible: true
                blinkColor: "#ff0000"
                objectName: root.objectName + ".in1.temp1"
            }

            MFUnit {
                id: temp2
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в верхней зоне 2"
                visible: true
                blinkColor: "#ff0000"
                objectName: root.objectName + ".in1.temp2"
            }

            MFUnit {
                id: temp3
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в верхней зоне 3"
                visible: true
                blinkColor: "#ff0000"
                objectName: root.objectName + ".in1.temp3"
            }
        }

        Row {
            width: parent.width + 2


            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: alarmTemp1
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в верхней зоне 1"
                visible: true
                blinkColor: "#ff0000"
                objectName: root.objectName + ".in1.alarmTemp1"
                readOnly: false
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp1Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp2
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в верхней зоне 2"
                visible: true
                blinkColor: "#ff0000"
                objectName: root.objectName + ".in1.alarmTemp2"
                readOnly: false
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp2Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp3
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в верхней зоне 3"
                visible: true
                blinkColor: "#ff0000"
                objectName: root.objectName + ".in1.alarmTemp3"
                readOnly: false
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp3Changed(value)
                checkLimit: true
                downLimit: 20
            }
        }
    }

    Column {
        x: 8
        y: 214
        width: 150
        height: 60
        spacing: -1
        visible: root.humidity
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "вых. влажность"
        }
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            transformOrigin: Item.Center
            blinkColor: "#ff0000"
            objectName: root.objectName + ".out2.temp1"
        }
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            correctingButtons: true
            objectName: root.objectName + ".out2.templimiter"
        }
    }
    Column {
        x: 8
        y: 166
        width: 150
        height: 42
        spacing: -1
        visible: root.humidity
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 2
            text: "вх. влажность"
        }
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 2
            transformOrigin: Item.Center
            blinkColor: "#ff0000"
            objectName: root.objectName + ".out2.temp1"
        }
    }

    Row {
        id: rowUt
        x: 178
        y: 489
        width: 315
        height: 40
        Transporter{
            id: ut
            name: "Выгрузной стол"
            y: 2
            height: parent.height - 4
            anchors.right: utSpeed.left
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            objectName: root.objectName + ".ut"
            signal speedChanged(variant value)
            function setSpeed(value){
                utSpeed.setLableSilent(value)
            }
        }

        MFUnit{
            id: utSpeed
            name: "Скорость выгрузки"
            x: 76
            y: 0
            width: 130
            height: parent.height
            anchors.right: parent.right
            anchors.rightMargin: 0
            backgroundColor: "#5fd5fe"
            tooltip: "Скорость движения выгрузного стола в процентах (от 0 до 100)"
            correctingButtons: true
            readOnly: false
            checkLimit: true
            objectName: root.objectName + ".ut.mf"
            onValueChanged: ut.speedChanged(value)
        }
    }
    Row {
        property int count: 3
        x: 175
        y: 434
        width: 318
        height: 49
        Column{
            width: parent.width-70
            height: parent.height

            Row {
                id: column1
                width: parent.width
                height: parent.height / 2
                property  int count : 2

                Button {
                    width: parent.width/ parent.count
                    height: parent.height
                    text: "Пуск"
                    onClicked: root.run()
                }
                Button {
                    width: parent.width/ parent.count
                    height: parent.height
                    text: "Продувка и стоп"
                    onClicked: root.cooling()
                }

            }
            Button {
                width: parent.width
                height: parent.height / 2
                text: "Пуск c авт. контролем влажности"
                onClicked: root.runAuto()
                enabled: root.humidity
            }

        }
        Button {
            width: 70
            height: parent.height
            text: "СТОП"
            style: ButtonStyle{
                background: Rectangle{
                    color: control.pressed?"firebrick":"red"
                    radius: 3
                }
            }
            onClicked: root.stop()
        }
    }
    Column {
        x: 175
        y: 232
        width: 150
        height: 60
        spacing: -1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "срдн t°C продукта"
            visible: true
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: temp4
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в средней зоне 1"
                objectName: root.objectName + ".in1.temp4"
                visible: true
                blinkColor: "#ff0000"

            }

            MFUnit {
                id: temp5
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в средней зоне 2"
                objectName: root.objectName + ".in1.temp5"
                visible: true
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp6
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в средней зоне 3"
                objectName: root.objectName + ".in1.temp6"
                visible: true
                blinkColor: "#ff0000"
            }
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: alarmTemp4
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в средней зоне 1"
                objectName: root.objectName + ".in1.alarmTemp4"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp4Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp5
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в средней зоне 2"
                objectName: root.objectName + ".in1.alarmTemp5"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp5Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp6
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в средней зоне 3"
                objectName: root.objectName + ".in1.alarmTemp6"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp6Changed(value)
                checkLimit: true
                downLimit: 20
            }
        }
    }

    Column {
        x: 175
        y: 299
        width: 150
        height: 60
        spacing: -1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "низ t°C продукта"
            visible: true
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: temp7
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в нижней зоне 1"
                objectName: root.objectName + ".in1.temp7"
                blinkColor: "#ff0000"
                visible: true
            }

            MFUnit {
                id: temp8
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в нижней зоне 2"
                objectName: root.objectName + ".in1.temp8"
                blinkColor: "#ff0000"
                visible: true
            }

            MFUnit {
                id: temp9
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в нижней зоне 3"
                objectName: root.objectName + ".in1.temp9"
                blinkColor: "#ff0000"
                visible: true
            }
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: alarmTemp7
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в нижней зоне 1"
                objectName: root.objectName + ".in1.alarmTemp7"
                readOnly: false
                blinkColor: "#ff0000"
                visible: true
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp7Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp8
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в нижней зоне 2"
                objectName: root.objectName + ".in1.alarmTemp8"
                readOnly: false
                blinkColor: "#ff0000"
                visible: true
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp8Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp9
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в нижней зоне 3"
                objectName: root.objectName + ".in1.alarmTemp9"
                readOnly: false
                blinkColor: "#ff0000"
                visible: true
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp9Changed(value)
                checkLimit: true
                downLimit: 20
            }
        }
    }

    Column {
        x: 175
        y: 368
        width: 150
        height: 60
        spacing: -1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "t°C продукта в ЗО"
            visible: true
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: temp10
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в зоне охлаждения 1"
                objectName: root.objectName + ".in1.temp10"
                visible: true
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp11
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в зоне охлаждения 2"
                objectName: root.objectName + ".in1.temp11"
                visible: true
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp12
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура продукта в зоне охлаждения 3"
                objectName: root.objectName + ".in1.temp12"
                visible: true
                blinkColor: "#ff0000"
            }
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: alarmTemp10
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в зоне охлаждения 1"
                objectName: root.objectName + ".in1.alarmTemp10"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp10Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp11
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в зоне охлаждения 2"
                objectName: root.objectName + ".in1.alarmTemp11"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp11Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp12
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева продукта в зоне охлаждения 3"
                objectName: root.objectName + ".in1.alarmTemp12"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: root.alarmTemp12Changed(value)
                checkLimit: true
                downLimit: 20
            }
        }
    }

    Column {
        x: 8
        y: 299
        width: 150
        height: 80
        spacing: -1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 4
            text: "вх. t°C носителя "
            visible: true
        }

        Row {
            width: parent.width + 2
            height: parent.height / 4
            spacing: -1
            MFUnit {
                id: temp16
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя 1 до продукта"
                objectName: root.objectName + ".in1.temp16"
                visible: true
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp17
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя 2 до продукта"
                objectName: root.objectName + ".in1.temp17"
                visible: true
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp18
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя 3 до продукта"
                objectName: root.objectName + ".in1.temp18"
                visible: true
                blinkColor: "#ff0000"
            }
        }

        Row {
            id: burnersCommTemp
            width: parent.width + 2
            height: parent.height / 4
            spacing: -1
            MFUnit {
                id: targetTemp16
                name: ""
                width: parent.width / 3
                height: parent.height
                visible: true
                blinkColor: "#ff0000"
                checkLimit: true
                tooltip: "Уставка горелки 1"
                onValueChanged: b1.targetTempChanged(value)
                regexp: floatreg
                downLimit: 20
                readOnly: false
                backgroundColor: "#fdc69f"
                upLimit: 150
                correctingButtons: true
            }

            MFUnit {
                id: targetTemp17
                name: ""
                width: parent.width / 3
                height: parent.height
                visible: true
                blinkColor: "#ff0000"
                checkLimit: true
                tooltip: "Уставка горелки 2"
                onValueChanged: b2.targetTempChanged(value)
                regexp: floatreg
                downLimit: 20
                readOnly: false
                backgroundColor: "#fdc69f"
                upLimit: 150
                correctingButtons: true
            }

            MFUnit {
                id: targetTemp18
                name: ""
                width: parent.width / 3
                height: parent.height
                visible: true
                blinkColor: "#ff0000"
                checkLimit: true
                tooltip: "Уставка горелки 3"
                onValueChanged: b3.targetTempChanged(value)
                regexp: floatreg
                downLimit: 20
                readOnly: false
                backgroundColor: "#fdc69f"
                upLimit: 150
                correctingButtons: true
            }
        }

        Row {
            width: parent.width + 2
            height: parent.height / 4
            spacing: -1
            MFUnit {
                id: alarmTemp16
                name: ""
                width: parent.width / 3
                height: parent.height
                visible: true
                blinkColor: "#ff0000"
                checkLimit: true
                tooltip: "Температура перегрева теплоносителя после горелки 1"
                onValueChanged: b1.alarmTempChanged(value)
                regexp: floatreg
                downLimit: 20
                readOnly: false
                backgroundColor: "#debaba"
                upLimit: 150
                correctingButtons: true
            }

            MFUnit {
                id: alarmTemp17
                name: ""
                width: parent.width / 3
                height: parent.height
                visible: true
                blinkColor: "#ff0000"
                checkLimit: true
                tooltip: "Температура перегрева теплоносителя после горелки 1"
                onValueChanged: b2.alarmTempChanged(value)
                regexp: floatreg
                downLimit: 20
                readOnly: false
                backgroundColor: "#debaba"
                upLimit: 150
                correctingButtons: true
            }

            MFUnit {
                id: alarmTemp18
                name: ""
                width: parent.width / 3
                height: parent.height
                visible: true
                blinkColor: "#ff0000"
                checkLimit: true
                tooltip: "Температура перегрева теплоносителя после горелки 1"
                onValueChanged: b3.alarmTempChanged(value)
                regexp: floatreg
                downLimit: 20
                readOnly: false
                backgroundColor: "#debaba"
                upLimit: 150
                correctingButtons: true
            }
        }
    }

    Column {
        x: 342
        y: 103
        width: 150
        height: 60
        spacing: -1
        MFUnit {
            name: ""
            width: parent.width
            height: parent.height / 3
            text: "вых. t°C носителя "
            visible: true
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: temp13
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя 1 после продукта"
                objectName: root.objectName + ".in1.temp13"
                visible: true
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp14
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя 2 после продукта"
                objectName: root.objectName + ".in1.temp14"
                visible: true
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp15
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя 3 после продукта"
                objectName: root.objectName + ".in1.temp15"
                visible: true
                blinkColor: "#ff0000"
            }
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: alarmTemp13
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева теплоносителя 1 после продукта"
                objectName: root.objectName + ".in1.alarmTemp133"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                regexp: floatreg
                correctingButtons: true
                onValueChanged: root.alarmTemp13Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp14
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева теплоносителя 2 после продукта"
                objectName: root.objectName + ".in1.alarmTemp134"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                regexp: floatreg
                correctingButtons: true
                onValueChanged: root.alarmTemp14Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp15
                name: ""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#ff5d5d"
                tooltip: "Температура перегрева теплоносителя 3 после продукта"
                objectName: root.objectName + ".in1.alarmTemp135"
                readOnly: false
                visible: true
                blinkColor: "#ff0000"
                regexp: floatreg
                correctingButtons: true
                onValueChanged: root.alarmTemp15Changed(value)
                checkLimit: true
                downLimit: 20
            }
        }
    }
    PropertyWindow2 {
        id: prop
        width: 370
        objectName: "prop"
        winTitle: "Свойства "+name
        onStart: root.run()
        onStop: root.stop()
        buttonsType: 4
    }

    MFUnit {
        id: statusBar
        name: ""
        x: 8
        y: 103
        width: 316
        height: 60
        text: "Готовность"
        visible: true
        fontSize: 17
    }
    Button {
        id: buttonLoad
        x: 324
        y: 8
        width: 94
        height: 19
        style: ButtonStyle {
            background: Rectangle {
                color: control.checked?"blue":"lightblue"
                radius: 3
                border.color: "#000000"
                border.width: 1
            }
        }
        checkable: true
        onClicked: root.passLoad(checked)
        Text {
            width: 94
            height: 20
            color: parent.checked?"white":"black"
            text: "Авто загрузка"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }
    }
    Button {
        id: buttonCirc
        x: 324
        y: 30
        width: 94
        height: 19
        style: ButtonStyle {
            background: Rectangle {
                color: control.checked?"blue":"lightblue"
                radius: 3
                border.color: "#000000"
                border.width: 1
            }
        }
        checkable: true
        onClicked: root.passCircule(checked)
        Text {
            height: 20
            color: parent.checked?"white":"black"
            text: "Циркуляция"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }
    }
    Button {
        id: buttonUnload
        x: 324
        y: 51
        width: 94
        height: 19
        style: ButtonStyle {
            background: Rectangle {
                color: control.checked?"blue":"lightblue"
                radius: 3
                border.color: "#000000"
                border.width: 1
            }
        }
        checkable: true
        onClicked: root.passUnload(checked)
        Text {
            height: 20
            color: parent.checked?"white":"black"
            text: "Разгрузка"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            anchors.fill: parent
        }
    }
}


