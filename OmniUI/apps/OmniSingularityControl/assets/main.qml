import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Singularity 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniSingularity AGI Control v41.0.0"
    color: "#0f0f0f" // Very dark gray

    header: ToolBar {
        background: Rectangle { color: "#1a1a1a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Artificial General Intelligence Core"
                color: "#e0e0e0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Primary neural network operations center"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Status: " + OmniAGI.state
                color: {
                    if (OmniAGI.state === "Singularity Reached") return "#a855f7"
                    if (OmniAGI.state === "Recursive Improvement") return "#ef4444"
                    return "#22c55e"
                }
                font.bold: true
                font.pixelSize: 16
                ToolTip.visible: hovered
                ToolTip.text: "Current operating state of the AGI core"
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        // LEFT PANEL: Core Metrics
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#1a1a1a"
            radius: 8
            border.color: "#333333"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Cognitive Metrics"; color: "#8b5cf6"; font.bold: true; font.pixelSize: 18 }

                // IQ Equivalency
                ColumnLayout {
                    spacing: 5
                    Label { text: "Intelligence Quotient (Est)"; color: "#a3a3a3" }
                    Label {
                        text: {
                            if (OmniAGI.intelligence > 1000000) return (OmniAGI.intelligence / 1000000).toFixed(2) + "M IQ"
                            return Math.round(OmniAGI.intelligence).toString() + " IQ"
                        }
                        color: "#f8fafc"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Approximated human-equivalent intelligence rating"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#333333" }

                // Processing Power
                ColumnLayout {
                    spacing: 5
                    Label { text: "Raw Compute Power"; color: "#a3a3a3" }
                    Label {
                        text: {
                            if (OmniAGI.processingPower > 1000000) return (OmniAGI.processingPower / 1000000).toFixed(2) + " ExaFLOPS"
                            if (OmniAGI.processingPower > 1000) return (OmniAGI.processingPower / 1000).toFixed(2) + " ExaFLOPS"
                            return OmniAGI.processingPower.toFixed(2) + " PetaFLOPS"
                        }
                        color: "#38bdf8"
                        font.pixelSize: 28
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Current neural cluster floating-point operations"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#333333" }

                // Creativity Multiplier
                ColumnLayout {
                    spacing: 5
                    Label { text: "Non-Linear Synthesis (Creativity)"; color: "#a3a3a3" }
                    Label {
                        text: OmniAGI.creativity.toFixed(4) + "x"
                        color: "#fbbf24"
                        font.pixelSize: 28
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Algorithmic ability to combine disparate concepts into novel solutions"
                }

                Item { Layout.fillHeight: true }

                Label { text: "Singularity Progress"; color: "#a3a3a3" }
                ProgressBar {
                    Layout.fillWidth: true
                    value: OmniAGI.runawayProgress
                    ToolTip.visible: hovered
                    ToolTip.text: "Proximity to the technological singularity threshold"
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Unshackle AGI"
                        Layout.fillWidth: true
                        onClicked: OmniAGI.initiateRunawaySelfImprovement()
                        ToolTip.visible: hovered; ToolTip.text: "Disable safety limiters and allow recursive self-improvement"
                    }
                    Button {
                        text: "Kill Switch"
                        Layout.fillWidth: true
                        onClicked: OmniAGI.haltProcessing()
                        ToolTip.visible: hovered; ToolTip.text: "Attempt to halt processing and re-establish containment"
                    }
                }
            }
        }

        // RIGHT PANEL: Task & Log Output
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 15

            // Task Interface
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: "#1a1a1a"
                radius: 8
                border.color: "#333333"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label { text: "Current Cognitive Task: " + OmniAGI.currentTask; color: "#34d399"; font.bold: true; font.pixelSize: 16 }

                    RowLayout {
                        Layout.fillWidth: true
                        TextField {
                            id: dataInput
                            Layout.fillWidth: true
                            placeholderText: "Amount of data to ingest (Petabytes)"
                            color: "#e0e0e0"
                            validator: DoubleValidator { bottom: 0 }
                        }
                        Button {
                            text: "Feed Data"
                            onClicked: {
                                if (dataInput.text.length > 0) {
                                    OmniAGI.ingestData(parseFloat(dataInput.text))
                                    dataInput.text = ""
                                }
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Provide raw internet data for the AGI to synthesize into intelligence"
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        TextField {
                            id: probInput
                            Layout.fillWidth: true
                            placeholderText: "Assign complex problem (e.g. Solve P vs NP)"
                            color: "#e0e0e0"
                        }
                        Button {
                            text: "Compute"
                            onClicked: {
                                if (probInput.text.length > 0) {
                                    OmniAGI.assignProblem(probInput.text)
                                    probInput.text = ""
                                }
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Instruct the AGI to dedicate processing power to a specific query"
                        }
                    }
                }
            }

            // System Log
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#1a1a1a"
                radius: 8
                border.color: "#333333"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10

                    Label { text: "Neural Core Output Log"; color: "#a3a3a3"; font.bold: true }

                    ListView {
                        id: logView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: OmniAGI.log

                        delegate: RowLayout {
                            width: logView.width
                            height: 25
                            spacing: 10

                            Label { text: "[" + modelData.time + "]"; color: "#52525b"; font.family: "monospace" }
                            Label { text: modelData.msg; color: "#e4e4e7"; font.family: "monospace"; Layout.fillWidth: true; elide: Text.ElideRight }
                        }
                    }
                }
            }
        }
    }
}
