import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Language 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniChat LLM Interface v41.0.0"
    color: "#18181b" // Zinc 900

    header: ToolBar {
        background: Rectangle { color: "#27272a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "OmniLLM Transformer Node"
                color: "#e4e4e7"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Interact with the framework's synthetic linguistic consciousness"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: OmniLinguisticModel.isGenerating ? "Synthesizing Tokens..." : "Awaiting Input"
                color: OmniLinguisticModel.isGenerating ? "#fbbf24" : "#34d399"
                font.bold: true
                font.pixelSize: 16
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Context & Config
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

                Label { text: "Model Configuration"; color: "#a78bfa"; font.bold: true; font.pixelSize: 18 }

                // Model Name
                ColumnLayout {
                    spacing: 5
                    Label { text: "Active Architecture"; color: "#a1a1aa" }
                    ComboBox {
                        Layout.fillWidth: true
                        model: ["OmniLLM-175B (Quantized)", "OmniGPT-4 Turbo", "Galactic-Llama-7B"]
                        onCurrentTextChanged: OmniLinguisticModel.modelName = currentText
                        enabled: !OmniLinguisticModel.isGenerating
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Temperature
                ColumnLayout {
                    spacing: 5
                    Label { text: "Temperature (Chaos/Creativity)"; color: "#a1a1aa" }
                    RowLayout {
                        Layout.fillWidth: true
                        Slider {
                            id: tempSlider
                            Layout.fillWidth: true
                            from: 0.0
                            to: 2.0
                            value: OmniLinguisticModel.temperature
                            onValueChanged: OmniLinguisticModel.temperature = value
                            ToolTip.visible: hovered; ToolTip.text: "0 = Rigid and deterministic. >1.2 = Hallucination threshold."
                        }
                        Label {
                            text: OmniLinguisticModel.temperature.toFixed(2)
                            color: OmniLinguisticModel.temperature > 1.2 ? "#f43f5e" : "#60a5fa"
                            font.bold: true
                        }
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Context Window
                ColumnLayout {
                    spacing: 5
                    Label { text: "Context Window Usage"; color: "#a1a1aa" }
                    Label {
                        text: OmniLinguisticModel.tokenCount + " / 128,000 Tokens"
                        color: "#38bdf8"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniLinguisticModel.tokenCount / 128000.0
                        background: Rectangle { color: "#3f3f46"; radius: 2 }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Estimated computational token cost of current conversation history"
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Clear Context"
                        Layout.fillWidth: true
                        onClicked: OmniLinguisticModel.clearHistory()
                        ToolTip.visible: hovered; ToolTip.text: "Wipe memory of current conversation and reset token counter"
                    }
                    Button {
                        text: "Halt Inference"
                        Layout.fillWidth: true
                        enabled: OmniLinguisticModel.isGenerating
                        onClicked: OmniLinguisticModel.cancelGeneration()
                        ToolTip.visible: hovered; ToolTip.text: "Interrupt the token stream"
                    }
                }
            }
        }

        // RIGHT PANEL: Chat Interface
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#18181b"
            radius: 8
            border.color: "#3f3f46"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                // History List
                ListView {
                    id: chatView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniLinguisticModel.chatHistory
                    spacing: 10

                    onCountChanged: chatView.positionViewAtEnd()

                    delegate: Rectangle {
                        width: chatView.width
                        height: chatText.implicitHeight + 20
                        color: modelData.role === "user" ? "#27272a" : (modelData.role === "system" ? "#1e1b4b" : "#0f172a")
                        radius: 8
                        border.color: "#3f3f46"

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 15

                            Label {
                                text: modelData.role.toUpperCase()
                                color: modelData.role === "user" ? "#fbbf24" : (modelData.role === "system" ? "#818cf8" : "#34d399")
                                font.bold: true
                                Layout.alignment: Qt.AlignTop
                                Layout.preferredWidth: 80
                            }

                            Label {
                                id: chatText
                                text: modelData.content
                                color: "#e4e4e7"
                                wrapMode: Text.Wrap
                                Layout.fillWidth: true
                                font.pixelSize: 15
                                lineHeight: 1.4
                            }
                        }
                    }
                }

                // Active Stream
                Rectangle {
                    Layout.fillWidth: true
                    height: streamText.implicitHeight + 20
                    color: "#0f172a"
                    radius: 8
                    border.color: "#38bdf8"
                    border.width: 2
                    visible: OmniLinguisticModel.isGenerating

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 15

                        Label {
                            text: "ASSISTANT"
                            color: "#34d399"
                            font.bold: true
                            Layout.alignment: Qt.AlignTop
                            Layout.preferredWidth: 80
                        }

                        Label {
                            id: streamText
                            text: OmniLinguisticModel.currentResponse + "█" // Cursor block
                            color: "#e4e4e7"
                            wrapMode: Text.Wrap
                            Layout.fillWidth: true
                            font.pixelSize: 15
                            lineHeight: 1.4
                        }
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Input Area
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10

                    TextField {
                        id: chatInput
                        Layout.fillWidth: true
                        Layout.preferredHeight: 50
                        placeholderText: "Query the OmniLLM..."
                        color: "#e4e4e7"
                        enabled: !OmniLinguisticModel.isGenerating
                        onAccepted: sendBtn.clicked()
                    }

                    Button {
                        id: sendBtn
                        text: "SEND"
                        Layout.preferredHeight: 50
                        Layout.preferredWidth: 100
                        enabled: !OmniLinguisticModel.isGenerating && chatInput.text.length > 0
                        onClicked: {
                            OmniLinguisticModel.sendMessage(chatInput.text);
                            chatInput.text = "";
                        }
                        ToolTip.visible: hovered; ToolTip.text: "Send query to the transformer context window"
                    }
                }
            }
        }
    }
}
