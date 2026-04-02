import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Gfx 1.0 // Map 3D to Gfx namespace since we created it in 3d folder

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "Omni3D Rendering Engine v41.0.0"
    color: "#0f172a" // Slate 900

    OmniCamera {
        id: mainCamera
        position: Qt.vector3d(0, 0, -10) // Looking from -Z towards +Z
        target: Qt.vector3d(0, 0, 0)
        fov: 60.0
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Software 3D Rasterizer"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Custom built CPU-based 3D rendering pipeline"
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Spawn Object"
                onClicked: {
                    var comp = Qt.createComponent("TestCube.qml");
                    var obj = comp.createObject(viewport);
                    viewport.addMesh(obj);
                }
            }
            Button {
                text: "Clear Scene"
                onClicked: viewport.clearScene()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Camera & Scene Settings
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#1e293b"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Scene Settings"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Camera Position X
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Label { text: "Camera X:"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: mainCamera.position.x.toFixed(1); color: "#f8fafc" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: -20; to: 20; value: mainCamera.position.x
                        onValueChanged: mainCamera.position = Qt.vector3d(value, mainCamera.position.y, mainCamera.position.z)
                    }
                }

                // Camera Position Y
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Label { text: "Camera Y (Elevation):"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: mainCamera.position.y.toFixed(1); color: "#f8fafc" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: -20; to: 20; value: mainCamera.position.y
                        onValueChanged: mainCamera.position = Qt.vector3d(mainCamera.position.x, value, mainCamera.position.z)
                    }
                }

                // Camera Position Z
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Label { text: "Camera Z (Depth):"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: mainCamera.position.z.toFixed(1); color: "#f8fafc" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: -20; to: 20; value: mainCamera.position.z
                        onValueChanged: mainCamera.position = Qt.vector3d(mainCamera.position.x, mainCamera.position.y, value)
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                Label { text: "Renderer Stats"; color: "#a1a1aa"; font.bold: true }

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Active Meshes:"; color: "#94a3b8" }
                    Item { Layout.fillWidth: true }
                    Label { text: viewport.activeMeshes; color: "#60a5fa"; font.bold: true }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Vertex Count:"; color: "#94a3b8" }
                    Item { Layout.fillWidth: true }
                    Label { text: viewport.vertexCount; color: "#fb923c"; font.bold: true }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // RIGHT PANEL: The Viewport
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#050505"
            radius: 8
            border.color: "#334155"
            border.width: 2
            clip: true

            OmniView3D {
                id: viewport
                anchors.fill: parent
                camera: mainCamera
                wireframeMode: true
                renderLoop: true

                Component.onCompleted: {
                    var comp = Qt.createComponent("TestCube.qml");
                    viewport.addMesh(comp.createObject(viewport, { "position": Qt.vector3d(0, 0, 0) }));
                }
            }
        }
    }
}
