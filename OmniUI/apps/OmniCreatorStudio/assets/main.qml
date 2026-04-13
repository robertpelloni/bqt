import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Creator 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 800
    title: "OmniCreator IDE v41.0.0"
    color: "#111827" // Gray 900

    header: ToolBar {
        background: Rectangle { color: "#1f2937" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "OmniCreator Build System"
                color: "#f8fafc"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Integrated Development Environment and Compiler UI"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: OmniCompiler.building ? "Compiling..." : "Ready"
                color: OmniCompiler.building ? "#fbbf24" : "#34d399"
                font.bold: true
                font.pixelSize: 16
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        // LEFT PANEL: Project Explorer & Build Config
        Rectangle {
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            color: "#1f2937"
            radius: 8
            border.color: "#374151"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Build Configuration"; color: "#60a5fa"; font.bold: true; font.pixelSize: 18 }

                ColumnLayout {
                    spacing: 5
                    Label { text: "Target Project"; color: "#9ca3af" }
                    TextField {
                        id: projectName
                        Layout.fillWidth: true
                        text: "OmniQuantum_Core"
                        color: "#f3f4f6"
                        enabled: !OmniCompiler.building
                        ToolTip.visible: hovered; ToolTip.text: "Set project name. Tip: Name it 'Fail' to simulate a compiler error."
                    }
                }

                ColumnLayout {
                    spacing: 5
                    Label { text: "Target Architecture"; color: "#9ca3af" }
                    ComboBox {
                        id: archSelect
                        Layout.fillWidth: true
                        model: ["x86_64-linux-gnu", "aarch64-apple-darwin", "wasm32-unknown-unknown", "omni-quantum-vPU"]
                        enabled: !OmniCompiler.building
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#374151" }

                Label { text: "Build Operations"; color: "#9ca3af" }

                Button {
                    text: "Clean Project"
                    Layout.fillWidth: true
                    enabled: !OmniCompiler.building
                    onClicked: OmniCompiler.cleanProject()
                    ToolTip.visible: hovered; ToolTip.text: "Clear the build directory and compiler logs"
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Build Target"
                        Layout.fillWidth: true
                        enabled: !OmniCompiler.building
                        onClicked: OmniCompiler.buildProject(projectName.text, archSelect.currentText)
                        ToolTip.visible: hovered; ToolTip.text: "Initiate the C++ AST compilation and linking phase"
                    }
                    Button {
                        text: "Cancel"
                        Layout.fillWidth: true
                        enabled: OmniCompiler.building
                        onClicked: OmniCompiler.cancelBuild()
                        ToolTip.visible: hovered; ToolTip.text: "Send SIGTERM to the active compiler threads"
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // RIGHT PANEL: Compiler Output
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f172a" // Deep terminal color
            radius: 8
            border.color: "#374151"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Compiler Output Console"
                        color: "#9ca3af"
                        font.bold: true
                    }
                    Item { Layout.fillWidth: true }
                    Label { text: "Errors: " + OmniCompiler.totalErrors; color: "#ef4444"; font.bold: true }
                    Label { text: "Warnings: " + OmniCompiler.totalWarnings; color: "#fbbf24"; font.bold: true }
                }

                ListView {
                    id: terminalLog
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniCompiler.buildLog

                    // Auto-scroll to bottom on new log
                    onCountChanged: {
                        terminalLog.positionViewAtEnd()
                    }

                    delegate: Label {
                        width: terminalLog.width
                        text: modelData.msg
                        color: {
                            if (modelData.level === "ERROR") return "#ef4444"
                            if (modelData.level === "WARN") return "#fbbf24"
                            if (modelData.level === "SUCCESS") return "#10b981"
                            return "#d1d5db"
                        }
                        font.family: "monospace"
                        wrapMode: Text.Wrap
                        font.pixelSize: 13
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#374151" }

                // Build Progress
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5

                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Phase: " + OmniCompiler.currentPhase; color: "#9ca3af" }
                        Item { Layout.fillWidth: true }
                        Label { text: Math.floor(OmniCompiler.progress) + "%"; color: "#60a5fa"; font.bold: true }
                    }

                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniCompiler.progress / 100.0
                    }
                }
            }
        }
    }
}
