import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Physics 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniPhysics Simulation Lab v41.0.0"
    color: "#18181b" // Zinc 900

    OmniPhysicsWorld {
        id: physicsWorld
        running: true
        gravity: Qt.vector2d(0, 9.81) // Down
        timeScale: 1.0
    }

    header: ToolBar {
        background: Rectangle { color: "#27272a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Rigid Body Dynamics Lab"
                color: "#e4e4e7"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Custom 2D Impulse-Resolution Physics Engine"
            }

            Item { Layout.fillWidth: true }

            Button {
                text: physicsWorld.running ? "⏸ Pause Physics" : "▶ Resume Physics"
                onClicked: physicsWorld.running = !physicsWorld.running
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: World Settings
        Rectangle {
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            color: "#27272a"
            radius: 8
            border.color: "#3f3f46"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "World Variables"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Gravity
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Gravity (Y-Axis)"; color: "#a1a1aa" }
                    RowLayout {
                        Layout.fillWidth: true
                        Slider {
                            Layout.fillWidth: true
                            from: -20.0
                            to: 20.0
                            value: physicsWorld.gravity.y
                            onValueChanged: physicsWorld.gravity = Qt.vector2d(physicsWorld.gravity.x, value)
                        }
                        Label {
                            text: physicsWorld.gravity.y.toFixed(1) + " m/s²"
                            color: "#60a5fa"
                            font.bold: true
                            Layout.preferredWidth: 60
                        }
                    }
                }

                // Wind (X-Gravity)
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Wind Force (X-Axis)"; color: "#a1a1aa" }
                    RowLayout {
                        Layout.fillWidth: true
                        Slider {
                            Layout.fillWidth: true
                            from: -20.0
                            to: 20.0
                            value: physicsWorld.gravity.x
                            onValueChanged: physicsWorld.gravity = Qt.vector2d(value, physicsWorld.gravity.y)
                        }
                        Label {
                            text: physicsWorld.gravity.x.toFixed(1) + " m/s²"
                            color: "#fb923c"
                            font.bold: true
                            Layout.preferredWidth: 60
                        }
                    }
                }

                // Time Scale
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Time Scale (Slow Motion)"; color: "#a1a1aa" }
                    RowLayout {
                        Layout.fillWidth: true
                        Slider {
                            Layout.fillWidth: true
                            from: 0.1
                            to: 3.0
                            value: physicsWorld.timeScale
                            onValueChanged: physicsWorld.timeScale = value
                        }
                        Label {
                            text: physicsWorld.timeScale.toFixed(1) + " x"
                            color: "#a78bfa"
                            font.bold: true
                            Layout.preferredWidth: 60
                        }
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                Label { text: "Spawner"; color: "#a1a1aa" }

                Button {
                    text: "Spawn Bouncy Box"
                    Layout.fillWidth: true
                    onClicked: {
                        var comp = Qt.createComponent("BouncyBox.qml");
                        var obj = comp.createObject(sandbox, {
                            "x": Math.random() * (sandbox.width - 50),
                            "y": 50,
                            "world": physicsWorld
                        });
                    }
                }

                Button {
                    text: "Spawn Heavy Crate"
                    Layout.fillWidth: true
                    onClicked: {
                        var comp = Qt.createComponent("HeavyCrate.qml");
                        var obj = comp.createObject(sandbox, {
                            "x": Math.random() * (sandbox.width - 50),
                            "y": 50,
                            "world": physicsWorld
                        });
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // RIGHT PANEL: The Physics Sandbox
        Rectangle {
            id: sandbox
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f172a"
            radius: 8
            border.color: "#3f3f46"
            border.width: 2
            clip: true

            Label {
                text: "Active RigidBodies: " + physicsWorld.activeBodies
                color: "#64748b"
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 10
            }

            // The Floor (Static Body)
            OmniRigidBody {
                world: physicsWorld
                isStatic: true
                restitution: 0.5
                friction: 0.9
                x: 0
                y: sandbox.height - 40
                width: sandbox.width
                height: 40

                Rectangle {
                    anchors.fill: parent
                    color: "#3f3f46"
                    border.color: "#52525b"

                    // Diagonal stripes pattern for static floor
                    Repeater {
                        model: Math.floor(parent.width / 40)
                        Rectangle {
                            x: index * 40; y: 0; width: 20; height: parent.height
                            color: "#27272a"
                        }
                    }
                }
            }

            // Left Wall (Static)
            OmniRigidBody {
                world: physicsWorld; isStatic: true; restitution: 0.2; friction: 0.5
                x: -20; y: 0; width: 20; height: sandbox.height
            }

            // Right Wall (Static)
            OmniRigidBody {
                world: physicsWorld; isStatic: true; restitution: 0.2; friction: 0.5
                x: sandbox.width; y: 0; width: 20; height: sandbox.height
            }
        }
    }
}
