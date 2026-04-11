import QtQuick 2.15
import Omni.Physics 1.0

OmniRigidBody {
    id: root
    width: 40
    height: 40
    mass: 1.0
    restitution: 0.9 // Very bouncy
    friction: 0.1
    shape: "Rectangle"

    Rectangle {
        anchors.fill: parent
        color: "#f43f5e" // Rose 500
        radius: 8
        border.color: "#9f1239"
        border.width: 2

        // Bounce effect
        SequentialAnimation {
            id: bounceAnim
            NumberAnimation { target: scaleTransform; property: "yScale"; to: 0.7; duration: 50; easing.type: Easing.OutQuad }
            NumberAnimation { target: scaleTransform; property: "yScale"; to: 1.1; duration: 50; easing.type: Easing.OutBack }
            NumberAnimation { target: scaleTransform; property: "yScale"; to: 1.0; duration: 50; easing.type: Easing.OutBounce }
        }

        transform: Scale { id: scaleTransform; origin.x: 20; origin.y: 40 }
    }

    onCollided: function(other, impactSpeed) {
        if (impactSpeed > 2.0) {
            bounceAnim.start();
        }
    }
}
