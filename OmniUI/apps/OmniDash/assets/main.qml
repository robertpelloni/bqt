import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.System 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniDash System Monitor v41.0.0"
    color: "#0f172a" // Slate 900

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Local Machine Dashboard"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Hardware utilization and system properties"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: OmniSystem.machineHostName + " (" + OmniSystem.osName + " " + OmniSystem.osVersion + ")"
                color: "#94a3b8"
                font.bold: true
                font.pixelSize: 14
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Core Hardware
        Rectangle {
            Layout.preferredWidth: 400
            Layout.fillHeight: true
            color: "#1e293b"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Resource Utilization"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // CPU
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "CPU Load (" + OmniSystem.cpuArchitecture + ")"; color: "#cbd5e1" }
                    RowLayout {
                        Layout.fillWidth: true
                        ProgressBar {
                            Layout.fillWidth: true
                            value: OmniSystem.cpuUsage / 100.0
                            background: Rectangle { color: "#334155"; radius: 2 }
                        }
                        Label { text: OmniSystem.cpuUsage.toFixed(1) + "%"; color: OmniSystem.cpuUsage > 85 ? "#ef4444" : "#60a5fa"; font.bold: true; Layout.preferredWidth: 50 }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Current processing thread allocation"
                }

                // RAM
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Memory (RAM)"; color: "#cbd5e1" }
                    RowLayout {
                        Layout.fillWidth: true
                        ProgressBar {
                            Layout.fillWidth: true
                            value: OmniSystem.ramUsage / 100.0
                            background: Rectangle { color: "#334155"; radius: 2 }
                        }
                        Label { text: OmniSystem.ramUsage.toFixed(1) + "%"; color: OmniSystem.ramUsage > 85 ? "#ef4444" : "#a78bfa"; font.bold: true; Layout.preferredWidth: 50 }
                    }
                }

                // DISK
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Primary Storage I/O"; color: "#cbd5e1" }
                    RowLayout {
                        Layout.fillWidth: true
                        ProgressBar {
                            Layout.fillWidth: true
                            value: OmniSystem.diskUsage / 100.0
                            background: Rectangle { color: "#334155"; radius: 2 }
                        }
                        Label { text: OmniSystem.diskUsage.toFixed(1) + "%"; color: "#fb923c"; font.bold: true; Layout.preferredWidth: 50 }
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                Label { text: "Power Management"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Battery
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Battery Status"; color: "#cbd5e1" }
                    RowLayout {
                        Layout.fillWidth: true
                        ProgressBar {
                            Layout.fillWidth: true
                            value: OmniSystem.batteryLevel / 100.0
                            background: Rectangle { color: "#334155"; radius: 2 }
                        }
                        Label { text: OmniSystem.batteryLevel + "%"; color: OmniSystem.batteryLevel < 20 ? "#ef4444" : "#34d399"; font.bold: true; Layout.preferredWidth: 50 }
                    }
                    Label { text: OmniSystem.isCharging ? "Charging (A/C Connected)" : "Discharging (Battery)"; color: OmniSystem.isCharging ? "#34d399" : "#fbbf24"; font.pixelSize: 12 }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Toggle AC Power"
                        Layout.fillWidth: true
                        onClicked: OmniSystem.togglePowerPlug()
                        ToolTip.visible: hovered; ToolTip.text: "Simulate plugging/unplugging the power adapter"
                    }
                    Button {
                        text: "Run Burn-in Test"
                        Layout.fillWidth: true
                        onClicked: OmniSystem.runStressTest()
                        ToolTip.visible: hovered; ToolTip.text: "Peg CPU usage to 100% and watch thermals rise"
                    }
                }
            }
        }

        // RIGHT PANEL: Thermals
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#020617" // Pitch black
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label {
                    text: "Thermal Sensor Array"
                    color: "#f43f5e"
                    font.bold: true
                    font.pixelSize: 18
                }

                ListView {
                    id: thermalView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniSystem.thermalSensors
                    spacing: 15

                    delegate: Rectangle {
                        width: thermalView.width
                        height: 70
                        color: "#1e293b"
                        border.color: modelData.critical ? "#ef4444" : "#334155"
                        border.width: modelData.critical ? 2 : 1
                        radius: 8

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 15
                            spacing: 15

                            Label {
                                text: modelData.name
                                color: "#f8fafc"
                                font.bold: true
                                Layout.preferredWidth: 150
                            }

                            Rectangle { width: 1; Layout.fillHeight: true; color: "#334155" }

                            // Thermal gauge visualization
                            Rectangle {
                                Layout.fillWidth: true
                                height: 10
                                color: "#334155"
                                radius: 5

                                Rectangle {
                                    height: 10
                                    width: parent.width * (modelData.temp / 120.0) // 120C max
                                    radius: 5
                                    color: modelData.critical ? "#ef4444" : (modelData.temp > 70 ? "#fb923c" : "#38bdf8")

                                    // Smooth animation
                                    Behavior on width { NumberAnimation { duration: 1000 } }
                                    Behavior on color { ColorAnimation { duration: 500 } }
                                }
                            }

                            Label {
                                text: modelData.temp.toFixed(1) + " °C"
                                color: modelData.critical ? "#ef4444" : "#e2e8f0"
                                font.bold: true
                                font.family: "monospace"
                                Layout.preferredWidth: 80
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                }
            }
        }
    }
}
