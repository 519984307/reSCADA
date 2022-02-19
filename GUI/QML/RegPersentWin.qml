import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: root
    width: 180
    height: 150
    title: "РК"
    flags: Qt.Window | Qt.Dialog
    minimumWidth: 180
    maximumHeight: 170
    property alias sepCorBtn: curValue.separCorrButtons

    property alias value: curValue.valueReal
    property alias step: regStep.valueReal

    property alias valueMax: maxValue.valueReal
    property alias valueMin: minValue.valueReal
    property alias readOnly: curValue.readOnly
    property color mainColor: "#a3fa96"
    property color scaleColor: "#1a6b14"


    //signal minRangeChanged(int value)
    //signal maxRangeChanged(int value)
    //signal valueChanged   (int value)
    //signal stepChanged    (int value)

    function setValueMinRange( MinRg ) { minValue.setValue( MinRg ) }
    function setValueMaxRange( MaxRg ) { maxValue.setValue( MaxRg ) }
    function setValue(Value) { curValue.setValue( Value ) }
    function setStep( Step ) { regStep.setValue(Step)}
    signal s_moreVal( variant More )
    signal s_lessVal( variant Less )
    signal s_valueChenged(variant Value)
    signal s_valueMaxChenged(variant Value)
    signal s_valueMinChenged(variant Value)

    onVisibleChanged: {
        if (visible == true) {
            var absolutePos = mapToGlobal(0, 0)
            x = absolutePos.x
            y = absolutePos.y
            requestActivate()
        }
    }

    Text {
        id: textWrkDp
        height: 20
        text: "РАБОЧИЙ ДИАПАЗОН %"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        font.pixelSize: height * 0.6
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Rectangle {
        color: "#6e9ec8"
    }
    Rectangle {
        color: "#c36b6b"
    }

    MFUnit {
        id: maxValue
        height: 20
        valueReal: 100
        backgroundColor: "#c36b6b"
        borderColor: "#c36b6b"
        tooltip: "максимальное значение"
        readOnly: false
        visible: true
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.top: textWrkDp.bottom
        correctingButtons: true
        //onValueRealChanged: maxRangeChanged(valueReal)
        limited: true
        downLimit: 0//minValue.valueReal + 1
        onValueChanged: s_valueMaxChenged( Value )
    }
    MFUnit {
        id: minValue
        height: 20
        valueReal: 0
        backgroundColor: "#6e9ec8"
        borderColor: "#6e9ec8"
        tooltip: "минимальное значение"
        readOnly: false
        visible: true
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.top: textWrkDp.bottom
        correctingButtons: true
        //onValueRealChanged: minRangeChanged(valueReal)
        limited: true
        upLimit: 100//maxValue.valueReal - 1
        onValueChanged: s_valueMinChenged( Value )
    }
    Item {
        id: item1
        height: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: minValue.bottom
        Rectangle {
            id: min
            width: valueMin / 100 * parent.width
            color: "#6e9ec8"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
        Rectangle {
            id: wrk
            color: scaleColor
            anchors.left: min.right
            anchors.right: max.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
        Rectangle {
            id: max
            width: (1 - valueMax / 100) * parent.width
            color: "#c36b6b"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
        Rectangle {
            id: recLvl
            x: value / 100 * parent.width
            width: 2
            height: parent.height * 1.1
            color: mainColor
        }
    }
    Rectangle {
        color: mainColor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: item1.bottom
        anchors.bottom: stepTxt.bottom
    }
    Text {
        id: textLvl
        height: 20
        text: "ТЕКУЩЕЕ ЗНАЧЕНИЕ %"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: item1.bottom
        font.pixelSize: height * 0.6
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    MFUnit {
        id: curValue
        height: 40
        valueReal: 60
        backgroundColor: mainColor
        borderColor: mainColor
        tooltip: "Min"
        readOnly: false
        visible: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textLvl.bottom
        correctingButtons: true
        //onValueRealChanged: valueChanged(valueReal)
        limited: true
        upLimit: maxValue.valueReal
        downLimit: minValue.valueReal
        step: regStep.valueReal
        onS_more: s_moreVal( More )
        onS_less: s_lessVal( Less )
        onValueChanged: s_valueChenged( Value )
    }
    Text {
        id: stepTxt
        height: 20
        width: text.length * font.pixelSize * 0.7
        visible: !curValue.readOnly
        text: "ШАГ РЕГУЛИРОВКИ -"
        anchors.left: parent.left
        anchors.top: curValue.bottom
        font.pixelSize: height * 0.6
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    MFUnit {
        id: regStep
        valueReal: 1
        height: 20
        visible: !curValue.readOnly
        backgroundColor: mainColor
        borderColor: mainColor
        anchors.left: stepTxt.right
        anchors.right: parent.right
        anchors.top: curValue.bottom
        textInput.font.bold: true
        correctingButtons: false
        upLimit: 10
        downLimit: 1
        //onValueRealChanged: stepChanged(valueReal)
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:2}
}
##^##*/
