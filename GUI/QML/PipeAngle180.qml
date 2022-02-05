import QtQuick 2.15
import "fap.js" as Fap

PipeAngle360 {
    width: 300
    height: 150
    endAmgle: 180
    xCenter: width / 2
    yCenter: 0
    radius: Math.min(width, height)
}
