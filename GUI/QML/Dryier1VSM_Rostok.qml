import QtQuick 2.12
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "fap.js" as Fap

Item {
    id: root
    objectName: "dr"
    width: 850
    height: 530

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

    property color fillColor: "grey"
    property color borderColor: "red"
    property color alarmTemp: "red"
    property color normalTemp: "#23b4f6"

    //    onStChanged:          Fap.changeColor(root)
    //    onStdChanged:         Fap.changeColor(root)
    onAlarmChanged:       {
        if(alarm) {
            rectangleFill.opacity = 1
        }
        else
            rectangleFill.opacity = 0
    }
    onAlarmNotifyChanged: Fap.changeColor(root)
    onAlarmBlinkChanged:  Fap.changeColor(root)
    //    onBlockedChanged:     Fap.changeColor(root)
    onAvailableChanged:   Fap.changeColor(root)
    //    onLinkedChanged:      Fap.changeColor(root)
    //    onManualChanged:      Fap.changeColor(root
    onBorderColorChanged: {
        if(root.borderColor == "red"){

        }
        else
            rectangleFill.border.width = 0
    }

    signal start()
    signal cooling()
    signal coolingStop()

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
    function openSettings(){
        if(prop.visible==false){
            var absolutePos = Fap.getAbsolutePosition(root);
            prop.setX(absolutePos.x)
            prop.setY(absolutePos.y)
            prop.show()
        }
        prop.requestActivate()
    }
    //!-----------
    function setCoolingTemp(value){ coolingTemp.setLableSilent(value) }
    //    function setCoolingDelay(value){ coolingDelay.setLableSilent(value) }
    //    function setBrDelay(value){ brDelay.setLableSilent(value) }
    //    function setCfDelay(value){ cfDelay.setLableSilent(value) }

    signal coolingTempChanged(variant value);
    //    signal coolingDelayChanged(variant value);
    //    signal brDelayChanged(variant value);
    //    signal cfDelayChanged(variant value);
    //!-----------

    function setTempHL(value){ elTempHL.text = value + "°" }
    function setTempML(value){ elTempML.text = value + "°" }
    function setTempLL(value){ elTempLL.text = value + "°" }
    function setTempEx(value){ elTempEx.text = value + "°" }

    function setTempAlarmHL  (){ elTempHL.color = alarmTemp  }
    function resetTempAlarmHL(){ elTempHL.color = normalTemp }
    function setTempAlarmML  (){ elTempML.color = alarmTemp  }
    function resetTempAlarmML(){ elTempML.color = normalTemp }
    function setTempAlarmLL  (){ elTempLL.color = alarmTemp  }
    function resetTempAlarmLL(){ elTempLL.color = normalTemp }
    function setTempAlarmEx  (){ elTempEx.color = alarmTemp  }
    function resetTempAlarmEx(){ elTempEx.color = normalTemp }

    function setOverTempHL(value){ overTempHL.setLableSilent(value) }
    function setOverTempML(value){ overTempML.setLableSilent(value) }
    function setOverTempLL(value){ overTempLL.setLableSilent(value) }
    function setOverTempEx(value){ overTempEX.setLableSilent(value) }

    signal overTempHLChanged(variant value);
    signal overTempMLChanged(variant value);
    signal overTempLLChanged(variant value);
    signal overTempExChanged(variant value);

    MouseArea {
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.top: rectangleFill.bottom
        anchors.right: rectangleFill.left
        anchors.bottom: rectangleFill.top
        anchors.left: rectangleFill.right
        anchors.topMargin: 0
        z: 5
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
    MFUnit {
        id: targetTempBr
        name: ""
        x: 181
        y: 307
        width: 110
        height: 40
        z: 10
        borderWidth: 2
        fontSize: 18
        backgroundColor: "#ff5d5d"
        tooltip: "Необходимая t°C теплоносителя после горелки"
        objectName: root.objectName + ".targetTempBr"
        readOnly: false
        visible: true
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        valueText: "45"
        blinkColor: "#ff0000"
        correctingButtons: true
        regexp: floatreg
        onValueChanged: br.targetTempChanged(value)
        checkLimit: true
        downLimit: 20
        upLimit: 150
    }

    Rectangle {
        id: rectangleFill
        x: 354
        y: 109
        width: 324
        height: 345
        color: "#e60000"
        border.color: "#000000"
        z: 0
        border.width: 0
    }

    RegExpValidator {
        id: intreg
        regExp: /[0-9]+/
    }
    Text {
        property int size: 100
        id: flame
        x: 164
        y: 399
        width: 20
        height: 20
        rotation: 90
        text: "i"
        z: 99
        verticalAlignment: Text.AlignVCenter
        font.family: "Guifx v2 Transports"
        font.pixelSize: size / 2
        horizontalAlignment: Text.AlignHCenter
        color: "red"
        opacity: (br.std||br.manual)? 1 : 0
    }

    Fan {
        id: br
        name: "ГОРЕЛКА"
        rotate: -90
        mirror: true
        objectName: root.objectName + ".br"
        mount: false
        function setTemp(value){ elTempIn.text = value + "°" }
        function setAlarmTemp(value)
        {
            if(value) elTempIn.color = "red"
            else elTempIn.color = "#23b4f6"
        }
        function setTargetTemp(value){ targetTempBr.setLableSilent(value) }
        function setOverTemp(value){ overTempBr.setLableSilent(value) }
        function setTempAlarm(){ elTempIn.color = alarmTemp  }
        function resetTempAlarm(){ elTempIn.color = normalTemp }
        function setStep(value){
            if(value)
                flame.font.pixelSize = flame.size
            else
                flame.font.pixelSize = flame.size / 2
        }
        signal targetTempChanged(variant value)
        signal overTempChanged(variant value)
        x: 22
        y: 383
        width: 112
        height: 109
        fontSize: 13
        Button {
            x: -5
            y: 72
            width: 48
            height: 47
            iconSource: ""
            style: ButtonStyle {
                background: Rectangle {

                    color: "#cacaca"
                    radius: 6
                    border.width: 0
                    border.color: "#6c6c6c"
                }
            }
            Image {
                id: image1
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "../clipart276790.png"
            }
            onClicked: parent.openSettings()
        }

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


    Button {
        id: buttonCooling
        x: 9
        y: 144
        width: 195
        height: 60
        Text {
            text: qsTr("ПРОДУВКА")
            font.family: "Verdana"
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 27
        }
        style: ButtonStyle {
            background: Rectangle {
                //color: "#98d1eb"
                color: control.pressed?"#6ae3f7":"#98d1eb"
                radius: 6
                border.width: 3
                border.color: "#386372"
            }
        }
        onClicked: root.cooling()
    }

    Button {
        id: buttonCollingStop
        x: 9
        y: 78
        width: 195
        height: 60
        Text {
            color: "#ffffff"
            text: "СТОП С ПРОДУВКОЙ"
            lineHeight: 0.8
            anchors.fill: parent
            layer.enabled: false
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.bold: true
            font.pixelSize: 23
        }
        style: ButtonStyle {
            background: Rectangle {
                //color: "#852025"
                color: control.pressed?"#6ae3f7":"#852025"
                border.width: 3
                border.color: "#386372"
                radius: 6
            }
        }
        onClicked: root.coolingStop()
    }

    Button {
        id: buttonStart
        x: 9
        y: 12
        width: 195
        height: 60
        Text {
            height: 81
            text: qsTr("ПУСК")
            font.letterSpacing: 0
            font.bold: true
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 27
        }
        style: ButtonStyle {
            background: Rectangle {
                border.width: 3
                border.color: "#386372"
                color: control.pressed?"#6ae3f7":"#31ba2c"
                radius: 6
            }
        }
        onClicked: root.start()
    }


    Transporter{
        id: ut
        name: "ВЫГРУЗНОЙ СТОЛ"
        width: 203

        height: 47
        fontSize: 17
        objectName: root.objectName + ".ut"
        signal speedChanged(variant value)
        x: 579
        y: 465
        borderWidth: 3
        function setSpeed(value){
            utspeed.setLableSilent(value)
        }
        Button {
            x: 214
            y: -1
            width: 48
            height: 47
            iconSource: ""
            style: ButtonStyle {
                background: Rectangle {

                    color: "#cacaca"
                    radius: 6
                    border.width: 0
                    border.color: "#6c6c6c"
                }
            }
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "../clipart276790.png"
            }
            onClicked: parent.openSettings()
        }
    }

    MFUnit{
        id: utspeed
        name: ""
        x: 352
        y: 465
        width: 107
        height: 44
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        fontSize: 25
        backgroundColor: "#38829c"
        tooltip: "Скорость выгрузного стола (%)"
        correctingButtons: true
        readOnly: false
        checkLimit: true
        objectName: root.objectName + ".ut.mf"
        onValueChanged: ut.speedChanged(value)
        borderWidth: 2
        regexp: floatreg
    }



    Machine{
        name: "ЦФ1"
        x: 719
        y: 243
        width: 120
        height: 55
        rotation: 0
        fontSize: 40
        //mount: false
        objectName: root.objectName + ".cf1"
        Button {
            id: button
            x: 79
            y: -44
            width: 48
            height: 47
            style: ButtonStyle {
                background: Rectangle {
                    color: "#cacaca"
                    radius: 6
                    border.color: "#6c6c6c"
                    border.width: 0
                }
            }
            iconSource: ""

            Image {
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.right: parent.right
                anchors.rightMargin: 0
                source: "../clipart276790.png"
                fillMode: Image.PreserveAspectFit
            }
            onClicked: parent.openSettings()
        }
    }

    Machine {
        id: cf2
        name: "ЦФ2"
        width: 120
        height: 55
        borderWidth: 2
        rotation: 0
        fontSize: 40
        //mount: false
        objectName: root.objectName + ".cf2"
        signal speedChanged(variant value)
        x: 720
        y: 377
        function setSpeed(value){
            cf2speed.setLableSilent(value)
        }
        Button {
            x: 79
            y: -45
            width: 48
            height: 47
            style: ButtonStyle {
                background: Rectangle {
                    color: "#cacaca"
                    radius: 6
                    border.color: "#6c6c6c"
                    border.width: 0
                }
            }
            iconSource: ""

            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "../clipart276790.png"
            }
            onClicked: parent.openSettings()

        }
        MFUnit {
            id: cf2speed
            name: ""
            x: -34
            y: -51
            width: 107
            height: 44
            borderWidth: 2
            fontSize: 25
            regexp: floatreg
            blinkColor: "#ff0000"
            readOnly: false
            correctingButtons: true
            visible: true
            valueFontSize.bold: true
            valueFontSize.pointSize: 20
            downLimit: 0
            upLimit: 100
            checkLimit: true
            objectName: root.objectName + ".cf2.mf"
            backgroundColor: "#38829c"
            tooltip: "Скорость вращения циклофена в процентах (от 0 до 100)"
            onValueChanged: cf2.speedChanged(value)
        }
    }
    MFUnit {
        id: overTempBr
        name: ""
        x: 366
        y: 162
        width: 93
        height: 40
        z: 10
        borderWidth: 2
        fontSize: 18
        backgroundColor: "#8ca360"
        tooltip: "t°C перегрева теплоносителя после горелки"
        objectName: root.objectName + ".overTempBr"
        readOnly: false
        visible: true
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        valueText: "45"
        blinkColor: "#ff0000"
        correctingButtons: true
        regexp: floatreg
        onValueChanged: br.overTempChanged(value)
        checkLimit: true
        downLimit: 20
        upLimit: 150
    }
    MFUnit {
        id: overTempHL
        name: ""
        x: 469
        y: 162
        width: 93
        height: 40
        z: 10
        borderWidth: 2
        fontSize: 18
        blinkColor: "#ff0000"
        downLimit: 20
        backgroundColor: "#8ca360"
        visible: true
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        valueText: "45"
        checkLimit: true
        objectName: root.objectName + ".overTempHL"
        tooltip: "t°C перегрева продукта в верхней зоне"
        regexp: floatreg
        readOnly: false
        correctingButtons: true
        onValueChanged: root.overTempHLChanged(value)
    }
    MFUnit {
        id: overTempML
        name: ""
        x: 469
        y: 277
        width: 93
        height: 40
        z: 10
        borderWidth: 2
        fontSize: 18
        backgroundColor: "#8ca360"
        tooltip: "t°C перегрева продукта в середине"
        objectName: root.objectName + ".overTempML"
        readOnly: false
        visible: true
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        valueText: "45"
        blinkColor: "#ff0000"
        correctingButtons: true
        regexp: floatreg
        onValueChanged: root.overTempMLChanged(value)
        checkLimit: true
        downLimit: 20
    }
    MFUnit {
        id: overTempLL
        name: ""
        x: 469
        y: 393
        width: 93
        height: 40
        z: 10
        borderWidth: 2
        fontSize: 18
        backgroundColor: "#8ca360"
        tooltip: "t°C перегрева продукта в зоне охлаждения"
        objectName: root.objectName + ".overTempLL"
        readOnly: false
        visible: true
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        valueText: "45"
        blinkColor: "#ff0000"
        correctingButtons: true
        regexp: floatreg
        onValueChanged: root.overTempLLChanged(value)
        checkLimit: true
        downLimit: 20
    }
    MFUnit {
        id: overTempEX
        name: ""
        x: 574
        y: 162
        width: 93
        height: 40
        z: 10
        borderWidth: 2
        fontSize: 18
        backgroundColor: "#8ca360"
        tooltip: "t°C перегрева теплоносителя на выходе"
        objectName: root.objectName + ".overTempEX"
        readOnly: false
        visible: true
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        valueText: "45"
        blinkColor: "#ff0000"
        regexp: floatreg
        correctingButtons: true
        onValueChanged: root.overTempExChanged(value)
        checkLimit: true
        downLimit: 20
    }

    Button {
        x: 596
        y: 382
        width: 48
        height: 47
        text: ""
        z: 10
        style: ButtonStyle {
            background: Rectangle {
                color: "#cacaca"
                radius: 6
                border.color: "#6c6c6c"
                border.width: 0
            }
        }
        iconSource: ""
        Image {
            z: 10
            anchors.fill: parent
            source: "../clipart276790.png"
            fillMode: Image.PreserveAspectFit
        }
        onClicked: parent.openSettings()
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

    Item {
        id: silo
        x: 416
        y: 31
        width: 38
        height: 72
        z: 5
        Indicator2 {
            id: indicator1
            x: 0
            y: 0
            width: parent.width
            height: parent.height
            multicolor: true
            visible: true
            objectName: root.objectName + ".l1"
        }
        signal addToCurrentRoteNeedUnload()
        signal addToCurrentRoteNeedLoad()
        function activateHL(){
            indicator1.activateHL()
        }
        function deactivateHL(){
            indicator1.deactivateHL()
        }
        function activateML(){
            indicator1.activateML()
        }
        function deactivateML(){
            indicator1.deactivateML()
        }
        function activateLL(){
            indicator1.activateLL()
        }
        function deactivateLL(){
            indicator1.deactivateLL()
        }
        function openSettings(){
            if(siloprop.visible==false){
                var absolutePos = Fap.getAbsolutePosition(root);
                siloprop.setX(absolutePos.x)
                siloprop.setY(absolutePos.y)
                siloprop.show()
            }
            siloprop.requestActivate()
        }

        MouseArea{
            id:mouseAreaSilo
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if( mouse.modifiers & Qt.ShiftModifier ){
                    silo.addToCurrentRoteNeedLoad()
                }else if( mouse.modifiers & Qt.ControlModifier ){
                    silo.addToCurrentRoteNeedUnload()
                }else if( mouse.button & Qt.RightButton ) {
                    if( siloprop.visible==false ){
                        var absolutePos = Fap.getAbsolutePosition( silo );
                        siloprop.setX( absolutePos.x+mouseX )
                        siloprop.setY( absolutePos.y+mouseY )
                        siloprop.show()
                    }
                    siloprop.requestActivate()
                }
            }

            //            {
            //                if(mouse.button & Qt.RightButton) {
            //                    parent.openSettings()
            //                }
            //            }
        }

        PropertyWindow2 {
            id: siloprop
            winTitle: "Свойства бункера"
            buttonsType: 4
            objectName: "prop"
        }
        objectName: root.objectName + ".bun"
        Button {
            x: -54
            y: 0
            width: 48
            height: 47
            iconSource: ""
            style: ButtonStyle {
                background: Rectangle {

                    color: "#cacaca"
                    radius: 6
                    border.width: 0
                    border.color: "#6c6c6c"
                }
            }
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "../clipart276790.png"
            }
            onClicked: parent.openSettings()
        }
    }

    MFUnit {
        id: coolingTemp
        name: "t°C ПРОДУВКИ"
        x: 12
        y: 212
        width: 186
        height: 70
        text: ""
        valueFontSize.bold: true
        valueFontSize.pointSize: 20
        valueText: "15"
        borderWidth: 2
        fontSize: 20
        regexp: floatreg
        backgroundColor: "#98d1eb"
        downLimit: 0
        upLimit: 100
        correctingButtons: true
        tooltip: "Целевая температура охлаждения"
        objectName: root.objectName + ".coolingtemp"
        readOnly: false
        checkLimit: true
        blinkColor: "#ff0000"
        visible: true
        onValueChanged: root.coolingTempChanged(value)
    }

    Image {
        id: image
        height: 530
        z: 1
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 1
        fillMode: Image.PreserveAspectFit
        source: "1br1sec2cf.png"

        Rectangle {
            x: 470
            y: 117
            z: 2
            width: 90
            height: 43
            color: "#80040404"
            radius: 6
            border.width: 0
            border.color: "#000000"
            Text {
                id: elTempHL
                color: "#23b4f6"
                text: "HL"
                anchors.fill: parent
                font.pixelSize: 27
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Rectangle {
            x: 470
            y: 233
            z: 2
            width: 90
            height: 43
            color: "#80040404"
            radius: 6
            border.width: 0
            border.color: "#000000"
            Text {
                id: elTempML
                color: "#23b4f6"
                text: "ML"
                anchors.fill: parent
                font.pixelSize: 27
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Rectangle {
            x: 470
            y: 349
            z: 2
            width: 90
            height: 43
            color: "#80040404"
            radius: 6
            border.width: 0
            border.color: "#000000"
            Text {
                id: elTempLL
                color: "#23b4f6"
                text: "LL"
                anchors.fill: parent
                font.pixelSize: 27
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Rectangle {
            x: 368
            y: 117
            z: 2
            width: 90
            height: 43
            color: "#62040404"
            radius: 6
            border.width: 0
            border.color: "#000000"
            Text {
                id: elTempIn
                color: "#23b4f6"
                text: "In"
                anchors.fill: parent
                font.pixelSize: 27
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }
        Rectangle {
            x: 575
            y: 117
            z: 2
            width: 90
            height: 43
            color: "#62040404"
            radius: 6
            border.width: 0
            border.color: "#000000"
            Text {
                id: elTempEx
                color: "#23b4f6"
                text: "Ex"
                anchors.fill: parent
                font.pixelSize: 27
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

        }
    }

}

/*##^##
Designer {
    D{i:0}D{i:28;locked:true}
}
##^##*/
