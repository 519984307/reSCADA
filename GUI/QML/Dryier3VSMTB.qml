import QtQuick 2.12
import QtQuick.Controls 2.12
//import ETag 1.0
//import ElevatitUnits 1.0
import "fap.js" as Fap

Item {
    id: root
    width: 490
    height: 570
    property alias numb: lableNumb.text
    property int borderWidth: 2
    //property alias name: lable.text
    property alias alarm: prop.alarm
    property bool humidity: true
    property bool _EBberner: true

    property bool st: false
    property bool std: false
    //property alias alarm: prop.alarm
    property alias alarmNotify: prop.alarmNotify
    //property bool alarmBlink: false
    property bool blocked: false
    property bool available: prop.available
    property bool linked: prop.linked
    property bool manual: false
    property bool levelReverse: false
    property bool cleaning: false
    //property QtObject promCoreLinc
    //property Dryer3VSM_Kromy dryer
    //property InETag tagTemp1

    //        Connections{
    //            target: dr2
    //            onS_maxBernerTempChd:{
    //                maxBrnTemp.setLableSilent( value )
    //            }
    //        }
    //Connections{
    //    tar
    //    dryer.onS_maxBernerTempChd: maxBrnTemp.setLableSilent( value )
    //    maxBrnTemp.valueChanged: dryer.setmaxBernerTemp(value)
    //}
    //    Connections{
    //        target: dryer
    //        onS_maxBernerTempChd:{
    //            maxBrnTemp.setLableSilent( value )
    //        }
    //    }
    property color borderColor: "pink"

    onAlarmChanged:       { Fap.changeColor(root) }
    onAlarmNotifyChanged: Fap.changeColor(root)
    //    onAlarmBlinkChanged:  Fap.changeColor(root)
    onAvailableChanged:   Fap.changeColor(root)

    function started(){
        startComand()
        std=true
        manual=false
        cleaning=false
        Fap.changeColor(root)
        //dryer.brnrTempProdTempPID
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

    signal start()
    signal stop()
    signal cooling()
    signal coolingStop()

    signal changedUnload(bool status)
    signal changedCircule(bool status)
    signal changedLoad(bool status)

    signal passUnload(bool status)
    signal passCircule(bool status)
    signal passLoad(bool status)

    function setUnload(status) {
        buttonUnload.checked = status
    }
    function setCircule(status) {
        buttonCirc.checked = status
    }
    function setLoad(status) {
        buttonLoad.checked = status
    }

    function setStatus(value) {
        statusBar.setLableSilent(value)
    }

    function setTemp1(value) {
        temp1.setLableSilent(value)
    }
    function setTemp2(value) {
        temp2.setLableSilent(value)
    }
    function setTemp3(value) {
        temp3.setLableSilent(value)
    }
    function setTemp4(value) {
        temp4.setLableSilent(value)
    }
    function setTemp5(value) {
        temp5.setLableSilent(value)
    }
    function setTemp6(value) {
        temp6.setLableSilent(value)
    }
    function setTemp7(value) {
        temp7.setLableSilent(value)
    }
    function setTemp8(value) {
        temp8.setLableSilent(value)
    }
    function setTemp9(value) {
        temp9.setLableSilent(value)
    }
    function setTemp10(value) {
        temp10.setLableSilent(value)
    }
    function setTemp11(value) {
        temp11.setLableSilent(value)
    }
    function setTemp12(value) {
        temp12.setLableSilent(value)
    }
    function setTemp13(value) {
        temp13.setLableSilent(value)
    }
    function setTemp14(value) {
        temp14.setLableSilent(value)
    }
    function setTemp15(value) {
        temp15.setLableSilent(value)
    }

    function setAlarmTemp1(value) {
        alarmTemp1.setLableSilent(value)
    }
    function setAlarmTemp2(value) {
        alarmTemp2.setLableSilent(value)
    }
    function setAlarmTemp3(value) {
        alarmTemp3.setLableSilent(value)
    }
    function setAlarmTemp4(value) {
        alarmTemp4.setLableSilent(value)
    }
    function setAlarmTemp5(value) {
        alarmTemp5.setLableSilent(value)
    }
    function setAlarmTemp6(value) {
        alarmTemp6.setLableSilent(value)
    }
    function setAlarmTemp7(value) {
        alarmTemp7.setLableSilent(value)
    }
    function setAlarmTemp8(value) {
        alarmTemp8.setLableSilent(value)
    }
    function setAlarmTemp9(value) {
        alarmTemp9.setLableSilent(value)
    }
    function setAlarmTemp10(value) {
        alarmTemp10.setLableSilent(value)
    }
    function setAlarmTemp11(value) {
        alarmTemp11.setLableSilent(value)
    }
    function setAlarmTemp12(value) {
        alarmTemp12.setLableSilent(value)
    }
    function setAlarmTemp13(value) {
        alarmTemp13.setLableSilent(value)
    }
    function setAlarmTemp14(value) {
        alarmTemp14.setLableSilent(value)
    }
    function setAlarmTemp15(value) {
        alarmTemp15.setLableSilent(value)
    }

    signal alarmTemp1Changed(variant value)
    signal alarmTemp2Changed(variant value)
    signal alarmTemp3Changed(variant value)
    signal alarmTemp4Changed(variant value)
    signal alarmTemp5Changed(variant value)
    signal alarmTemp6Changed(variant value)
    signal alarmTemp7Changed(variant value)
    signal alarmTemp8Changed(variant value)
    signal alarmTemp9Changed(variant value)
    signal alarmTemp10Changed(variant value)
    signal alarmTemp11Changed(variant value)
    signal alarmTemp12Changed(variant value)
    signal alarmTemp13Changed(variant value)
    signal alarmTemp14Changed(variant value)
    signal alarmTemp15Changed(variant value)

    function setHrIn(value) {
        hrIn.setLableSilent(value)
    }
    function setHrOut(value) {
        hrOut.setLableSilent(value)
    }

    function setTargetHumidity(value) {
        targetHumidity.setLableSilent(value)
    }
    signal s_targetHumidityChd(variant value)

    function setTargetTempLow(value) {
        targetTempLow.setLableSilent(value)
    }
    signal s_targetTempLowChd(variant value)

    function setBurnersTargetTemp(value) {
        targetTemp16.setLableSilent(value)
    }
    function setBurnersAlarmTemp(value) {
        brsAlarmTemp.setLableSilent(value)
    }




    signal s_maxBernerTempChd(variant value);
    function setMaxBernerTemp(value){ maxBrnTemp.setLableSilent(value)}
    signal s_maxProductTempChd(variant value);
    function setMaxProductTemp(value){ maxProductTemp.setLableSilent(value) }
    signal s_maxUTSpeedChd(variant value);
    function setMaxUTSpeed(value){ maxUTSpeed.setLableSilent(value) }

    signal s_brnrTempProdTempPID_KpChd  (variant value)
    signal s_brnrTempProdTempPID_KiChd  (variant value)
    signal s_brnrTempProdTempPID_KdChd  (variant value)
    signal s_brnrTempProdTempPID_TdChd  (variant value)
    signal s_brnrTempProdTempPID_MaxUChd(variant value)
    signal s_brnrTempProdTempPID_MinUChd(variant value)

    signal s_prodTempHumPID_KpChd       (variant value)
    signal s_prodTempHumPID_KiChd       (variant value)
    signal s_prodTempHumPID_KdChd       (variant value)
    signal s_prodTempHumPID_TdChd       (variant value)
    signal s_prodTempHumPID_MaxUChd     (variant value)
    signal s_prodTempHumPID_MinUChd     (variant value)

    signal s_UTSpeedHumPID_KpChd        (variant value)
    signal s_UTSpeedHumPID_KiChd        (variant value)
    signal s_UTSpeedHumPID_KdChd        (variant value)
    signal s_UTSpeedHumPID_TdChd        (variant value)
    signal s_UTSpeedHumPID_MaxUChd      (variant value)
    signal s_UTSpeedHumPID_MinUChd      (variant value)

    function setBrnrTempProdTempPID_Kp  (value){ brnrTempProdTempPID_Kp.text = value}
    function setBrnrTempProdTempPID_Ki  (value){ brnrTempProdTempPID_Ki.text = value}
    function setBrnrTempProdTempPID_Kd  (value){ brnrTempProdTempPID_Kd.text = value}
    function setBrnrTempProdTempPID_Td  (value){ brnrTempProdTempPID_Td.text = value}
    //function setBrnrTempProdTempPID_MaxU(value){ brnrTempProdTempPID_MaxU.setLableSilent(value)}
    //function setBrnrTempProdTempPID_MinU(value){ brnrTempProdTempPID_MinU.setLableSilent(value)}

    function setProdTempHumPID_Kp       (value){ prodTempHumPID_Kp.text = value}
    function setProdTempHumPID_Ki       (value){ prodTempHumPID_Ki.text = value}
    function setProdTempHumPID_Kd       (value){ prodTempHumPID_Kd.text = value}
    function setProdTempHumPID_Td       (value){ prodTempHumPID_Td.text = value}
    //function setProdTempHumPID_MaxU     (value){ prodTempHumPID_MaxU      .setLableSilent(value)}
    //function setProdTempHumPID_MinU     (value){ prodTempHumPID_MinU      .setLableSilent(value)}

    function setUTSpeedHumPID_Kp        (value){ utSpeedHumPID_Kp.text = value}
    function setUTSpeedHumPID_Ki        (value){ utSpeedHumPID_Ki.text = value}
    function setUTSpeedHumPID_Kd        (value){ utSpeedHumPID_Kd.text = value}
    function setUTSpeedHumPID_Td        (value){ utSpeedHumPID_Td.text = value}
    //function setUTSpeedHumPID_MaxU      (value){ utSpeedHumPID_MaxU       .setLableSilent(value)}
    //function setUTSpeedHumPID_MinU      (value){ utSpeedHumPID_MinU       .setLableSilent(value)}

    signal s_autoHumidityChd(bool value);
    function setAutoHumidity(value){ checkBoxAutoHum.checked = value }

    signal s_autoTempChd(bool value);
    function setAutoTemp(value){ checkBoxAutoTemp.checked = value }

    Rectangle {
        id: rectangleFill
        x: 6
        y: 6
        color: "gray"
        anchors.fill: parent
        border.width: 5
        border.color: root.alarm?Fap.alarm:"gray"
        radius: 20
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: {
            if (prop.visible == false) {
                var absolutePos = Fap.getAbsolutePosition(root)
                prop.setX(absolutePos.x + mouseX)
                prop.setY(absolutePos.y + mouseY)
                prop.show()
            }
        }
    }


    MFUnit {
        id: statusBar

        y: 76
        height: 31
        text: "Готовность"
        anchors.left: parent.left
        anchors.right: parent.right
        valueFontSize.pointSize: 14
        anchors.rightMargin: 9
        anchors.leftMargin: 9
        visible: true
        fontSize: 17
    }

    RegExpValidator {
        id: intreg
        regExp: /[0-9]+/
    }

    RegExpValidator {
        id: floatreg
        regExp: /[0-9]+[.][0-9]/
    }


    Column {
        id: clProd
        x: 325
        y: 8
        width: 93
        height: 62
        spacing: 2

        Button {
            id: buttonLoad
            width: 94
            height: 19
            text: "Авто загрузка"
            font.letterSpacing: 0.9
            font.bold: false
            font.pointSize: 7
            font.capitalization: Font.AllUppercase
            background: Rectangle {
                color: parent.checked ? "blue" : "lightblue"
                radius: 3
                border.color: "#000000"
                border.width: 1
            }

            checkable: true
            onClicked: root.passLoad(checked)
            //        Text {
            //            height: 20
            //            color: parent.checked ? "white" : "black"
            //            text: "Авто загрузка"
            //            horizontalAlignment: Text.AlignHCenter
            //            verticalAlignment: Text.AlignVCenter
            //            wrapMode: Text.WordWrap
            //            anchors.fill: parent
            //        }
        }

        Button {
            id: buttonCirc
            width: 94
            height: 19
            text: "Циркуляция"
            font.letterSpacing: 0.9
            font.bold: false
            font.pointSize: 7
            font.capitalization: Font.AllUppercase
            background: Rectangle {
                color: parent.checked ? "blue" : "lightblue"
                radius: 3
                border.color: "#000000"
                border.width: 1
            }
            checkable: true
            onClicked: root.passCircule(checked)
            //        Text {
            //            height: 20
            //            color: parent.checked ? "white" : "black"
            //            text:
            //            horizontalAlignment: Text.AlignHCenter
            //            verticalAlignment: Text.AlignVCenter
            //            wrapMode: Text.WordWrap
            //            anchors.fill: parent
            //        }
        }

        Button {
            id: buttonUnload
            width: 94
            height: 19
            text: "Разгрузка"
            font.letterSpacing: 0.9
            font.bold: false
            font.pointSize: 7
            font.capitalization: Font.AllUppercase
            background: Rectangle {
                color: parent.checked ? "blue" : "lightblue"
                radius: 3
                border.color: "#000000"
                border.width: 1
            }
            checkable: true
            onClicked: root.passUnload(checked)
            //        Text {
            //            height: 20
            //            color: parent.checked ? "white" : "black"
            //            text: "Разгрузка"
            //            horizontalAlignment: Text.AlignHCenter
            //            verticalAlignment: Text.AlignVCenter
            //            wrapMode: Text.WordWrap
            //            anchors.fill: parent
            //        }
        }
    }

    Text {
        id: lableNumb
        x: 424
        y: 8
        width: 57
        height: 62
        text: "1"
        font.pixelSize: 60
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.bold: true
    }



    Column {
        id: clBurners
        width: 152
        height: 168
        anchors.left: parent.left
        anchors.top: clBr.bottom
        anchors.leftMargin: 5
        anchors.topMargin: 5
        spacing: 0
        Fan {
            id: b1
            name: "Г1"
            y: 43
            width: 56
            height: 55
            anchors.left: parent.left
            rotate: 90
            anchors.leftMargin: 0
            objectName: root.objectName + ".b1"
            mount: false
            function setTemp(value) {
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
                x: 50
                y: 3
                width: 48
                height: 45
                color: parent.std ? "#ff0000" : "#737373"
                text: "i"
                verticalAlignment: Text.AlignVCenter
                layer.textureMirroring: ShaderEffectSource.NoMirroring
                layer.enabled: true
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }

            Machine {
                id: comb1
                name: "ТБ1"
                x: 97
                y: 18
                width: 50
                height: 30
                anchors.right: parent.right
                anchors.rightMargin: -89
                objectName: root.objectName + ".b1.comb1"
                visible: root._EBberner
            }
            fontSize: 8
        }

        Fan {
            id: b2
            name: "Г2"
            y: 43
            width: 56
            height: 55
            anchors.left: parent.left
            rotate: 90
            anchors.leftMargin: 0
            objectName: root.objectName + ".b2"
            mount: false
            function setTemp(value) {
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
                x: 50
                y: 3
                width: 48
                height: 45
                color: parent.std ? "#ff0000" : "#737373"
                text: "i"
                verticalAlignment: Text.AlignVCenter
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }

            Machine {
                id: comb2
                name: "ТБ2"
                x: 97
                y: 18
                width: 50
                height: 30
                anchors.rightMargin: 0
                objectName: root.objectName + ".b2.comb2"
                visible: root._EBberner
            }
            fontSize: 8
        }

        Fan {
            id: b3
            name: "Г3"
            y: 43
            width: 56
            height: 55
            anchors.left: parent.left
            rotate: 90
            anchors.leftMargin: 0
            objectName: root.objectName + ".b3"
            mount: false
            function setTemp(value) {
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
                x: 50
                y: 3
                width: 48
                height: 45
                color: parent.std ? "#ff0000" : "#737373"
                text: "i"
                verticalAlignment: Text.AlignVCenter
                rotation: 0.806
                font.family: "Guifx v2 Transports"
                font.pixelSize: 60
                horizontalAlignment: Text.AlignHCenter
            }

            Machine {
                id: comb3
                name: "ТБ3"
                x: 97
                y: 18
                width: 50
                height: 30
                anchors.rightMargin: 0
                objectName: root.objectName + ".b3.comb3"
                visible: root._EBberner
            }
            fontSize: 8
        }
    }


    Column {
        id: clFans
        width: 157
        height: 203
        anchors.left: clProdTemp.right
        anchors.top: clExt.bottom
        anchors.leftMargin: 5
        anchors.topMargin: -3
        spacing: -5

        Row {
            spacing: -5
            width: 152
            height: 55
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: -5
            Fan {
                name: "ЦФ1"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf1"
            }

            Fan {
                name: "ЦФ2"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf2"
            }

            Fan {
                name: "ЦФ3"
                x: 100
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf3"
            }
        }

        Row {
            spacing: -5
            width: 152
            height: 55
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: -5
            Fan {
                name: "ЦФ4"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf4"
            }

            Fan {
                name: "ЦФ5"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf5"
            }

            Fan {
                name: "ЦФ6"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf6"
            }
        }

        Row {
            spacing: -5
            width: 152
            height: 55
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: -5
            Fan {
                name: "ЦФ7"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf7"
            }

            Fan {
                name: "ЦФ8"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf8"
            }

            Fan {
                name: "ЦФ9"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf9"
            }
        }

        Row {
            spacing: -5
            width: 152
            height: 55
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: -5
            Fan {
                name: "ЦФ10"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf10"
            }

            Fan {
                name: "ЦФ11"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf11"
            }

            Fan {
                name: "ЦФ12"
                width: 56
                height: 55
                rotate: 90
                fontSize: 8
                mount: false
                objectName: root.objectName + ".cf12"
            }
        }
    }

    Row {
        id: rowSilo
        x: 160
        y: 8
        height: 62
        spacing: 0
        width: 160
        objectName: root.objectName + ".bun"

        signal addToCurrentRoteFull
        signal addToCurrentRoteEmpty

        function activateHL1() {
            indicator1.activateHL()
        }
        function deactivateHL1() {
            indicator1.deactivateHL()
        }
        function activateML1() {
            indicator1.activateML()
        }
        function deactivateML1() {
            indicator1.deactivateML()
        }
        function activateLL1() {
            indicator1.activateLL()
        }
        function deactivateLL1() {
            indicator1.deactivateLL()
        }

        function activateHL2() {
            indicator2.activateHL()
        }
        function deactivateHL2() {
            indicator2.deactivateHL()
        }
        function activateML2() {
            indicator2.activateML()
        }
        function deactivateML2() {
            indicator2.deactivateML()
        }
        function activateLL2() {
            indicator2.activateLL()
        }
        function deactivateLL2() {
            indicator2.deactivateLL()
        }

        function activateHL3() {
            indicator3.activateHL()
        }
        function deactivateHL3() {
            indicator3.deactivateHL()
        }
        function activateML3() {
            indicator3.activateML()
        }
        function deactivateML3() {
            indicator3.deactivateML()
        }
        function activateLL3() {
            indicator3.activateLL()
        }
        function deactivateLL3() {
            indicator3.deactivateLL()
        }

        Indicator2 {
            id: indicator1
            width: parent.width / 3
            height: 62
            multicolor: true
            visible: true
            objectName: root.objectName + ".l1"
        }
        Indicator2 {
            id: indicator2
            x: 50
            y: 0
            width: parent.width / 3
            height: 62
            multicolor: true
            visible: true
            objectName: root.objectName + ".l2"
        }
        Indicator2 {
            id: indicator3
            x: 100
            y: 0
            width: parent.width / 3
            height: 62
            multicolor: true
            visible: true
            objectName: root.objectName + ".l3"
        }
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.modifiers & Qt.ShiftModifier) {
                    rowSilo.addToCurrentRoteEmpty()
                } else if (mouse.modifiers & Qt.ControlModifier) {
                    rowSilo.addToCurrentRoteFull()
                } else if (mouse.button & Qt.RightButton) {
                    if (siloprop.visible == false) {
                        var absolutePos = Fap.getAbsolutePosition(rowSilo)
                        siloprop.setX(absolutePos.x + mouseX)
                        siloprop.setY(absolutePos.y + mouseY)
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
            onExNameChanged: {
                textField.text = siloprop.exName
            }
            buttonsType: 4
        }
    }





    Row {
        id: rowUt
        y: 383
        height: 35
        anchors.left: clProdTemp.left
        anchors.right: parent.right
        anchors.leftMargin: 0
        anchors.rightMargin: 9
        Transporter {
            id: ut
            name: "ВЫГРУЗНОЙ СТОЛ"
            y: 2
            width: 180
            height: parent.height - 4
            anchors.left: parent.left
            fontSize: 12
            anchors.leftMargin: 0
            objectName: root.objectName + ".ut"
            signal speedChanged(variant value)
            function setSpeed(value) {
                utSpeed.setLableSilent(value)
            }
        }

        MFUnit {
            id: utSpeed
            name: qsTr("СКОРОСТЬ ВЫГРУЗКИ")
            y: 0
            height: parent.height
            text: "12"
            anchors.left: ut.right
            anchors.right: parent.right
            valueFontSize.family: "DSEG7 Classic"
            valueFontSize.bold: false
            valueFontSize.pointSize: 12
            upLimit: 50
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            fontSize: 12
            backgroundColor: "#5fd5fe"
            tooltip: "Частота ПЧ выгрузного стола в Гц (от 1 до 50))"
            correctingButtons: true
            readOnly: false
            checkLimit: true
            objectName: root.objectName + ".utSpeed.mf"
            onValueChanged: ut.speedChanged(value)
        }
    }


    Row {
        id:contrlPan
        property int count: 3
        anchors.left: rowUt.left
        anchors.right: statusBar.right
        anchors.top: rowUt.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 9
        spacing: 4
        anchors.leftMargin: 0
        anchors.topMargin: 5
        anchors.rightMargin: 0
        Column {
            id: column3
            height: parent.height
            anchors.left: parent.left
            anchors.right: parent.right
            spacing: 4
            anchors.leftMargin: 0
            anchors.rightMargin: 0

            Row {
                id: rowCtrl
                width: parent.width
                height: 25
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                spacing: 2
                property int count: 2

                Button {
                    id: btnStart
                    height: parent.height
                    text: "ПУСК"
                    font.pointSize: 8
                    font.bold: true
                    onClicked: root.start()
                }
                Button {
                    id: btnColling
                    height: parent.height
                    text: "ПРОДУВКА"
                    anchors.left: btnStart.right
                    anchors.right: btnCollingStop.left
                    anchors.leftMargin: 2
                    anchors.rightMargin: 2
                    font.pointSize: 8
                    font.bold: true
                    onClicked: root.cooling()
                }
                Button {
                    id:btnCollingStop
                    height: 25
                    text: "ПРОДУВКА И СТОП"
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    font.pointSize: 8
                    font.bold: true
                    font.capitalization: Font.AllUppercase
                    onClicked: root.coolingStop()
                }
            }
            //            Item{
            //                id: autoParam
            //                anchors.left: parent.left
            //                anchors.right: parent.right
            //                anchors.top: column1.bottom
            //                anchors.bottom: parent.bottom
            //                anchors.bottomMargin: 0
            //                anchors.rightMargin: 0
            //                anchors.leftMargin: 0
            //                //contentWidth: 50
            //                //contentHeight: 80
            //                anchors.topMargin: 0
            //                //font.pointSize: 10
            //                //title: "Автоматическая сушка"

            Column {
                anchors.left: parent.left
                anchors.right: parent.right
                //anchors.top: column1.bottom
                anchors.topMargin: 0
                spacing: 1
                anchors.leftMargin: 0
                anchors.rightMargin: 0
                Row {
                    id: rowMaxBrnTemp
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text10
                        width: 150
                        text: qsTr("МАКС. t°C ГОРЕЛОК")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.MixedCase

                    }
                    MFUnit {
                        id: maxBrnTemp
                        onValueChanged: root.s_maxBernerTempChd(value)
                        visible: true
                        anchors.left: text10.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        valueFontSize.bold: false
                        valueFontSize.family: "DSEG7 Classic"
                        valueFontSize.pointSize: 8
                        anchors.topMargin: 0
                        backgroundColor: "#fdc69f"
                        downLimit: 20
                        upLimit: 150
                        anchors.rightMargin: 0
                        checkLimit: true
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        tooltip: "Макс. t°C горелок"
                        readOnly: false
                        regexp: floatreg
                        correctingButtons: true
                        blinkColor: "#ff0000"
                        mantissa:1
                        //                        Connections{
                        //                            target: dryer
                        //                            onS_maxBernerTempChd:{
                        //                                maxBrnTemp.setLableSilent( value )
                        //                            }
                        //                        }
                    }
                }
                Row {
                    id: row6
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    visible: root.humidity
                    Text {
                        id: textProdTemp
                        width: 150
                        text: "МАКС. t°C ПРОДУКТА"
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        fontSizeMode: Text.HorizontalFit
                        font.capitalization: Font.MixedCase
                    }
                    MFUnit {
                        id: maxProductTemp
                        onValueChanged: root.s_maxProductTempChd(value)
                        visible: true
                        anchors.left: textProdTemp.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        valueFontSize.bold: false
                        valueFontSize.family: "DSEG7 Classic"
                        valueFontSize.pointSize: 8
                        anchors.topMargin: 0
                        backgroundColor: "#fbeb5d"
                        downLimit: 0
                        upLimit: 100
                        anchors.rightMargin: 0
                        checkLimit: true
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        tooltip: "Макс. t°C продукта"
                        readOnly: false
                        correctingButtons: true
                        blinkColor: "#ff0000"
                        mantissa:1
                        regexp: floatreg
                    }
                }

                Row {
                    id: row7
                    width: 60
                    height: 20
                    visible: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text11
                        width: 120
                        text: qsTr(" Макс. скор. выгр-ки")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 10
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase
                    }
                    MFUnit {
                        id: maxUTSpeed
                        onValueChanged: root.s_maxUTSpeedChd(value)

                        visible: true
                        anchors.left: text11.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        valueFontSize.pointSize: 10
                        anchors.topMargin: 0
                        backgroundColor: "#5fd5fe"
                        downLimit: 1
                        upLimit: 50
                        anchors.rightMargin: 0
                        checkLimit: true
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        readOnly: false
                        regexp: floatreg
                        correctingButtons: true
                        blinkColor: "#ff0000"
                    }
                }

                Row {
                    id: row8
                    width: 60
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 0
                    visible: root.humidity
                    Text {
                        id: text12
                        width: 150
                        text: qsTr("Целевая влажность")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 11
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        font.capitalization: Font.AllUppercase

                    }

                    MFUnit {
                        id: targetHumidity
                        onValueChanged: root.s_targetHumidityChd(value)

                        visible: true
                        anchors.left: text12.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        valueFontSize.bold: false
                        valueFontSize.family: "DSEG7 Classic"
                        valueFontSize.pointSize: 8
                        anchors.topMargin: 0
                        backgroundColor: "#ffffff"
                        downLimit: 1
                        upLimit: 50
                        anchors.rightMargin: 0
                        checkLimit: true
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        readOnly: false
                        regexp: floatreg
                        correctingButtons: true
                        blinkColor: "#ff0000"
                        mantissa:1
                        tooltip: "Целевая влажность"

                    }
                    anchors.rightMargin: 0
                }
            }
            Row{
                id: rowAuto

                CheckBox {
                    id: checkBoxAutoTemp
                    width: 40
                    height: 40
                    onCheckStateChanged:
                        root.s_autoTempChd(checkState == Qt.Checked)
                }
                Label {
                    width: 120
                    text: "автоматическое поддержание t°C продукта"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                    font.bold: true
                    anchors.bottomMargin: 0
                    font.pointSize: 9
                    anchors.topMargin: 0
                }
                CheckBox {
                    id: checkBoxAutoHum
                    width: 40
                    height: 40
                    visible: root.humidity
                    onCheckStateChanged:
                        root.s_autoHumidityChd(checkState == Qt.Checked)
                }
                Label{
                    width: 105
                    text: "автоматическое поддержание влажности"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.Wrap
                    clip: false
                    font.bold: true
                    font.pointSize: 9
                    anchors.bottomMargin: 0
                    anchors.topMargin: 0
                    visible: root.humidity
                }
            }

            //}

        }
    }

    Column {
        id: clProdTemp
        width: 160
        height: 265
        anchors.left: clBr.right
        anchors.top: statusBar.bottom
        anchors.leftMargin: 5
        anchors.topMargin: 5
        spacing: 3

        Column {
            id: cl1
            height: 60
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            spacing: -1

            Row {
                width: parent.width + 2
                height: 40

                spacing: -1
                MFUnit {
                    id: temp1
                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в верхней зоне 1"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    objectName: root.objectName + ".in1.temp1"
                    extAlarmLevel: alarmTemp1.realValue
                }
                MFUnit {
                    id: temp2
                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в верхней зоне 2"
                    visible: true
                    text: "100"
                    valueText: "99"
                    valueFontSize.family: "DSEG7 Classic"
                    valueFontSize.letterSpacing: 0
                    valueFontSize.bold: false
                    extAlarmLevel: alarmTemp2.realValue
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    objectName: root.objectName + ".in1.temp2"
                }

                MFUnit {
                    id: temp3

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в верхней зоне 3"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    objectName: root.objectName + ".in1.temp3"
                    extAlarmLevel: alarmTemp3.realValue
                }
            }

            Row {
                width: parent.width + 2

                height: parent.height / 3
                spacing: -1
                MFUnit {
                    id: alarmTemp1

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в верхней зоне 1"
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
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

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в верхней зоне 2"
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
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

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в верхней зоне 3"
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
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
            id: cl2
            height: 60
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            spacing: -1

            Row {
                width: parent.width + 2
                height: 40
                spacing: -1
                MFUnit {
                    id: temp4

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в средней зоне 1"
                    objectName: root.objectName + ".in1.temp4"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    extAlarmLevel: alarmTemp4.realValue
                }

                MFUnit {
                    id: temp5

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в средней зоне 2"
                    objectName: root.objectName + ".in1.temp5"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    extAlarmLevel: alarmTemp5.realValue
                }

                MFUnit {
                    id: temp6

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в средней зоне 3"
                    objectName: root.objectName + ".in1.temp6"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    extAlarmLevel: alarmTemp6.realValue
                }
            }

            Row {
                width: parent.width + 2
                height: parent.height / 3
                spacing: -1
                MFUnit {
                    id: alarmTemp4

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в средней зоне 1"
                    objectName: root.objectName + ".in1.alarmTemp4"
                    readOnly: false
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    blinkColor: "#ff0000"
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp4Changed(value)
                    checkLimit: true
                    downLimit: 20
                }

                MFUnit {
                    id: alarmTemp5

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в средней зоне 2"
                    objectName: root.objectName + ".in1.alarmTemp5"
                    readOnly: false
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    blinkColor: "#ff0000"
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp5Changed(value)
                    checkLimit: true
                    downLimit: 20
                }

                MFUnit {
                    id: alarmTemp6

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в средней зоне 3"
                    objectName: root.objectName + ".in1.alarmTemp6"
                    readOnly: false
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
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
            id: cl3
            width: 150
            height: 80
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            spacing: -1

            Row {
                width: parent.width + 2
                height: 40
                spacing: -1
                MFUnit {
                    id: temp7

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в нижней зоне 1"
                    objectName: root.objectName + ".in1.temp7"
                    blinkColor: "#ff0000"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    extAlarmLevel: alarmTemp7.realValue
                }

                MFUnit {
                    id: temp8

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в нижней зоне 2"
                    objectName: root.objectName + ".in1.temp8"
                    blinkColor: "#ff0000"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    extAlarmLevel: alarmTemp8.realValue
                }

                MFUnit {
                    id: temp9

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в нижней зоне 3"
                    objectName: root.objectName + ".in1.temp9"
                    blinkColor: "#ff0000"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    extAlarmLevel: alarmTemp9.realValue
                }
            }

            Row {
                width: parent.width + 2
                height: parent.height / 4
                spacing: -1
                MFUnit {
                    id: targetTempLow

                    width: parent.width - 2
                    height: parent.height
                    backgroundColor: "#fbeb5d"
                    tooltip: "Автоподдержание температуры продукта в нижней зоне"
                    readOnly: false
                    blinkColor: "#ff0000"
                    visible: true
                    text: ""
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    valueFontSize.pointSize: 8
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.s_targetTempLowChd(value)
                    checkLimit: true
                    downLimit: 20
                    mantissa:1
                }
            }
            Row {
                width: parent.width + 2
                height: parent.height / 4
                spacing: -1
                MFUnit {
                    id: alarmTemp7

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в нижней зоне 1"
                    objectName: root.objectName + ".in1.alarmTemp7"
                    readOnly: false
                    blinkColor: "#ff0000"
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp7Changed(value)
                    checkLimit: true
                    downLimit: 20
                }

                MFUnit {
                    id: alarmTemp8

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в нижней зоне 2"
                    objectName: root.objectName + ".in1.alarmTemp8"
                    readOnly: false
                    blinkColor: "#ff0000"
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp8Changed(value)
                    checkLimit: true
                    downLimit: 20
                }

                MFUnit {
                    id: alarmTemp9

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в нижней зоне 3"
                    objectName: root.objectName + ".in1.alarmTemp9"
                    readOnly: false
                    blinkColor: "#ff0000"
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp9Changed(value)
                    checkLimit: true
                    downLimit: 20
                }
            }
        }

        Column {
            id: cl4
            width: 150
            height: 60
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            spacing: -1

            Row {
                width: parent.width + 2
                height: 40
                spacing: -1
                MFUnit {
                    id: temp10

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в зоне охлаждения 1"
                    objectName: root.objectName + ".in1.temp10"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    extAlarmLevel: alarmTemp10.realValue
                }

                MFUnit {
                    id: temp11

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в зоне охлаждения 2"
                    objectName: root.objectName + ".in1.temp11"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    extAlarmLevel: alarmTemp11.realValue
                }

                MFUnit {
                    id: temp12

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#afffb4"
                    tooltip: "Температура продукта в зоне охлаждения 3"
                    objectName: root.objectName + ".in1.temp12"
                    visible: true
                    valueFontSize.family: "DSEG7 Classic"
                    allovAlarmBlinck: true
                    valueFontSize.pointSize: 16
                    blinkColor: "#ff0000"
                    extAlarmLevel: alarmTemp12.realValue
                }
            }

            Row {
                width: parent.width + 2
                height: parent.height / 3
                spacing: -1
                MFUnit {
                    id: alarmTemp10

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в зоне охлаждения 1"
                    objectName: root.objectName + ".in1.alarmTemp10"
                    readOnly: false
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    blinkColor: "#ff0000"
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp10Changed(value)
                    checkLimit: true
                    downLimit: 20
                }

                MFUnit {
                    id: alarmTemp11

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в зоне охлаждения 2"
                    objectName: root.objectName + ".in1.alarmTemp11"
                    readOnly: false
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    blinkColor: "#ff0000"
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp11Changed(value)
                    checkLimit: true
                    downLimit: 20
                }

                MFUnit {
                    id: alarmTemp12

                    width: parent.width / 3
                    height: parent.height
                    backgroundColor: "#f03e3e"
                    tooltip: "Температура перегрева продукта в зоне охлаждения 3"
                    objectName: root.objectName + ".in1.alarmTemp12"
                    readOnly: false
                    visible: true
                    valueFontSize.bold: false
                    valueFontSize.family: "DSEG7 Classic"
                    disappear: true
                    valueFontSize.pointSize: 8
                    blinkColor: "#ff0000"
                    correctingButtons: true
                    regexp: floatreg
                    onValueChanged: root.alarmTemp12Changed(value)
                    checkLimit: true
                    downLimit: 20
                }
            }
        }
    }

    Column {
        id: clBr
        width: 150
        height: 80
        anchors.left: statusBar.left
        anchors.top: statusBar.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 5
        spacing: -1

        Row {
            width: parent.width + 2
            height: 40

            spacing: -1
            MFUnit {
                id: temp16
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя горелки 1 до продукта"
                objectName: root.objectName + ".in1.temp16"
                visible: true
                valueFontSize.family: "DSEG7 Classic"
                allovAlarmBlinck: true
                valueFontSize.pointSize: 16
                blinkColor: "#ff0000"
                extAlarmLevel: alarmTemp16.realValue
            }

            MFUnit {
                id: temp17

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя горелки 2 до продукта"
                objectName: root.objectName + ".in1.temp17"
                visible: true
                valueFontSize.family: "DSEG7 Classic"
                allovAlarmBlinck: true
                valueFontSize.pointSize: 16
                blinkColor: "#ff0000"
                extAlarmLevel: alarmTemp17.realValue
            }

            MFUnit {
                id: temp18
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя горелки 3 до продукта"
                //objectName: root.objectName + ".in1.temp18"
                visible: true
                valueFontSize.family: "DSEG7 Classic"
                allovAlarmBlinck: true
                valueFontSize.pointSize: 16
                blinkColor: "#ff0000"
                extAlarmLevel: alarmTemp18.realValue

            }
        }

        Row {
            id: burnersCommTemp
            width: parent.width + 2
            height: 25
            spacing: -1
            MFUnit {
                id: targetTemp16

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#fdc69f"
                tooltip: "Уставка горелки 1"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                valueFontSize.pointSize: 10
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: b1.targetTempChanged(value)
                checkLimit: true
                downLimit: 20
                upLimit: 150
            }
            MFUnit {
                id: targetTemp17

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#fdc69f"
                tooltip: "Уставка горелки 2"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                valueFontSize.pointSize: 10
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: b2.targetTempChanged(value)
                checkLimit: true
                downLimit: 20
                upLimit: 150
            }
            MFUnit {
                id: targetTemp18

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#fdc69f"
                tooltip: "Уставка горелки 3"
                readOnly: false
                visible: true
                text: "99"
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                valueFontSize.pointSize: 10
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: b3.targetTempChanged(value)
                checkLimit: true
                downLimit: 20
                upLimit: 150
            }
        }
        Row {
            width: parent.width + 2
            height: parent.height / 4
            spacing: -1
            MFUnit {
                id: alarmTemp16
                name:""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#f03e3e"
                tooltip: "Температура перегрева теплоносителя после горелки 1"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                disappear: true
                valueFontSize.pointSize: 8
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: {
                    temp16.extAlarmLevel = value
                    b1.alarmTempChanged(value)
                }
                checkLimit: true
                downLimit: 20
                upLimit: 150
            }
            MFUnit {
                id: alarmTemp17
                name:""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#f03e3e"
                tooltip: "Температура перегрева теплоносителя после горелки 1"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                disappear: true
                valueFontSize.pointSize: 8
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: {
                    temp17.extAlarmLevel = value
                    b2.alarmTempChanged(value)
                }
                checkLimit: true
                downLimit: 20
                upLimit: 150
            }
            MFUnit {
                id: alarmTemp18
                name:""
                width: parent.width / 3
                height: parent.height
                backgroundColor: "#f03e3e"
                tooltip: "Температура перегрева теплоносителя после горелки 1"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                disappear: true
                valueFontSize.pointSize: 8
                blinkColor: "#ff0000"
                correctingButtons: true
                regexp: floatreg
                onValueChanged: {
                    temp18.extAlarmLevel = value
                    b3.alarmTempChanged(value)
                }
                checkLimit: true
                downLimit: 20
                upLimit: 150
            }
        }
    }

    Column {
        id: clExt
        y: 112
        height: 60
        anchors.left: clProdTemp.right
        anchors.right: statusBar.right
        anchors.rightMargin: 0
        anchors.leftMargin: 5
        spacing: -1

        Row {
            width: parent.width + 2
            height: 40
            spacing: -1
            MFUnit {
                id: temp13

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя горелки 1 после продукта"
                objectName: root.objectName + ".in1.temp13"
                visible: true
                valueFontSize.family: "DSEG7 Classic"
                allovAlarmBlinck: true
                valueFontSize.pointSize: 16
                blinkColor: "#ff0000"
                extAlarmLevel: alarmTemp13.realValue
            }

            MFUnit {
                id: temp14

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя горелки 2 после продукта"
                objectName: root.objectName + ".in1.temp14"
                visible: true
                valueFontSize.family: "DSEG7 Classic"
                allovAlarmBlinck: true
                valueFontSize.pointSize: 16
                blinkColor: "#ff0000"
                extAlarmLevel: alarmTemp14.realValue
            }

            MFUnit {
                id: temp15

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#afffb4"
                tooltip: "Температура теплоносителя горелки 3 после продукта"
                objectName: root.objectName + ".in1.temp15"
                visible: true
                valueFontSize.family: "DSEG7 Classic"
                allovAlarmBlinck: true
                valueFontSize.pointSize: 16
                blinkColor: "#ff0000"
                extAlarmLevel: alarmTemp15.realValue
            }
        }

        Row {
            width: parent.width + 2
            height: parent.height / 3
            spacing: -1
            MFUnit {
                id: alarmTemp13

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#f03e3e"
                tooltip: "Температура перегрева теплоносителя горелки 1 после продукта"
                objectName: root.objectName + ".in1.alarmTemp133"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                disappear: true
                valueFontSize.pointSize: 8
                blinkColor: "#ff0000"
                regexp: floatreg
                correctingButtons: true
                onValueChanged: root.alarmTemp13Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp14

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#f03e3e"
                tooltip: "Температура перегрева теплоносителя горелки 2 после продукта"
                objectName: root.objectName + ".in1.alarmTemp134"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                disappear: true
                valueFontSize.pointSize: 8
                blinkColor: "#ff0000"
                regexp: floatreg
                correctingButtons: true
                onValueChanged: root.alarmTemp14Changed(value)
                checkLimit: true
                downLimit: 20
            }

            MFUnit {
                id: alarmTemp15

                width: parent.width / 3
                height: parent.height
                backgroundColor: "#f03e3e"
                tooltip: "Температура перегрева теплоносителя горелки 3 после продукта"
                objectName: root.objectName + ".in1.alarmTemp135"
                readOnly: false
                visible: true
                valueFontSize.bold: false
                valueFontSize.family: "DSEG7 Classic"
                disappear: true
                valueFontSize.pointSize: 8
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
        winTitle: "Свойства " + name
        onStart: root.start()
        onStop: root.stop()
        buttonsType: 4
    }

    Column {
        id: clPID
        x: 507
        y: 0
        width: 122
        height: 388
        spacing: 5



        Text {
            id: text1
            height: 35
            color: "#c9d2c6"
            text: qsTr("НАСТРОЙКИ ПИД РЕГУЛЯТОРОВ")
            anchors.left: parent.left
            anchors.right: parent.right
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            font.bold: true
            clip: false
            anchors.leftMargin: 0
            anchors.rightMargin: 0
        }

        Item {
            id: itemProdPID
            height: 106
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            Rectangle{
                color: "transparent"
                border.color: "#c9d2c6"
                anchors.fill: parent
                radius: 3
            }
            Text{
                id:t1
                color: "#c9d2c6"
                font.pointSize: 8
                transformOrigin: Item.Center
                text: qsTr("t°C ПРОДУКТА")
                anchors.left: parent.left
                anchors.top: parent.top
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.capitalization: Font.MixedCase
                anchors.topMargin: 4
                anchors.leftMargin: 6
                font.bold: true
            }

            Column {
                id: clProdTempPID
                height: 80
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: t1.bottom
                anchors.rightMargin: 4
                anchors.leftMargin: 4
                anchors.topMargin: 5

                Row {
                    id: row1
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0

                    Text {
                        id: text5
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Kp")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.italic: false
                        font.bold: true
                    }

                    TextInput {
                        id: brnrTempProdTempPID_Kp
                        onEditingFinished: root.s_brnrTempProdTempPID_KpChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text5.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        anchors.bottomMargin: 0
                        anchors.rightMargin: 0
                        anchors.leftMargin: 0
                        readOnly: false
                        validator: DoubleValidator{
                            locale: "en_EN"
                            decimals: 2
                            bottom: 0
                            top: 1000
                        }
                    }
                }

                Row {
                    id: row2
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                    Text {
                        id: text6
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Ki")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.italic: false
                        font.bold: true
                    }

                    TextInput {
                        id: brnrTempProdTempPID_Ki
                        onEditingFinished: root.s_brnrTempProdTempPID_KiChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text6.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        readOnly: false
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        anchors.rightMargin: 0
                        validator: DoubleValidator{
                            locale: "en_EN"
                            decimals: 6
                            bottom: 0
                            top: 10
                        }
                    }
                }

                Row {
                    id: row3
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Text {
                        id: text7
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Kd")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.italic: false
                        font.bold: true
                    }

                    TextInput {
                        id: brnrTempProdTempPID_Kd
                        onEditingFinished: root.s_brnrTempProdTempPID_KdChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text7.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        readOnly: false
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        anchors.rightMargin: 0
                        validator: DoubleValidator{
                            locale: "en_EN"
                            decimals: 2
                            bottom: 0
                            top: 1000
                        }
                    }
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                }

                Row {
                    id: row4
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    Text {
                        id: text8
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Td(с)")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.italic: false
                        font.bold: true
                    }

                    TextInput {
                        id: brnrTempProdTempPID_Td
                        onEditingFinished: root.s_brnrTempProdTempPID_TdChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text8.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        readOnly: false
                        anchors.leftMargin: 0
                        anchors.bottomMargin: 0
                        anchors.topMargin: 0
                        anchors.rightMargin: 0
                        validator: intreg
                    }
                    anchors.leftMargin: 0
                    anchors.rightMargin: 0
                }
            }
        }

        Item {
            id: itemProdTempHumPID
            height: 106
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            visible: root.humidity
            Rectangle {
                color: "#00000000"
                radius: 3
                border.color: "#c9d2c6"
                anchors.fill: parent
            }

            Text {
                id: t2
                color: "#c9d2c6"
                text: qsTr("влажность")
                anchors.left: parent.left
                anchors.top: parent.top
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.capitalization: Font.AllUppercase
                font.pointSize: 8
                font.bold: true
                anchors.leftMargin: 6
                anchors.topMargin: 4
                transformOrigin: Item.Center
            }

            Column {
                id: clProdTempHumPID
                height: 80
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: t2.bottom
                anchors.rightMargin: 4
                anchors.leftMargin: 4
                Row {
                    id: row5
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text9
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Kp")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false
                    }

                    TextInput {
                        id: prodTempHumPID_Kp
                        onEditingFinished: root.s_prodTempHumPID_KpChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text9.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: DoubleValidator{
                            locale: "en_EN"
                            decimals: 2
                            bottom: 0
                            top: 1000
                        }
                    }
                }

                Row {
                    id: row9
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text13
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Ki")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false
                    }

                    TextInput {
                        id: prodTempHumPID_Ki
                        onEditingFinished: root.s_prodTempHumPID_KiChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text13.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: DoubleValidator{
                            locale: "en_EN"
                            decimals: 6
                            bottom: 0
                            top: 10
                        }
                    }
                }

                Row {
                    id: row10
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text14
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Kd")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false
                    }

                    TextInput {
                        id: prodTempHumPID_Kd
                        onEditingFinished: root.s_prodTempHumPID_KdChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text14.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: DoubleValidator{
                            locale: "en_EN"
                            decimals: 2
                            bottom: 0
                            top: 1000
                        }
                    }
                }

                Row {
                    id: row11
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text15
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Td(с)")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false

                    }

                    TextInput {
                        id: prodTempHumPID_Td
                        onEditingFinished: root.s_prodTempHumPID_TdChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text15.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: intreg
                    }
                }
                anchors.topMargin: 5
            }
        }

        Item {
            id: utSpeedHumPID
            height: 120
            visible: false
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            Rectangle {
                color: "#00000000"
                radius: 3
                border.color: "#c9d2c6"
                anchors.fill: parent
            }

            Text {
                id: t3
                color: "#c9d2c6"
                text: qsTr("скорость выгрузки")
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                anchors.rightMargin: 0
                font.capitalization: Font.AllUppercase
                font.pointSize: 8
                font.bold: true
                anchors.leftMargin: 6
                anchors.topMargin: 4
                transformOrigin: Item.Center
            }

            Column {
                id: clUTSpeedHumPID
                height: 80
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: t3.bottom
                anchors.rightMargin: 4
                anchors.leftMargin: 4
                Row {
                    id: row12
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text16
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Kp")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false
                    }

                    TextInput {
                        id: utSpeedHumPID_Kp
                        onEditingFinished: root.s_UTSpeedHumPID_KpChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text16.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: DoubleValidator{
                            decimals: 2
                            bottom: 0
                            top: 1000
                        }
                    }
                }

                Row {
                    id: row13
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text17
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Ki")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false
                    }

                    TextInput {
                        id: utSpeedHumPID_Ki
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text17.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: DoubleValidator{
                            decimals: 2
                            bottom: 0
                            top: 1000
                        }
                    }
                }

                Row {
                    id: row14
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text18
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Kd")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false
                    }

                    TextInput {
                        id: utSpeedHumPID_Kd
                        onEditingFinished: root.s_UTSpeedHumPID_KdChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text18.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: DoubleValidator{
                            decimals: 2
                            bottom: 0
                            top: 1000
                        }
                    }
                }

                Row {
                    id: row15
                    width: 104
                    height: 20
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.rightMargin: 0
                    anchors.leftMargin: 0
                    Text {
                        id: text19
                        width: 52
                        color: "#c9d2c6"
                        text: qsTr("Td(с)")
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.italic: false
                    }

                    TextInput {
                        id: utSpeedHumPID_Td
                        onEditingFinished: root.s_UTSpeedHumPID_TdChd(text)
                        visible: true
                        color: "#c9d2c6"
                        text: "0.5"
                        anchors.left: text19.right
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        anchors.topMargin: 0
                        readOnly: false
                        anchors.rightMargin: 0
                        anchors.bottomMargin: 0
                        anchors.leftMargin: 0
                        validator: intreg
                    }
                }
                anchors.topMargin: 5
            }
        }



    }

    Column {
        id: clHm
        width: 128
        height: 144
        anchors.top: rowUt.top
        anchors.horizontalCenter: clBr.horizontalCenter
        anchors.topMargin: 0
        spacing: 9
        visible: root.humidity
        Column {
            id: inHum
            y: 383
            width: 94
            height: 40
            visible: root.humidity
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            spacing: -1
            //            MFUnit {

            //                width: parent.width
            //                height: parent.height / 2

            //                fontSize: 10
            //            }

            MFUnit {
                id: hrIn
                name: "ВХ. ВЛАЖНОСТЬ"
                width: parent.width
                height: parent.height
                blinkColor: "#ff0000"
                transformOrigin: Item.Center
                mantissa:1
                regexp: floatreg
                text: "12"
                valueFontSize.family: "DSEG7 Classic"
                valueFontSize.bold: false
                valueFontSize.pointSize: 12
            }
        }

        Transporter {
            id: hShA

            y: 496
            height: 20
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            objectName: root.objectName + ".hShA"
            Text {
                text: "ШНЕК ВХ. ВЛАГОМЕРА"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: "WordWrap"
            }
        }
        Column {
            id: outHum
            y: 449
            height: 40
            visible: root.humidity
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            spacing: -1
            //            MFUnit {
            //                width: parent.width
            //                height: parent.height / 2
            //                text: "ВЫХ. ВЛАЖНОСТЬ"
            //                fontSize: 10
            //            }

            MFUnit {
                id: hrOut
                name: "ВЫХ. ВЛАЖНОСТЬ"
                width: parent.width
                height: parent.height
                blinkColor: "#ff0000"
                transformOrigin: Item.Center
                mantissa:1
                regexp: floatreg
                text: "12.3"
                valueFontSize.family: "DSEG7 Classic"
                valueFontSize.bold: false
                valueFontSize.pointSize: 12
            }
        }

        Transporter {
            id: hShB
            y: 485
            width: 61
            height: 20
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.leftMargin: 0
            objectName: root.objectName + ".hShB"
            Text {
                text: "ШНЕК ВЫХ. ВЛАГОМЕРА"
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: "WordWrap"
            }
        }

    }

    Button {
        id: bnSTOP
        x: 9
        y: 8
        width: 145
        height: 62
        text: "СТОП!"
        background: Rectangle {
            color: parent.pressed ? "firebrick" : "red"
            radius: 15
        }
        font.pointSize: 30
        font.bold: true
        onClicked: root.stop()
    }





}



