import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Universe 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniUniverse God Mode Console v41.0.0"
    color: "#0f172a" // Slate 900

    property string alertMessage: ""

    Connections {
        target: OmniFundamentalConstants
        function onPhysicsBroken(reason) {
            alertMessage = "UNIVERSE COLLAPSED: " + reason;
            alertTimer.start();
        }
    }

    Timer {
        id: alertTimer
        interval: 4000
        onTriggered: alertMessage = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Fundamental Laws Controller"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Alter the foundational constants of spacetime"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: alertMessage
                color: "#ef4444"
                font.bold: true
                visible: alertMessage !== ""
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Constants Sliders
        Rectangle {
            Layout.preferredWidth: 450
            Layout.fillHeight: true
            color: "#1e293b"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Physical Constants"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Speed of Light
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Speed of Light (c)"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: OmniFundamentalConstants.speedOfLight.toExponential(4) + " m/s"; color: "#f8fafc"; font.bold: true; font.family: "monospace" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: 1000.0
                        to: 999999999.0
                        value: OmniFundamentalConstants.speedOfLight
                        onValueChanged: OmniFundamentalConstants.speedOfLight = value
                        ToolTip.visible: hovered; ToolTip.text: "Maximum speed of causality in the vacuum"
                    }
                }

                // Gravity
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Gravitational Constant (G)"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: OmniFundamentalConstants.gravitationalConstant.toExponential(4) + " N⋅m²/kg²"; color: "#f8fafc"; font.bold: true; font.family: "monospace" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: 1e-15
                        to: 1e-8
                        value: OmniFundamentalConstants.gravitationalConstant
                        onValueChanged: OmniFundamentalConstants.gravitationalConstant = value
                        ToolTip.visible: hovered; ToolTip.text: "Strength of gravitational attraction between masses"
                    }
                }

                // Fine Structure
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Fine Structure Constant (α)"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: OmniFundamentalConstants.fineStructureConstant.toExponential(4); color: "#f8fafc"; font.bold: true; font.family: "monospace" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: 0.0001
                        to: 0.2
                        value: OmniFundamentalConstants.fineStructureConstant
                        onValueChanged: OmniFundamentalConstants.fineStructureConstant = value
                        ToolTip.visible: hovered; ToolTip.text: "Strength of the electromagnetic interaction"
                    }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Evaluate Viability"
                        Layout.fillWidth: true
                        onClicked: OmniFundamentalConstants.calculateConsequences()
                        ToolTip.visible: hovered; ToolTip.text: "Run anthropic principle check on the current laws of physics"
                    }
                    Button {
                        text: "Reset to Standard Model"
                        Layout.fillWidth: true
                        onClicked: OmniFundamentalConstants.resetToStandardModel()
                        ToolTip.visible: hovered; ToolTip.text: "Revert all constants to Earth-Standard values"
                    }
                }
            }
        }

        // RIGHT PANEL: Reality Sandbox Output
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f172a"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label {
                    text: "OmniUniverse Metaphysical Log"
                    color: "#a78bfa"
                    font.bold: true
                    font.pixelSize: 18
                }

                ListView {
                    id: logView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniFundamentalConstants.log

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
                            wrapMode: Text.Wrap
                        }
                    }
                }
            }
        }
    }
}
