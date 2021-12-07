import QtQuick 2.12
//import QtQuick.Controls 1.3
import QtQuick.Controls 2.12
import "fap.js" as Fap

Item {
    id: root
    width: 120
    height: 40
    property color  backgroundColor: "white"
    property color  blinkColor: "yellow"
    property color  borderColor: "black"
    property int    borderWidth: 1
    //property int    fontSize: 12
    property alias  text: valueLable.text
    property string tooltip: ""

    property bool   checkLimit: true
    property bool   correctingButtons: false
    property alias  readOnly: valueLable.readOnly
    property real   step: 1
    property int    upLimit: 100
    property int    downLimit: 0
    //    property alias  regexp: valueLable.validator
    property int    mantissa: 1
    property alias  valueFontSize: valueLable.font
    property bool   disappear: false
    property alias  valueText: valueLable.text
    property real   extAlarmLevel: 0

    function setValue(value){//уст значение без мигания
        if (root.text !== value){
            if (!Fap.isString(value)){
                value = value.toFixed(mantissa)
            }
            root.text = value
        }
    }

    function setValueLimited(value){//уст с учетом лимитов
        var newValue = Number(value)
        if( newValue < root.downLimit ){
            newValue = root.downLimit
            setValue( newValue)
            valueChanged( root.text )
        }
        else if( newValue > root.upLimit ){
            newValue = root.upLimit
            setValue( newValue)
            valueChanged( root.text )
        }
        else
            setValue( newValue)
    }

    signal valueChanged(variant value)

    MouseArea{
        anchors.fill: parent
        onClicked: root.notified();
        hoverEnabled: true
        onEntered: {
            ttip.visible = true
        }
        onExited: {
            ttip.visible = false
        }
        onContainsMouseChanged: {
            if( disappear ){
                if( containsMouse )
                    body.visible = true;
                else
                    body.visible = false;
            }
        }
    }
    Rectangle{
        id: body
        visible: ! root.disappear
        anchors.fill: parent
        color: backgroundColor
        border.width: borderWidth
        border.color: borderColor
        ToolTip{
            id: ttip
            delay: 2000
            timeout: 2000
            visible: false
            text: tooltip
        }

        TextInput{
            property bool
            editing: false
            text: "99"
            id: valueLable
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.italic: editing
            font.pixelSize: height * 0.7
            font.family: "DSEG7 Classic"
            readOnly: true
            validator: DoubleValidator{
                decimals: mantissa
                notation: Qt.StandardNotation
                locale: "GB"
            }
            onEditingFinished: {
                if (Number(text) < downLimit ) {
                    text = downLimit
                }
                parent.valueChanged(text)
            }
            onTextChanged: {
                if (Number(text) > upLimit) {
                    remove(text.length - 1, text.length)
                }
            }
            Text{
                width: height / 3*2
                visible: root.correctingButtons
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: "-"
                font.bold: true
                font.pixelSize: 0
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
                MouseArea{
                    anchors.fill: parent
                    onClicked: root.changeLimited(-root.step);
                }
            }

            Text{
                width: height / 3*2
                visible: root.correctingButtons
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                text: "+"
                font.bold: true
                font.pixelSize: height * 0.7
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.bottomMargin: 0
                MouseArea{
                    anchors.fill: parent
                    onClicked: root.changeLimited(root.step);
                }
            }

        }
    }
}

/*##^##
Designer {
    D{i:0;height:35;width:102}
}
##^##*/
