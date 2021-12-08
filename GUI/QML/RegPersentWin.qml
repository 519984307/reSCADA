import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: root
    width: 180
    height: 170
    title: "РК"

    flags: Qt.Window | Qt.Dialog
    minimumWidth: 180
    maximumHeight: 170

    signal minRangeChanged ( string value )
    signal maxRangeChanged ( string value )
    signal valueChanged    ( string value )

    function setMinRange ( value: string ) { minValue.setValue( value ) }
    function setMaxRange ( value: string ) { maxValue.setValue( value ) }
    function setValue    ( value: string ) { curValue.setValue( value ) }
    function readValue   (){
        return curValue.valueText
    }

    onVisibleChanged: {
        if( visible == true){
            var absolutePos = mapToGlobal(0,0);
            x = absolutePos.x
            y = absolutePos.y
            requestActivate()
        }
    }

    Text{
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
    Rectangle{
        anchors.fill: textFrom
        color: "#6e9ec8"
    }
    Text{
        id: textFrom
        height: 20
        text: "ОТ"
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.top: textWrkDp.bottom
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: height * 0.8
    }
    Rectangle{
        anchors.fill: textTo
        color: "#c36b6b"
    }
    Text{
        id: textTo
        height: 20
        text: "ДО"
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.top: textWrkDp.bottom
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: height * 0.8
    }

    MFUnit {
        id: maxValue
        height: 20
        valueText: "80"
        backgroundColor: "#c36b6b"
        borderColor: "#c36b6b"
        tooltip: "Max"
        readOnly: false
        visible: true
        anchors.left: parent.horizontalCenter
        anchors.right: parent.right
        anchors.top: textTo.bottom
        anchors.topMargin: 0
        anchors.leftMargin: 0
        anchors.rightMargin: 0
        correctingButtons: true
        onValueChanged:  maxRangeChanged( value )
        checkLimit: true
        downLimit: Number(minValue.valueText) + 1
    }
    MFUnit {
        id: minValue
        height: 20
        valueText: "20"
        backgroundColor: "#6e9ec8"
        borderColor: "#6e9ec8"
        tooltip: "Min"
        readOnly: false
        visible: true
        anchors.left: parent.left
        anchors.right: parent.horizontalCenter
        anchors.top: textFrom.bottom
        correctingButtons: true
        onValueChanged:  minRangeChanged( value )
        checkLimit: true
        upLimit: Number(maxValue.valueText) - 1

    }
    Item{
        id: item1
        height: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: minValue.bottom
        Rectangle{
            id: min
            width: Number(minValue.valueText)/100 * parent.width
            color: "#6e9ec8"
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
        Rectangle{
            id: wrk
            color: "#1a6b14"
            anchors.left: min.right
            anchors.right: max.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
        Rectangle{
            id: max
            width: (1 - Number(maxValue.valueText)/100) * parent.width
            color: "#c36b6b"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
        Rectangle{
            id: recLvl
            x: Number(curValue.valueText)/100 * parent.width
            width: 2
            height: parent.height * 1.1
            color: "#a3fa96"
        }
    }
    Rectangle{
        color: "#a3fa96"
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: item1.bottom
        anchors.bottom: stepTxt.bottom
    }
    Text {
        id: textLvl
        height: 20
        text: "ТЕКУЩЕЕ РАСКРЫТИЕ %"
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
        valueText: "60"
        backgroundColor: "#a3fa96"
        borderColor: "#a3fa96"
        tooltip: "Min"
        readOnly: false
        visible: true
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: textLvl.bottom
        correctingButtons: true
        onValueChanged: root.valueChanged( value )
        checkLimit: true
        upLimit: Number(maxValue.valueText)
        downLimit: Number(minValue.valueText)
        step: Number(regStep.valueText)
    }
    Text {
        id: stepTxt
        height: 20
        width: text.length * font.pixelSize *0.7
        text: "ШАГ РЕГУЛИРОВКИ -"
        anchors.left: parent.left
        anchors.top: curValue.bottom
        font.pixelSize: height * 0.6
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    MFUnit{
        id: regStep
        valueText: "1"
        height: 20
        backgroundColor: "#a3fa96"
        borderColor: "#a3fa96"
        anchors.left: stepTxt.right
        anchors.right: parent.right
        anchors.top: curValue.bottom
        textInput.font.bold: true
        correctingButtons: false
        upLimit: 10
        downLimit: 0.1
    }
}



