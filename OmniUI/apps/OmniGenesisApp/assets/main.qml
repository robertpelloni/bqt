import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Genesis 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniGenesis Cosmic Simulator v41.0.0"
    color: "#0a0a0a"

    header: ToolBar {
        background: Rectangle { color: "#171717" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Genesis Engine"
                color: "#e5e5e5"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Universal lifecycle and thermodynamic control panel"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Cosmic Era: " + OmniSimulationLoop.metaState
                color: {
                    if (OmniSimulationLoop.metaState === "Singularity") return "#d946ef"
                    if (OmniSimulationLoop.metaState === "Inflationary Epoch") return "#facc15"
                    if (OmniSimulationLoop.metaState === "Heat Death") return "#52525b"
                    if (OmniSimulationLoop.metaState === "Big Crunch") return "#ef4444"
                    return "#3b82f6"
                }
                font.bold: true
                font.pixelSize: 18
                ToolTip.visible: hovered
                ToolTip.text: "Current dominant physical paradigm of the cosmos"
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Universe Parameters
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#171717"
            radius: 8
            border.color: "#262626"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Thermodynamics"; color: "#fb923c"; font.bold: true; font.pixelSize: 18 }

                // Cycle
                ColumnLayout {
                    spacing: 5
                    Label { text: "Iteration Cycle"; color: "#a3a3a3" }
                    Label {
                        text: "Brahman Breath #" + OmniSimulationLoop.cycleCount
                        color: "#e5e5e5"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Number of Big Bang/Big Crunch cycles completed"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#262626" }

                // Usable Energy
                ColumnLayout {
                    spacing: 5
                    Label { text: "Available Exergy (Work Energy)"; color: "#a3a3a3" }
                    Label {
                        text: OmniSimulationLoop.energy.toFixed(2) + " %"
                        color: OmniSimulationLoop.energy > 50 ? "#4ade80" : "#f87171"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniSimulationLoop.energy / 100
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Energy available for stellar ignition and biological life"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#262626" }

                // Entropy
                ColumnLayout {
                    spacing: 5
                    Label { text: "Universal Entropy"; color: "#a3a3a3" }
                    Label {
                        text: "S = " + OmniSimulationLoop.entropy.toFixed(4)
                        color: "#c084fc"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniSimulationLoop.entropy
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Total disorder/chaos in the closed system. Reaching 1.0 equals Heat Death."
                }

                Item { Layout.fillHeight: true }

                Label { text: "God Controls"; color: "#a3a3a3" }

                RowLayout {
                    Layout.fillWidth: true
                    Button {
                        text: "Big Bang"
                        Layout.fillWidth: true
                        onClicked: OmniSimulationLoop.initiateBigBang()
                        enabled: OmniSimulationLoop.metaState === "Singularity"
                        ToolTip.visible: hovered; ToolTip.text: "Explode the singularity into an inflationary spacetime manifold"
                    }
                    Button {
                        text: "Big Crunch"
                        Layout.fillWidth: true
                        onClicked: OmniSimulationLoop.triggerBigCrunch()
                        enabled: OmniSimulationLoop.metaState !== "Singularity"
                        ToolTip.visible: hovered; ToolTip.text: "Reverse dark energy expansion; collapse universe back into a point"
                    }
                }

                Button {
                    text: "Accelerate Entropy (+dS)"
                    Layout.fillWidth: true
                    onClicked: OmniSimulationLoop.accelerateHeatDeath()
                    ToolTip.visible: hovered; ToolTip.text: "Force massive thermodynamic decay (fast forward to Heat Death)"
                }
            }
        }

        // RIGHT PANEL: Epoch History Log
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#171717"
            radius: 8
            border.color: "#262626"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label {
                    text: "Cosmological Epoch Timeline"
                    color: "#67e8f9"
                    font.bold: true
                    font.pixelSize: 18
                }

                ListView {
                    id: historyView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniSimulationLoop.epochHistory

                    delegate: Rectangle {
                        width: historyView.width
                        height: 60
                        color: index % 2 === 0 ? "#262626" : "#171717"
                        border.color: "#404040"
                        border.width: 1

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 15

                            Label {
                                text: "[" + modelData.timestamp + "]"
                                color: "#52525b"
                                font.family: "monospace"
                            }

                            Rectangle { width: 2; Layout.fillHeight: true; color: "#404040" }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2
                                Label {
                                    text: "Cycle " + modelData.cycle + " | " + modelData.epoch
                                    color: "#e5e5e5"
                                    font.bold: true
                                }
                                RowLayout {
                                    Label { text: "Energy: " + modelData.energy; color: "#fb923c"; font.pixelSize: 12 }
                                    Label { text: "Entropy: " + modelData.entropy; color: "#c084fc"; font.pixelSize: 12 }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
