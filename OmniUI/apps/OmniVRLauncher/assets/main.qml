import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.VR 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniVR Launcher v41.0.0"
    color: "#0f172a" // Slate 900

    property string alertMessage: ""
    property bool isSick: OmniVRSystem.motionSickness > 80

    Connections {
        target: OmniVRSystem
        function onSicknessChanged() {
            if (OmniVRSystem.motionSickness > 95) {
                alertMessage = "CRITICAL: Sim Sickness Overload. Emergency Disconnect.";
                alertTimer.start();
            } else if (OmniVRSystem.motionSickness > 80) {
                alertMessage = "WARNING: Vestibular Mismatch High. Nausea Imminent.";
            } else {
                alertMessage = "";
            }
        }
    }

    Timer {
        id: alertTimer
        interval: 4000
        onTriggered: alertMessage = ""
    }

    // Nausea Effect Overlay
    Rectangle {
        anchors.fill: parent
        color: "#166534" // Sickly green
        opacity: isSick ? (OmniVRSystem.motionSickness - 80) / 40.0 : 0.0
        z: 100
        visible: opacity > 0
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "OmniXR Full-Dive Dashboard"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Manage your Virtual/Augmented Reality sessions"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: alertMessage
                color: "#ef4444"
                font.bold: true
                visible: alertMessage !== ""
                z: 101
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: HMD Tracking & Telemetry
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

                Label { text: "Headset Telemetry"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Tracking Status
                ColumnLayout {
                    spacing: 5
                    Label { text: "Tracking Status"; color: "#94a3b8" }
                    Label {
                        text: OmniVRSystem.hmdStatus
                        color: OmniVRSystem.active ? "#22c55e" : "#94a3b8"
                        font.pixelSize: 18
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Camera inside-out tracking state"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // FPS
                ColumnLayout {
                    spacing: 5
                    Label { text: "Display Refresh Rate (FPS)"; color: "#94a3b8" }
                    Label {
                        text: OmniVRSystem.framerate.toFixed(1) + " Hz"
                        color: OmniVRSystem.framerate < 90.0 ? "#ef4444" : "#a3e635"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniVRSystem.framerate / 144.0
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Below 90Hz causes severe motion sickness in VR."
                }

                // Sickness Meter
                ColumnLayout {
                    spacing: 5
                    Label { text: "Vestibular Disconnect (Nausea)"; color: "#94a3b8" }
                    Label {
                        text: OmniVRSystem.motionSickness.toFixed(1) + "%"
                        color: OmniVRSystem.motionSickness > 50.0 ? "#ef4444" : "#cbd5e1"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniVRSystem.motionSickness / 100.0
                        background: Rectangle { color: "#334155"; radius: 2 }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Simulated inner-ear discomfort caused by low FPS, high latency, or artificial locomotion."
                }

                Item { Layout.fillHeight: true }

                Label { text: "Session Controls"; color: "#94a3b8"; font.bold: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Start VR Link"
                        Layout.fillWidth: true
                        enabled: !OmniVRSystem.active
                        onClicked: OmniVRSystem.startSession("Metaverse Core")
                        ToolTip.visible: hovered; ToolTip.text: "Initialize the VR runtime and engage displays"
                    }
                    Button {
                        text: "End Session"
                        Layout.fillWidth: true
                        enabled: OmniVRSystem.active
                        onClicked: OmniVRSystem.endSession()
                        ToolTip.visible: hovered; ToolTip.text: "Drop to passthrough or disconnect"
                    }
                }
            }
        }

        // RIGHT PANEL: Game Sim & Log
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // Interaction Testing
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: "#0f172a"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label { text: "Locomotion & Hardware Stress Test"; color: "#fb923c"; font.bold: true; font.pixelSize: 18 }

                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "HMD Spatial Pose: [" + OmniVRSystem.hmdPosition.x.toFixed(2) + ", " + OmniVRSystem.hmdPosition.y.toFixed(2) + ", " + OmniVRSystem.hmdPosition.z.toFixed(2) + "]m"; color: "#94a3b8" }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        Button {
                            text: "Smooth Walk Forward"
                            enabled: OmniVRSystem.active
                            onClicked: OmniVRSystem.simulateMovement(0.0, 0.0, -1.0)
                            ToolTip.visible: hovered; ToolTip.text: "Moves the virtual camera without physical body movement. Highly nauseating."
                        }
                        Button {
                            text: "Snap Turn"
                            enabled: OmniVRSystem.active
                            onClicked: OmniVRSystem.simulateMovement(1.0, 0.0, 0.0)
                            ToolTip.visible: hovered; ToolTip.text: "Instantly rotates the camera. Safer for the inner ear."
                        }
                    }

                    Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        Button {
                            text: "Simulate Thermal Throttle (Drop FPS)"
                            enabled: OmniVRSystem.active
                            onClicked: OmniVRSystem.triggerFrameDrop()
                            ToolTip.visible: hovered; ToolTip.text: "Forces GPU to drop frames, ruining immersion and causing immediate sickness."
                        }
                        Button {
                            text: "Inject Network Latency"
                            enabled: OmniVRSystem.active
                            onClicked: OmniVRSystem.injectLatency()
                            ToolTip.visible: hovered; ToolTip.text: "Increases photon-to-motion latency. When you turn your head, the world lags behind."
                        }
                    }
                }
            }

            // Runtime Log
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#1e293b"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label {
                        text: "OmniXR Diagnostic Output"
                        color: "#a78bfa"
                        font.bold: true
                    }

                    ListView {
                        id: logView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: OmniVRSystem.log

                        delegate: RowLayout {
                            width: logView.width
                            height: 25
                            spacing: 10

                            Label {
                                text: "[" + modelData.level + "]"
                                color: {
                                    if (modelData.level === "ERROR") return "#ef4444"
                                    if (modelData.level === "WARN") return "#fbbf24"
                                    if (modelData.level === "SUCCESS") return "#34d399"
                                    return "#64748b"
                                }
                                font.bold: true
                                font.family: "monospace"
                                Layout.preferredWidth: 80
                            }

                            Label {
                                text: modelData.msg
                                color: "#e2e8f0"
                                font.family: "monospace"
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }
    }
}
