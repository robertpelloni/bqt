import BobUIQuick 2.15
import Omni.Gfx 1.0

OmniMesh {
    id: root
    source: "Cube"
    scale: BobUI.vector3d(2, 2, 2)

    // Rotate constantly
    NumberAnimation on rotation.x {
        from: 0; to: 360; duration: 5000; loops: Animation.Infinite
    }
    NumberAnimation on rotation.y {
        from: 0; to: 360; duration: 7000; loops: Animation.Infinite
    }
}
