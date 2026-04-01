import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Net 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniNet WebSocket Diagnostics v41.0.0"
    color: "#0f172a" // Slate 900

    property string alertMessage: ""

    Connections {
        target: OmniWebSocket
        function onErrorOccurred(error) {
            alertMessage = "NETWORK FAULT: " + error;
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
                text: "Real-Time Packet Streamer"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "WSS JSON RPC debugging and network telemetry"
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

        // LEFT PANEL: Connection Config
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

                Label { text: "Connection Broker"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // WSS URL
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "WebSocket Endpoint URI"; color: "#94a3b8" }
                    TextField {
                        Layout.fillWidth: true
                        text: OmniWebSocket.url
                        color: "#f8fafc"
                        enabled: !OmniWebSocket.connected
                        onTextChanged: OmniWebSocket.url = text
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // Status
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Link Status"; color: "#94a3b8" }
                    Label {
                        text: OmniWebSocket.status
                        color: OmniWebSocket.connected ? "#34d399" : "#fbbf24"
                        font.pixelSize: 18
                        font.bold: true
                    }
                }

                // Ping
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Round-Trip Latency (Ping)"; color: "#94a3b8" }
                    RowLayout {
                        Layout.fillWidth: true
                        ProgressBar {
                            Layout.fillWidth: true
                            value: OmniWebSocket.latency / 500.0 // 500ms max scale
                            background: Rectangle { color: "#334155"; radius: 2 }
                        }
                        Label {
                            text: OmniWebSocket.latency + " ms"
                            color: OmniWebSocket.latency > 200 ? "#ef4444" : "#60a5fa"
                            font.bold: true
                            Layout.preferredWidth: 60
                        }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Simulated network delay and packet jitter"
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Open WSS Connection"
                        Layout.fillWidth: true
                        enabled: !OmniWebSocket.connected
                        onClicked: OmniWebSocket.open()
                        ToolTip.visible: hovered; ToolTip.text: "Attempt to negotiate a TLS 1.3 secure web socket handshake"
                    }
                    Button {
                        text: "Disconnect"
                        Layout.fillWidth: true
                        enabled: OmniWebSocket.connected
                        onClicked: OmniWebSocket.close()
                        ToolTip.visible: hovered; ToolTip.text: "Gracefully sever the socket connection"
                    }
                }
            }
        }

        // RIGHT PANEL: Log & JSON Builder
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // Packet Stream
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
                        text: "Inbound / Outbound Packet Stream"
                        color: "#a78bfa"
                        font.bold: true
                        font.pixelSize: 18
                    }

                    ListView {
                        id: logView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: OmniWebSocket.log

                        onCountChanged: logView.positionViewAtEnd()

                        delegate: RowLayout {
                            width: logView.width
                            height: 25
                            spacing: 10

                            Label {
                                text: "[" + modelData.time + "]"
                                color: "#64748b"
                                font.family: "monospace"
                            }

                            Label {
                                text: modelData.dir
                                color: {
                                    if (modelData.dir === "IN") return "#34d399"
                                    if (modelData.dir === "OUT") return "#60a5fa"
                                    if (modelData.dir === "ERR") return "#ef4444"
                                    return "#fbbf24" // SYS
                                }
                                font.bold: true
                                font.family: "monospace"
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

            // JSON Payload Injector
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: "#1e293b"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 10

                    Label { text: "JSON RPC Payload Injector"; color: "#fb923c"; font.bold: true }

                    ScrollView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        TextArea {
                            id: jsonPayload
                            text: '{\n  "type": "authentication_request",\n  "token": "bearer_abc123",\n  "api_version": 2\n}'
                            color: "#f8fafc"
                            font.family: "monospace"
                            background: Rectangle { color: "#0f172a"; radius: 4; border.color: "#475569" }
                            enabled: OmniWebSocket.connected
                        }
                    }

                    Button {
                        text: "Send Payload"
                        Layout.alignment: Qt.AlignRight
                        enabled: OmniWebSocket.connected
                        onClicked: OmniWebSocket.send(jsonPayload.text)
                        ToolTip.visible: hovered; ToolTip.text: "Serialize the JSON block above and fire it through the active socket"
                    }
                }
            }
        }
    }
}
