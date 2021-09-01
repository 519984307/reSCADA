import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "fap.js" as Fap
Item {
    //еще изменения
    id: root
    width: 342
    height: 460

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
    property real rowOpacity: 0.3

    property alias fillColor: rectangleFill.color
    property color borderColor: "pink"
    property string name: "Сушилка 1"

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

    function setCfLineLimitTempHL(value){
        cflinelimittemp1.setLableSilent(value)
    }
    function setCfLineLimitTempML1(value){
        cflinelimittemp2.setLableSilent(value)
    }
    function setCfLineLimitTempML2(value){
        cflinelimittemp3.setLableSilent(value)
    }
    function setCfLineLimitTempLL(value){
        cflinelimittemp4.setLableSilent(value)
    }

    function setHumIn(value){
        humIn.setLableSilent(value)
    }
    function setHumOut(value){
        humOut.setLableSilent(value)
    }
    function setTempIn(value){
        tempIn.setLableSilent(value)
    }
    function setTempOut(value){
        tempOut.setLableSilent(value)
    }
    signal cfLineLimitTemp1Changed(variant value)
    signal cfLineLimitTemp2Changed(variant value)
    signal cfLineLimitTemp3Changed(variant value)
    signal cfLineLimitTemp4Changed(variant value)

    function setBurnerLimitTemp(value){
        burnerlimittemp.setLableSilent(value)
    }
    signal alarmTempChanged(variant value)

    function setLineLimitTempHL(value){
        templimitHL.setLableSilent(value)
    }
    function setLineLimitTempML1(value){
        templimitML1.setLableSilent(value)
    }
    function setLineLimitTempML2(value){
        templimitML2.setLableSilent(value)
    }
    function setLineLimitTempLL(value){
        templimitLL.setLableSilent(value)
    }
    signal lineLimitTempHLChanged(variant value)
    signal lineLimitTempML1Changed(variant value)
    signal lineLimitTempML2Changed(variant value)
    signal lineLimitTempLLChanged(variant value)

    function setTemp1(value){
        temp1.setLableSilent(value)
    }
    function setTemp2(value){
        temp2.setLableSilent(value)
    }
    function setTemp3(value){
        temp3.setLableSilent(value)
    }
    function setTemp4(value){
        temp4.setLableSilent(value)
    }
    function setTemp5(value){
        temp5.setLableSilent(value)
    }
    function setTemp6(value){
        temp6.setLableSilent(value)
    }
    function setTemp7(value){
        temp7.setLableSilent(value)
    }
    function setTemp8(value){
        temp8.setLableSilent(value)
    }
    function setTemp9(value){
        temp9.setLableSilent(value)
    }
    function setTemp10(value){
        temp10.setLableSilent(value)
    }
    function setTemp11(value){
        temp11.setLableSilent(value)
    }
    function setTemp12(value){
        temp12.setLableSilent(value)
    }
    function setTemp13(value){
        temp13.setLableSilent(value)
    }
    function setTemp14(value){
        temp14.setLableSilent(value)
    }
    function setTemp15(value){
        temp15.setLableSilent(value)
    }
    function setTemp16(value){
        temp16.setLableSilent(value)
    }

    function activateHL1(){
        indicator1.activateHL()
    }
    function deactivateHL1(){
        indicator1.deactivateHL()
    }
    function activateML1(){
        indicator1.activateML()
    }
    function deactivateML1(){
        indicator1.deactivateML()
    }
    function activateLL1(){
        indicator1.activateLL()
    }
    function deactivateLL1(){
        indicator1.deactivateLL()
    }

    function activateHL2(){
        indicator2.activateHL()
    }
    function deactivateHL2(){
        indicator2.deactivateHL()
    }
    function activateML2(){
        indicator2.activateML()
    }
    function deactivateML2(){
        indicator2.deactivateML()
    }
    function activateLL2(){
        indicator2.activateLL()
    }
    function deactivateLL2(){
        indicator2.deactivateLL()
    }

    RegExpValidator {
        id: intreg
        regExp: /[0-9]+/
    }
    RegExpValidator {
        id: floatreg
        regExp: /[0-9]+[.][0-9]/
    }

    MouseArea {
        x: 0
        y: 0
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
        color: root.alarm?Fap.alarm:"gray"
        anchors.bottomMargin: 0
        x: 6
        y: 6
        z: 0
        anchors.fill: parent
        border.width: 5
        border.color: root.borderColor
    }
    Rectangle {
        id: rectangleOpas
        color: "#ffffff"
        anchors.bottomMargin: 0
        x: 6
        y: 6
        anchors.fill: parent
        opacity: 0.2
    }
    //    Canvas {
    //        id: canvas
    //        anchors.rightMargin: -10
    //        anchors.leftMargin: -10
    //        anchors.bottomMargin: -10
    //        anchors.topMargin: -10
    //        anchors.fill: parent
    //        visible: parent.visible

    //        onPaint: {
    //            var context = getContext("2d");
    //            context.clearRect(0,0,width,height);
    //            context.strokeStyle = "black";
    //            context.fillStyle = root.alarm?Fap.alarm:"gray"
    //            context.lineWidth = borderWidth*2;
    //            context.beginPath();
    //            context.moveTo(canvasOffset, height-canvasOffset);
    //            context.lineTo(width-canvasOffset, height-canvasOffset);
    //            context.lineTo(width-canvasOffset, canvasOffset+height/11);
    //            context.lineTo(width-width/3-canvasOffset, canvasOffset);
    //            context.lineTo(canvasOffset+width/3, canvasOffset);
    //            context.lineTo(canvasOffset, canvasOffset+height/11);
    //            context.lineTo(canvasOffset, height-canvasOffset);
    //            context.closePath;
    //            context.stroke();
    //            context.fill();
    //        }

    //    }
    //            Dryier{
    //                anchors.fill: parent
    //            }
    MFUnit {
        id: humIn
        name: ""
        x: 286
        y: 5
        width: 52
        height: 34
        blinkColor: "#ff0000"
        transformOrigin: Item.Center
        mantissa:1
        regexp: floatreg
        valueText: "99.9"
        fontSize: 9
        valueFontSize.family: "DSEG7 Classic"
        valueFontSize.bold: false
        valueFontSize.pointSize: 14
        tooltip: "Входной влагомер"
    }
    MFUnit {
        id: tempIn
        name: ""
        x: 240
        y: 19
        width: 40
        height: 20
        blinkColor: "#ff0000"
        transformOrigin: Item.Center
        mantissa:1
        regexp: floatreg
        text: "99.9"
        valueText: "99.9"
        fontSize: 12
        valueFontSize.family: "DSEG7 Classic"
        valueFontSize.bold: false
        valueFontSize.pointSize: 8
        tooltip: "Входная температура"
    }
    MFUnit {
        id: humOut
        name: ""
        x: 286
        y: 457
        width: 52
        height: 34
        text: "99.9"
        blinkColor: "#ff0000"
        transformOrigin: Item.Center
        mantissa:1
        regexp: floatreg
        valueText: "99.9"
        fontSize: 9
        valueFontSize.family: "DSEG7 Classic"
        valueFontSize.bold: false
        valueFontSize.pointSize: 14
        tooltip: "Выходной влагомер"
    }
    MFUnit {
        id: tempOut
        name: ""
        x: 240
        y: 471
        width: 40
        height: 20
        blinkColor: "#ff0000"
        transformOrigin: Item.Center
        mantissa:1
        regexp: floatreg
        text: "99.9"
        valueText: "99.9"
        fontSize: 12
        valueFontSize.family: "DSEG7 Classic"
        valueFontSize.bold: false
        valueFontSize.pointSize: 8
        tooltip: "Входная температура"
    }
    Column{
        x: 125
        y: 200
        width: 214
        height: 224
        spacing: 0

        Item{
            //id: element1
            width: parent.width
            height: 56
            Rectangle {
                color: "#ffffff"
                anchors.fill: parent
                opacity: root.rowOpacity
            }


            Fan {
                id: cf1
                name: "Ц 1"
                x: 47
                y: 0
                width: 50
                height: 50
                function setTemp(value){
                    cf1t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf1s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp1_2.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf1s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 1"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: -45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf1t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueText: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 1"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: -34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                fontSize: 8
                objectName: root.objectName + ".cf1"
                mount: false
            }

            Fan {
                id: cf2
                name: "Ц 2"
                x: 118
                y: 0
                width: 50
                height: 50
                fontSize: 8
                mount: false
                mirror: true
                function setTemp(value){
                    cf2t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf2s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp1_2.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf2s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 2"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: 45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf2t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 2"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: 34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                objectName: root.objectName + ".cf2"
            }

            MFUnit {
                id: cflinelimittemp1_2
                name: ""
                x: 0
                y: 34
                width: 60
                height: 17
                text: ""
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "Порог аварии t°C по датчикам циклофенов 1-2"
                borderColor: "#000000"
                backgroundColor: "#c2f65f"
                anchors.horizontalCenter: parent.horizontalCenter
                correctingButtons: true
                objectName: root.objectName + ".cflinelimittemp1"
                readOnly: false
                onValueChanged: {
                    root.cfLineLimitTemp1Changed(value)
                    cf1.limitTempChanged(value)
                    cf2.limitTempChanged(value)
                }
                regexp: floatreg
            }



        }

        Item{
            width: parent.width
            height: 56
            Fan {
                id: cf3
                name: "Ц 3"
                x: 47
                y: 0
                width: 50
                height: 50
                function setTemp(value){
                    cf3t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf3s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp3_4.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf3s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 3"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: -45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf3t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 3"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: -34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                fontSize: 8
                objectName: root.objectName + ".cf3"
                mount: false
            }

            Fan {
                id: cf4
                name: "Ц 4"
                x: 118
                y: 3
                width: 50
                height: 50
                fontSize: 8
                mount: false
                mirror: true
                function setTemp(value){
                    cf4t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf4s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp3_4.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf4s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 4"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: 45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf4t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 4"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: 34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                objectName: root.objectName + ".cf4"
            }

            MFUnit {
                id: cflinelimittemp3_4
                name: ""
                x: 0
                y: 34
                width: 60
                height: 17
                text: ""
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "Порог аварии t°C по датчикам циклофенов 3-4"
                borderColor: "#000000"
                backgroundColor: "#c2f65f"
                anchors.horizontalCenter: parent.horizontalCenter
                correctingButtons: true
                objectName: root.objectName + ".cflinelimittemp2"
                readOnly: false
                onValueChanged: {
                    root.cfLineLimitTemp2Changed(value)
                    cf3.limitTempChanged(value)
                    cf4.limitTempChanged(value)
                }
                regexp: floatreg
            }

        }
        Item{
            width: parent.width
            height: 56
            Rectangle {
                color: "#ffffff"
                anchors.fill: parent
                opacity: rowOpacity
            }
            Fan {
                id: cf5
                name: "Ц 5"
                x: 47
                y: 0
                width: 50
                height: 50
                function setTemp(value){
                    cf5t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf5s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp5_6.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf5s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 5"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: -45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf5t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 5"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: -34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                fontSize: 8
                objectName: root.objectName + ".cf5"
                mount: false
            }

            Fan {
                id: cf6
                name: "Ц 6"
                x: 118
                y: 0
                width: 50
                height: 50
                fontSize: 8
                mount: false
                mirror: true
                function setTemp(value){
                    cf6t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf6s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp5_6.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf6s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 6"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: 45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf6t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 6"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: 34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                objectName: root.objectName + ".cf6"
            }

            MFUnit {
                id: cflinelimittemp5_6
                name: ""
                x: 0
                y: 34
                width: 60
                height: 17
                text: ""
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "Порог аварии t°C по датчикам циклофенов 5-6"
                borderColor: "#000000"
                backgroundColor: "#c2f65f"
                anchors.horizontalCenter: parent.horizontalCenter
                correctingButtons: true
                objectName: root.objectName + ".cflinelimittemp3"
                readOnly: false
                onValueChanged: {
                    root.cfLineLimitTemp3Changed(value)
                    cf5.limitTempChanged(value)
                    cf6.limitTempChanged(value)
                }
                regexp: floatreg
            }

        }
        Item{
            width: parent.width
            height: 56
            Fan {
                id: cf7
                name: "Ц 7"
                x: 47
                y: 0
                width: 50
                height: 50
                function setTemp(value){
                    cf7t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf7s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp7_8.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf7s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 7"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: -45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf7t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 7"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: -34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                fontSize: 8
                objectName: root.objectName + ".cf7"
                mount: false
            }

            Fan {
                id: cf8
                name: "Ц 8"
                x: 118
                y: 0
                width: 50
                height: 50
                fontSize: 8
                mount: false
                mirror: true
                function setTemp(value){
                    cf8t.setLableSilent(value)
                }
                function setSpeed(value){
                    cf8s.setLableSilent(value)
                }
                function setLimitTemp(value){
                    cflinelimittemp7_8.setLableSilent(value)
                }
                signal speedChanged(variant value)
                signal tempChanged(variant value)
                signal limitTempChanged(variant value)
                MFUnit {
                    id: cf8s
                    name: ""
                    x: 0
                    y: 16
                    width: parent.width
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "Частота Гц тока на эл.двигателе циклофена 8"
                    backgroundColor: "#5fd5fe"
                    anchors.horizontalCenterOffset: 45
                    anchors.horizontalCenter: parent.horizontalCenter
                    readOnly: false
                    objectName: parent.objectName + ".speed"
                    correctingButtons: true
                    onValueChanged: parent.speedChanged(value)
                    regexp: intreg
                    upLimit: 50
                }

                MFUnit {
                    id: cf8t
                    name: ""
                    x: 0
                    y: 34
                    width: 40
                    height: parent.height / 3
                    text: ""
                    valueFontSize.family: "DSEG7 Classic"
                    borderWidth: 1
                    tooltip: "t°C циклофена 8"
                    backgroundColor: "#afffb4"
                    anchors.horizontalCenterOffset: 34
                    anchors.horizontalCenter: parent.horizontalCenter
                    objectName: parent.objectName + ".temp"
                    correctingButtons: false
                    onValueChanged: parent.tempChanged(value)
                }
                objectName: root.objectName + ".cf8"
            }

            MFUnit {
                id: cflinelimittemp7_8
                name: ""
                x: 0
                y: 34
                width: 60
                height: 17
                text: ""
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "Порог аварии t°C по датчикам циклофенов 7-8"
                borderColor: "#000000"
                backgroundColor: "#c2f65f"
                anchors.horizontalCenter: parent.horizontalCenter
                correctingButtons: true
                objectName: root.objectName + ".cflinelimittemp4"
                readOnly: false
                onValueChanged: {
                    root.cfLineLimitTemp4Changed(value)
                    cf7.limitTempChanged(value)
                    cf8.limitTempChanged(value)
                }
                regexp: floatreg
            }

        }

    }

    Column {
        property  int count : 2
        x: 5
        y: 302
        width: 116
        height: 57

        Button {
            width: 116
            height: parent.height/ parent.count
            text: "ПУСК СУШИЛКИ"
            //tooltip: "Пуск сушки"
            onClicked: root.start()
        }

        Button {
            width: 116
            height: parent.height/ parent.count
            text: "СТОП СУШИЛКИ"
            //tooltip: "Остановка сушки"
            style: ButtonStyle{
                background: Rectangle{
                    color: control.pressed?"#f13816":"red"
                }
            }
            onClicked: {
                root.stop()
            }
        }

    }

    Item {
        id: element
        x: 5
        y: 200
        width: 116
        height: 101
        Fan {
            id: b2
            name: "Г 2"
            y: 0
            width: 44
            height: 38
            objectName: root.objectName + ".b2"
            mount: false
            function setTemp(value){
                b2t.setLableSilent(value)
            }
            function setTargetTemp(value){
                b2st.setLableSilent(value)
            }
            function setBurnerLimitTemp(value){
                burnerlimittemp.setLableSilent(value)
            }
            signal tempChanged(variant value)
            signal setTempChanged(variant value)
            signal alarmTempChanged(variant value)
            x: 59
            Text {
                id: text1
                x: 23
                y: -18
                width: 29
                height: 26
                //visible: (parent.manual || parent.std)
                color: (parent.manual || parent.std) ? "#ff0000" : "#737373"
                text: "i"
                layer.enabled: false
                verticalAlignment: Text.AlignVCenter
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }

            MFUnit {
                id: b2t
                name: ""
                y: 59
                width: 57
                height: 20
                text: ""
                anchors.left: parent.left
                anchors.leftMargin: 0
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "t°C после горелки 2"
                backgroundColor: "#afffb4"
                correctingButtons: false
                objectName: root.objectName + "." + parent.objectName + ".temp"
                onValueChanged: parent.tempChanged(value)
            }

            MFUnit {
                id: b2st
                name: ""
                y: 37
                width: 57
                height: 20
                text: ""
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.rightMargin: -13
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "Заданная t°C после горелки 2"
                backgroundColor: "#ff5d5d"
                upLimit: 150
                readOnly: false
                correctingButtons: true
                objectName: root.objectName + "." + parent.objectName + ".settemp"
                onValueChanged: parent.setTempChanged(value)
                regexp: floatreg
            }
            fontSize: 8
        }

        Fan {
            id: b1
            name: "Г 1"
            x: 0
            y: 0
            width: 44
            height: 38
            objectName: root.objectName + ".b1"
            mount: false
            function setTemp(value){
                b1t.setLableSilent(value)
            }
            function setTargetTemp(value){
                b1st.setLableSilent(value)
            }
            function setBurnerLimitTemp(value){
                burnerlimittemp.setLableSilent(value)
            }
            signal tempChanged(variant value)
            signal setTempChanged(variant value)
            signal alarmTempChanged(variant value)
            Text {
                x: 23
                y: -18
                width: 29
                height: 26
                color: (parent.manual || parent.std) ? "#ff0000" : "#737373"
                text: "i"
                verticalAlignment: Text.AlignVCenter
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }

            MFUnit {
                id: b1t
                name: ""
                y: 59
                width: 57
                height: 20
                text: ""
                anchors.left: parent.left
                anchors.leftMargin: 0
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "t°C после горелки 1"
                backgroundColor: "#afffb4"
                correctingButtons: false
                objectName: root.objectName + "." + parent.objectName + ".temp"
                onValueChanged: parent.tempChanged(value)
            }

            MFUnit {
                id: b1st
                name: ""
                x: 0
                y: 37
                width: 57
                height: 20
                text: ""
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "Заданная t°C после горелки 1"
                backgroundColor: "#ff5d5d"
                upLimit: 150
                readOnly: false
                correctingButtons: true
                objectName: root.objectName + "." + parent.objectName + ".settemp"
                onValueChanged: parent.setTempChanged(value)
                regexp: floatreg
            }
            fontSize: 8
        }

        MFUnit {
            id: burnerlimittemp
            name: ""
            y: 80
            width: parent.width
            height: 20
            text: ""
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            valueFontSize.family: "DSEG7 Classic"
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            borderWidth: 1
            tooltip: "Порог аварии t°C горелок"
            borderColor: "#000000"
            backgroundColor: "#c2f65f"
            upLimit: 200
            readOnly: false
            correctingButtons: true
            objectName: root.objectName + "." + parent.objectName + ".bernerlimittemp"
            onValueChanged: {
                root.alarmTempChanged(value)
                b1.alarmTempChanged(value)
                b2.alarmTempChanged(value)
            }
            regexp: floatreg
        }
    }
    Item {
        id: bun
        objectName: root.objectName + ".bun"
        x: 128
        y: 5
        height: 34
        width: 186
        z: 5
        visible: true
        anchors.horizontalCenterOffset: -70
        anchors.horizontalCenter: parent.horizontalCenter
        Indicator2 {
            id: indicator1
            width: parent.width / 2
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            multicolor: true
            visible: true
            objectName: root.objectName + ".l1"
        }
        Indicator2 {
            id: indicator2
            width: parent.width / 2
            height: parent.height
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            multicolor: true
            visible: true
            objectName: root.objectName + ".l2"
        }
        signal addToCurrentRoteNeedUnload()
        signal addToCurrentRoteNeedLoad()
        function activateHL1(){
            indicator1.activateHL()
        }
        function deactivateHL1(){
            indicator1.deactivateHL()
        }
        function activateML1(){
            indicator1.activateML()
        }
        function deactivateML1(){
            indicator1.deactivateML()
        }
        function activateLL1(){
            indicator1.activateLL()
        }
        function deactivateLL1(){
            indicator1.deactivateLL()
        }
        function activateHL2(){
            indicator2.activateHL()
        }
        function deactivateHL2(){
            indicator2.deactivateHL()
        }
        function activateML2(){
            indicator2.activateML()
        }
        function deactivateML2(){
            indicator2.deactivateML()
        }
        function activateLL2(){
            indicator2.activateLL()
        }
        function deactivateLL2(){
            indicator2.deactivateLL()
        }
        MouseArea{
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if(mouse.modifiers & Qt.ShiftModifier){
                    bun.addToCurrentRoteNeedUnload()
                }else if(mouse.modifiers & Qt.ControlModifier){
                    bun.addToCurrentRoteNeedLoad()
                }else if(mouse.button & Qt.RightButton) {
                    if(bunprop.visible==false){
                        var absolutePos = Fap.getAbsolutePosition(bun);
                        bunprop.setX(absolutePos.x+mouseX)
                        bunprop.setY(absolutePos.y+mouseY)
                        bunprop.show()
                    }
                    bunprop.requestActivate()
                }
            }
        }

        PropertyWindow2 {
            id: bunprop
            winTitle: "Свойства бункера"
            buttonsType: 4
            objectName: "prop"
        }
    }

    Item {
        x: 5
        y: 392
        width: 116
        height: 60
        Transporter{
            id: ut
            name: "Выгрузной стол"
            x: 0
            y: 36
            width: 333
            height: 24
            borderWidth: 2
            objectName: root.objectName + ".ut"
            signal speedChanged(variant value)
            function setSpeed(value){
                ut_speed.setLableSilent(value)
            }
        }

        MFUnit{
            id: ut_speed
            name: "Скорость выгрузки"
            x: 0
            y: 0
            width: 119
            height: parent.height / 2
            text: ""
            valueText: "50"
            valueFontSize.family: "DSEG7 Classic"
            borderWidth: 1
            fontSize: 10
            tooltip: "Скорость колебаний выгрузного стола"
            backgroundColor: "#5fd5fe"
            readOnly: false
            correctingButtons: true
            objectName: root.objectName + ".ut.mf"
            onValueChanged: ut.speedChanged(value)
            regexp: intreg
            upLimit: 50
        }
    }
    Column {
        id: column1
        y: 42
        width: 579
        height: 42
        spacing: 2
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4


        Row {
            width: parent.width
            height: 20
            spacing: 58

            MFUnit {
                id: temp1
                name: ""
                width: 40
                height: parent.height
                text: ""
                valueText: "99"
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "1 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp1"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp2
                name: ""
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "2 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp2"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp3
                name: ""
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "3 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp3"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp4
                name: ""
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "4 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp4"
                blinkColor: "#ff0000"
            }
        }

        MFUnit {
            id: templimitHL
            name: ""
            width: 80
            height: 20
            valueFontSize.family: "DSEG7 Classic"
            borderWidth: 1
            tooltip: "Порог аварии t°C по датчикам 1-4"
            borderColor: "#000000"
            backgroundColor: "#c2f65f"
            anchors.horizontalCenter: parent.horizontalCenter
            readOnly: false
            objectName: root.objectName + "templimitHL"
            correctingButtons: true
            onValueChanged: root.lineLimitTempHLChanged(value)
            regexp: floatreg
        }
    }
    Column {
        id: column2
        width: 579
        height: 42
        spacing: 2
        anchors.top: column1.bottom
        anchors.topMargin: -6
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4

        Row {
            y: 0
            width: 240
            height: 20
            anchors.horizontalCenter: parent.horizontalCenter
            MFUnit {
                id: temp5
                name: ""
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "5 t°C"
                backgroundColor: "#afffb4"
                blinkColor: "#ff0000"
                objectName: root.objectName + ".temp5"
            }

            MFUnit {
                id: temp6
                name: ""
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "6 t°C"
                backgroundColor: "#afffb4"
                blinkColor: "#ff0000"
                objectName: root.objectName + ".temp6"
            }
            spacing: 160
        }

        MFUnit {
            id: templimitML1
            name: ""
            width: 80
            height: 20
            valueFontSize.family: "DSEG7 Classic"
            borderWidth: 1
            tooltip: "Порог аварии t°C по датчикам 5-6"
            borderColor: "#000000"
            backgroundColor: "#c2f65f"
            anchors.horizontalCenter: parent.horizontalCenter
            readOnly: false
            correctingButtons: true
            objectName: root.objectName + "templimitML1"
            onValueChanged: root.lineLimitTempML1Changed(value)
            regexp: floatreg
        }
    }

    Column {
        id: column4
        height: 42
        anchors.top: column3.bottom
        anchors.topMargin: -6
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4

        Row {
            width: 240
            height: 20
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 160
            MFUnit {
                id: temp15
                name: ""
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "15 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp15"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp16
                name: ""
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "16 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp16"
                blinkColor: "#ff0000"
            }
        }

        MFUnit {
            id: templimitLL
            name: ""
            width: 80
            height: 20
            valueFontSize.family: "DSEG7 Classic"
            borderWidth: 1
            tooltip: "Порог аварии t°C по датчикам 15-16"
            borderColor: "#000000"
            backgroundColor: "#c2f65f"
            anchors.horizontalCenter: parent.horizontalCenter
            readOnly: false
            objectName: root.objectName + "templimitLL"
            correctingButtons: true
            onValueChanged: root.lineLimitTempLLChanged(value)
            regexp: floatreg
        }
    }

    Column {
        id: column3
        width: 579
        height: 42
        anchors.top: column2.bottom
        anchors.topMargin: 2
        spacing: 2
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4

        Row {
            width: parent.width
            height: 20
            spacing: 2
            MFUnit {
                id: temp7
                name: ""
                x: 4
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "7 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp7"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp8
                name: ""
                x: 44
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "8 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp8"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp9
                name: ""
                x: 84
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "9 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp9"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp10
                name: ""
                x: 124
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "10 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp10"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp11
                name: ""
                x: 164
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "11 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp11"
                regexp: floatreg
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp12
                name: ""
                x: 204
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "12 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp12"
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp13
                name: ""
                x: 244
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "13 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp13"
                regexp: floatreg
                blinkColor: "#ff0000"
            }

            MFUnit {
                id: temp14
                name: ""
                x: 284
                width: 40
                height: parent.height
                valueFontSize.family: "DSEG7 Classic"
                borderWidth: 1
                tooltip: "14 t°C"
                backgroundColor: "#afffb4"
                objectName: root.objectName + ".temp14"
                blinkColor: "#ff0000"
            }
        }

        MFUnit {
            id: templimitML2
            name: ""
            width: 80
            height: 20
            valueFontSize.family: "DSEG7 Classic"
            borderWidth: 1
            tooltip: "Порог авариt°C и по датчикам 7-14"
            borderColor: "#000000"
            backgroundColor: "#c2f65f"
            anchors.horizontalCenter: parent.horizontalCenter
            readOnly: false
            objectName: root.objectName + "templimitML2"
            correctingButtons: true
            onValueChanged: root.lineLimitTempML2Changed(value)
            regexp: floatreg
        }
    }

    PropertyWindow2 {
        id: prop
        width: 370
        objectName: "prop"
        winTitle: "Свойства "+name
        onStart: root.start()
        onStop: root.stop()
        buttonsType: 4
    }





}


/*##^##
Designer {
    D{i:0;height:445;width:342}
}
##^##*/
