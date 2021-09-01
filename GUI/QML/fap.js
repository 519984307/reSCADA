const version = "3.6.0"

const headBackground = "gainsboro"
const background = "gainsboro"
const contrBackground = "white"

const defaultColor = "white"
const run = "green"//"limegreen"
const ready = "#3a7b9a"
const blocked = "brown"
const manual = "yellow"
const middle = "lime"//
const notAvailable = "darkgray"
const alarm = "red"
const notAlarm = "black"

const tra = "#f1c40f"
const trna = "#cfb53b"
const aspa = "#3498db"
const aspna = "#af7ac5"

const inPos = "limegreen"//"green"
const toPos = "lime"
const outPos = "cyan"
const noDef = "#f67e49"

const engTrueSignal = "green"
const engFalseSignal = "white"

const currentBackground = "black"
const currentBackgroundFont = "#441212"
const currentFont = "red"

const indicatorBackground = "#2E2E2E"
const indicatorHLActivate = "#DF0101"
const indicatorMLActivate = "#FF8000"
const indicatorLLActivate = "green"
const indicatorDeactivate = "gainsboro"

const blinkDelay = 500

//---------------------------------------------------

var colorArr = [
            {backColor:"maroon", fontColor:"white"},
            {backColor:"darkslategray", fontColor:"white"},
            {backColor:"navajowhite", fontColor:"black"},
            {backColor:"lavenderblush", fontColor:"black"},
            {backColor:"dodgerblue", fontColor:"white"},
            {backColor:"olive", fontColor:"white"},
            {backColor:"cornflowerblue", fontColor:"white"},
            {backColor:"royalblue", fontColor:"white"},
            {backColor:"palegreen", fontColor:"black"},
            {backColor:"lightcoral", fontColor:"black"},
            {backColor:"darkcyan", fontColor:"white"},
            {backColor:"mediumvioletred", fontColor:"white"},
            {backColor:"orange", fontColor:"white"},
            {backColor:"ivory", fontColor:"black"},
            {backColor:"palegoldenrod", fontColor:"black"},
            {backColor:"navy", fontColor:"white"},
            {backColor:"tomato", fontColor:"white"},
            {backColor:"purple", fontColor:"white"},
            {backColor:"red", fontColor:"white"},
            {backColor:"mediumslateblue", fontColor:"white"},
            {backColor:"darkmagenta", fontColor:"white"},
            {backColor:"silver", fontColor:"black"},
            {backColor:"mediumseagreen", fontColor:"white"},
            {backColor:"lightgreen", fontColor:"black"},
            {backColor:"mediumspringgreen", fontColor:"black"},
            {backColor:"lemonchiffon", fontColor:"black"},
            {backColor:"springgreen", fontColor:"black"},
            {backColor:"pink", fontColor:"black"},
            {backColor:"palevioletred", fontColor:"white"},
            {backColor:"saddlebrown", fontColor:"white"},
            {backColor:"oldlace", fontColor:"black"},
            {backColor:"ghostwhite", fontColor:"black"},
            {backColor:"Magenta", fontColor:"white"},
            {backColor:"dimgray", fontColor:"white"},
            {backColor:"aliceblue", fontColor:"black"},
            {backColor:"paleturquoise", fontColor:"black"},
            {backColor:"aquamarine", fontColor:"black"},
            {backColor:"darkslategrey", fontColor:"white"},
            {backColor:"floralwhite", fontColor:"black"},
            {backColor:"slategrey", fontColor:"white"},
            {backColor:"coral", fontColor:"white"},
            {backColor:"mintcream", fontColor:"black"},
            {backColor:"darkorange", fontColor:"white"},
            {backColor:"darkolivegreen", fontColor:"white"},
            {backColor:"mediumorchid", fontColor:"white"},
            {backColor:"moccasin", fontColor:"black"},
            {backColor:"plum", fontColor:"white"},
            {backColor:"lightcyan", fontColor:"black"},
            {backColor:"forestgreen", fontColor:"white"},
            {backColor:"cyan", fontColor:"black"},
            {backColor:"lime", fontColor:"black"},
            {backColor:"gold", fontColor:"black"},
            {backColor:"midnightblue", fontColor:"white"},
            {backColor:"snow", fontColor:"black"},
            {backColor:"fireBrick", fontColor:"white"},
            {backColor:"LightGrey", fontColor:"black"},
            {backColor:"brown", fontColor:"white"},
            {backColor:"darkgrey", fontColor:"black"},
            {backColor:"indianred", fontColor:"white"},
            {backColor:"khaki", fontColor:"black"},
            {backColor:"blanchedalmond", fontColor:"black"},
            {backColor:"gainsboro", fontColor:"black"},
            {backColor:"darkkhaki", fontColor:"white"},
            {backColor:"white", fontColor:"black"},
            {backColor:"green", fontColor:"white"},
            {backColor:"papayawhip", fontColor:"black"},
            {backColor:"darkviolet", fontColor:"white"},
            {backColor:"darkslateblue", fontColor:"white"},
            {backColor:"powderblue", fontColor:"black"},
            {backColor:"lightslategray", fontColor:"white"},
            {backColor:"darksalmon", fontColor:"black"},
            {backColor:"wheat", fontColor:"black"},
            {backColor:"cornsilk", fontColor:"black"},
            {backColor:"burlywood", fontColor:"white"},
            {backColor:"darkblue", fontColor:"white"},
            {backColor:"teal", fontColor:"white"},
            {backColor:"olivedrab", fontColor:"white"},
            {backColor:"yellowgreen", fontColor:"white"},
            {backColor:"lightsteelblue", fontColor:"black"},
            {backColor:"bisque", fontColor:"black"},
            {backColor:"cadetblue", fontColor:"white"},
            {backColor:"violet", fontColor:"white"},
            {backColor:"thistle", fontColor:"white"},
            {backColor:"chocolate", fontColor:"white"},
            {backColor:"lightskyblue", fontColor:"black"},
            {backColor:"mediumblue", fontColor:"white"},
            {backColor:"lightsalmon", fontColor:"black"},
            {backColor:"salmon", fontColor:"black"},
            {backColor:"aqua", fontColor:"black"},
            {backColor:"steelblue", fontColor:"white"},
            {backColor:"lightblue", fontColor:"black"},
            {backColor:"deeppink", fontColor:"white"},
            {backColor:"peachpuff", fontColor:"black"},
            {backColor:"sienna", fontColor:"white"},
            {backColor:"Peru", fontColor:"white"},
            {backColor:"indigo", fontColor:"white"},
            {backColor:"slateblue", fontColor:"white"},
            {backColor:"dimgrey", fontColor:"white"},
            {backColor:"rosybrown", fontColor:"white"},
            {backColor:"blue", fontColor:"white"},
            {backColor:"seagreen", fontColor:"white"},
            {backColor:"darkturquoise", fontColor:"white"},
            {backColor:"mistyrose", fontColor:"black"},
            {backColor:"linen", fontColor:"black"},
            {backColor:"mediumturquoise", fontColor:"white"},
            {backColor:"honeydew", fontColor:"black"},
            {backColor:"azure", fontColor:"black"},
            {backColor:"beige", fontColor:"black"},
            {backColor:"orchid", fontColor:"white"},
            {backColor:"deepskyblue", fontColor:"white"},
            {backColor:"mediumpurple", fontColor:"white"},
            {backColor:"antiquewhite", fontColor:"black"},
            {backColor:"black", fontColor:"white"},
            {backColor:"lightslategrey", fontColor:"white"},
            {backColor:"mediumaquamarine", fontColor:"black"},
            {backColor:"tan", fontColor:"white"},
            {backColor:"skyblue", fontColor:"black"},
            {backColor:"darkred", fontColor:"white"},
            {backColor:"lightpink", fontColor:"black"},
            {backColor:"sandybrown", fontColor:"white"},
            {backColor:"turquoise", fontColor:"black"},
            {backColor:"darkgoldenrod", fontColor:"white"},
            {backColor:"greenyellow", fontColor:"black"},
            {backColor:"limegreen", fontColor:"black"},
            {backColor:"lightgoldenrodyellow", fontColor:"black"},
            {backColor:"yellow", fontColor:"black"},
            {backColor:"hotpink", fontColor:"white"},
            {backColor:"lawngreen", fontColor:"black"},
            {backColor:"rebeccapurple", fontColor:"white"},
            {backColor:"whitesmoke", fontColor:"black"},
            {backColor:"fuchsia", fontColor:"white"},
            {backColor:"gray", fontColor:"black"},
            {backColor:"LightGrey", fontColor:"black"},
            {backColor:"lavender", fontColor:"black"},
            {backColor:"slategray", fontColor:"white"},
            {backColor:"darkgreen", fontColor:"white"},
            {backColor:"darkseagreen", fontColor:"white"},
            {backColor:"darkgray", fontColor:"black"},
            {backColor:"chartreuse", fontColor:"black"},
            {backColor:"blueviolet", fontColor:"white"},
            {backColor:"orangered", fontColor:"white"},
            {backColor:"lightseagreen", fontColor:"white"},
            {backColor:"seashell", fontColor:"black"},
            {backColor:"grey", fontColor:"black"},
            {backColor:"lightyellow", fontColor:"black"},
            {backColor:"goldenrod", fontColor:"white"},
            {backColor:"darkorchid", fontColor:"white"}
        ];

//---------------------------------------------------
function getAbsolutePosition(node) {
    var returnPos = {};
    returnPos.x = 0;
    returnPos.y = 0;
    if(node !== undefined && node !== null) {
        var parentValue = getAbsolutePosition(node.parent);
        returnPos.x = parentValue.x + node.x;
        returnPos.y = parentValue.y + node.y;
    }
    return returnPos;
}

function changeColor(node){
    if(node.alarm){
        node.borderColor = alarm
    }else if(node.cleaning){
        node.borderColor = tra
    }else{
        node.borderColor = notAlarm
    }

    if(node.alarmNotify&&node.alarmBlink){
        node.fillColor = alarm
    }else{
        if(!node.linked){
            node.fillColor = defaultColor
        }else if (!node.available){
            node.fillColor = notAvailable
        }else if(node.blocked){
            node.fillColor = blocked
        }else if(node.manual){
            node.fillColor = manual
        }else{
            if (node.st&node.std){
                node.fillColor = run
            }else if(st^std){
                node.fillColor = middle
            }else{
                node.fillColor = ready
            }
        }

    }
}

function changeColorG(node){
    if(node.alarm){
        node.borderColor1 = alarm
        node.borderColor2 = alarm
    }else{
        node.borderColor1 = notAlarm
        node.borderColor2 = notAlarm
    }
    if(!node.linked){
        node.fillColor1 = defaultColor
        node.fillColor2 = defaultColor
    }else if (!node.available){
        node.fillColor1 = notAvailable
        node.fillColor2 = notAvailable
    }else if(node.blocked){
        node.fillColor1 = blocked
        node.fillColor2 = blocked
    }else if(node.manualOpenMove|node.manualCloseMove){
        if (node.manualOpenMove){ node.fillColor1 = manual }
        if (node.manualCloseMove){ node.fillColor2 = manual }
    }
    else{
        if(node.closed && node.opened){
            node.fillColor1 = noDef
            node.fillColor2 = noDef
        }
        else if(node.opened){
            node.fillColor1 = inPos
            node.fillColor2 = inPos
        }
        else if(node.closed){
            node.fillColor1 = ready
            node.fillColor2 = ready
        }
        else {
            node.fillColor1 = inPos
            node.fillColor2 = ready
        }

        if (node.openCmd & node.openMove){
            node.fillColor1 = toPos
        }
        else if(node.openCmd ^ node.openMove){
            node.fillColor1 = toPos
        }

        if (node.closedCmd & node.closedMove){
            node.fillColor2 = outPos
        }
        else if(node.closedCmd ^ node.closedMove){
            node.fillColor2 = outPos
        }
    }
}

function changeColorV(node){
    if(node.alarm){
        node.borderColor1 = alarm
        node.borderColor2 = alarm
    }else{
        node.borderColor1 = notAlarm
        node.borderColor2 = notAlarm
    }
    if(!node.linked){
        node.fillColor1 = defaultColor
        node.fillColor2 = defaultColor
    }else if (!node.available){
        node.fillColor1 = notAvailable
        node.fillColor2 = notAvailable
    }else if(node.blocked){
        node.fillColor1 = blocked
        node.fillColor2 = blocked
    }else if(node.manual1|node.manual2){
        if (node.manual1){ node.fillColor1 = manual }
        if (node.manual2){ node.fillColor2 = manual }
    }
    else{
        if (node.st1&node.std1){
            node.fillColor1 = toPos
        }else if(node.st1^node.std1){
            node.fillColor1 = outPos
        }else if(node.inSt1){
            node.fillColor1 = inPos
        }else{
            node.fillColor1 = ready
        }

        if (node.st2&node.std2){
            node.fillColor2 = toPos
        }else if(node.st2^node.std2){
            node.fillColor2 = outPos
        }else if(node.inSt2){
            node.fillColor2 = inPos
        }else{
            node.fillColor2 = ready
        }
    }
}
function isString(val) {
    return (typeof val === "string" || val instanceof String);
}
function isInArea(p, size, point){
    var result = false;
    var j = size - 1;
    for (let i = 0; i < size; i++) {
        if ((p[i].Y < point.Y && p[j].Y >= point.Y || p[j].Y < point.Y && p[i].Y >= point.Y) &&
                (p[i].X + (point.Y - p[i].Y) / (p[j].Y - p[i].Y) * (p[j].X - p[i].X) < point.X))
            result = !result;
        j = i;
    }
    return result;
}
