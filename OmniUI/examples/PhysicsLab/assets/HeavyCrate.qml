import BobUIQuick 2.15
import Omni.Physics 1.0

OmniRigidBody {
    id: root
    width: 60
    height: 60
    mass: 50.0 // Very heavy
    restitution: 0.1 // Hardly bounces
    friction: 0.8 // High friction
    shape: "Rectangle"

    Rectangle {
        anchors.fill: parent
        color: "#ca8a04" // Yellow 600
        border.color: "#854d0e" // Yellow 800
        border.width: 4

        // Inner crate markings
        Rectangle {
            anchors.centerIn: parent
            width: parent.width - 12
            height: parent.height - 12
            border.color: "#854d0e"
            border.width: 2
            color: "transparent"

            // X mark
            Canvas {
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.strokeStyle = "#854d0e";
                    ctx.lineWidth = 2;
                    ctx.beginPath();
                    ctx.moveTo(0, 0); ctx.lineTo(width, height);
                    ctx.moveTo(width, 0); ctx.lineTo(0, height);
                    ctx.stroke();
                }
            }
        }
    }
}
