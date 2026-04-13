import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Script 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniScribe Execution Engine v41.0.0"
    color: "#18181b" // Zinc 900

    property string alertMessage: ""

    Connections {
        target: OmniScriptEngine
        function onErrorOccurred(error) {
            alertMessage = "SYNTAX ERROR: " + error;
            alertTimer.start();
        }
        function onScriptExecutedSuccessfully() {
            alertMessage = ""; // Clear errors
        }
    }

    Timer {
        id: alertTimer
        interval: 5000
        onTriggered: alertMessage = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#27272a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "ECMAScript 7 Sandbox"
                color: "#e4e4e7"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Evaluate raw JavaScript against the OmniUI C++ API layer"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: alertMessage
                color: "#f43f5e"
                font.bold: true
                visible: alertMessage !== ""
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Code Editor
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f172a"
            radius: 8
            border.color: "#3f3f46"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label { text: "Script Editor"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: codeEditor
                        text: "// OmniScribe Quick Start\n// Use Omni.print() to output to the console.\n\nfunction calculateFibonacci(n) {\n    if (n <= 1) return n;\n    return calculateFibonacci(n - 1) + calculateFibonacci(n - 2);\n}\n\nvar num = 10;\nOmni.print('Fibonacci(' + num + ') is ' + calculateFibonacci(num));\n\n// You can return variables to see them in output\n'Execution Complete';"
                        color: "#e2e8f0"
                        font.family: "monospace"
                        font.pixelSize: 14
                        background: Rectangle { color: "transparent" }
                        selectByMouse: true
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Clear Editor"
                        onClicked: codeEditor.text = ""
                        ToolTip.visible: hovered; ToolTip.text: "Wipe all text from the editor"
                    }
                    Item { Layout.fillWidth: true }
                    Button {
                        text: "▶ Evaluate Script"
                        onClicked: OmniScriptEngine.evaluateCode(codeEditor.text)
                        ToolTip.visible: hovered; ToolTip.text: "Compile and execute the JavaScript block in the QJSEngine"
                    }
                }
            }
        }

        // RIGHT PANEL: Console Output
        Rectangle {
            Layout.preferredWidth: 400
            Layout.fillHeight: true
            color: "#1e1e24"
            radius: 8
            border.color: "#3f3f46"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Execution Console"; color: "#34d399"; font.bold: true; font.pixelSize: 18 }
                    Item { Layout.fillWidth: true }
                    Button {
                        text: "Clear"
                        onClicked: OmniScriptEngine.clearLog()
                        ToolTip.visible: hovered; ToolTip.text: "Clear the console output"
                    }
                }

                ListView {
                    id: consoleView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniScriptEngine.executionLog

                    onCountChanged: consoleView.positionViewAtEnd()

                    delegate: Label {
                        width: consoleView.width
                        text: {
                            if (modelData.type === "INPUT") return modelData.msg;
                            if (modelData.type === "PRINT") return modelData.msg;
                            if (modelData.type === "OUTPUT") return "< " + modelData.msg;
                            return modelData.msg;
                        }
                        color: {
                            if (modelData.type === "ERROR") return "#f43f5e"
                            if (modelData.type === "INPUT") return "#64748b"
                            if (modelData.type === "OUTPUT") return "#fbbf24"
                            if (modelData.type === "PRINT") return "#f8fafc"
                            return "#a1a1aa"
                        }
                        font.family: "monospace"
                        wrapMode: Text.Wrap
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: ">"; color: "#64748b"; font.bold: true; font.family: "monospace" }
                    TextField {
                        id: replInput
                        Layout.fillWidth: true
                        placeholderText: "REPL input (e.g. 2 + 2)"
                        color: "#e2e8f0"
                        font.family: "monospace"
                        background: Rectangle { color: "transparent" }
                        onAccepted: {
                            if (text.length > 0) {
                                OmniScriptEngine.evaluateCode(text);
                                text = "";
                            }
                        }
                    }
                }
            }
        }
    }
}
