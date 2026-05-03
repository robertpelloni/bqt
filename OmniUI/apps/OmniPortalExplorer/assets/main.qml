import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Portal 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniPortal Stargate Command v41.0.0"
    color: "#0f172a" // Slate 900

    property string alertMessage: ""

    Connections {
        target: OmniPortalGate
        function onCriticalFailure(reason) {
            alertMessage = reason;
            alertTimer.start();
        }
    }

    Timer {
        id: alertTimer
        interval: 3000
        onTriggered: alertMessage = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Interdimensional Gateway"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Wormhole mechanics and transit hub"
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

        // LEFT PANEL: Gate Controls
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

                Label { text: "Wormhole Telemetry"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Status
                ColumnLayout {
                    spacing: 5
                    Label { text: "Gate Status:"; color: "#94a3b8" }
                    Label {
                        text: OmniPortalGate.status
                        color: {
                            if (OmniPortalGate.status === "Established") return "#34d399"
                            if (OmniPortalGate.status === "Dialing") return "#fbbf24"
                            return "#94a3b8"
                        }
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Current operating state of the singularity ring"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // Stability
                ColumnLayout {
                    spacing: 5
                    Label { text: "Structural Integrity (Stability)"; color: "#94a3b8" }
                    Label {
                        text: OmniPortalGate.stability.toFixed(1) + "%"
                        color: OmniPortalGate.stability < 30 ? "#f43f5e" : "#818cf8"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniPortalGate.stability / 100
                        background: Rectangle { color: "#334155"; radius: 2 }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Event horizon decay rate. If 0%, the wormhole collapses violently."
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // Power Draw
                ColumnLayout {
                    spacing: 5
                    Label { text: "Reactor Draw"; color: "#94a3b8" }
                    Label {
                        text: OmniPortalGate.energyDraw.toFixed(1) + " TeraJoules"
                        color: "#fb923c"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Energy required to maintain the spacetime puncture"
                }

                Item { Layout.fillHeight: true }

                Label { text: "Dialing Computer"; color: "#94a3b8"; font.bold: true }

                TextField {
                    id: dialAddress
                    Layout.fillWidth: true
                    placeholderText: "Target Dimension Coordinates (e.g. C-137)"
                    color: "#f8fafc"
                    text: OmniPortalGate.targetDimension
                    onTextChanged: OmniPortalGate.targetDimension = text
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Dial Coordinates"
                        Layout.fillWidth: true
                        enabled: !OmniPortalGate.open && OmniPortalGate.status === "Offline"
                        onClicked: OmniPortalGate.initiateDialingSequence()
                        ToolTip.visible: hovered; ToolTip.text: "Spin up the ring and attempt to puncture spacetime at the target coordinates"
                    }
                    Button {
                        text: "Collapse Gate"
                        Layout.fillWidth: true
                        enabled: OmniPortalGate.open || OmniPortalGate.status === "Dialing"
                        onClicked: OmniPortalGate.closeGate()
                        ToolTip.visible: hovered; ToolTip.text: "Cut power to the ring, snapping the wormhole shut"
                    }
                }

                Button {
                    text: "Inject Exotic Matter (Stabilize)"
                    Layout.fillWidth: true
                    enabled: OmniPortalGate.open
                    onClicked: OmniPortalGate.injectExoticMatter(15.0)
                    ToolTip.visible: hovered; ToolTip.text: "Inject negative-mass matter to prop the throat of the wormhole open longer"
                }
            }
        }

        // RIGHT PANEL: Visualizer & Logs
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // The Gate Visualizer
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#020617"
                radius: 8
                border.color: "#334155"
                border.width: 2
                clip: true

                // The physical ring
                Rectangle {
                    anchors.centerIn: parent
                    width: 300
                    height: 300
                    radius: 150
                    color: "transparent"
                    border.color: "#475569"
                    border.width: 20

                    // The event horizon
                    Rectangle {
                        anchors.centerIn: parent
                        width: 260
                        height: 260
                        radius: 130
                        color: OmniPortalGate.open ? "#0284c7" : "#020617"
                        opacity: OmniPortalGate.open ? (OmniPortalGate.stability / 100) : 1.0

                        // Rippling water effect logic (simplified for UI)
                        Behavior on opacity { NumberAnimation { duration: 500 } }

                        // If dialing, spin the ring
                        RotationAnimation on rotation {
                            loops: Animation.Infinite
                            from: 0; to: 360
                            duration: 2000
                            running: OmniPortalGate.status === "Dialing"
                        }
                    }

                    // Add chevrons
                    Repeater {
                        model: 7
                        Rectangle {
                            width: 20; height: 30; color: OmniPortalGate.open ? "#ef4444" : "#334155"
                            x: 140 - 10 + 160 * Math.sin(index * Math.PI * 2 / 7)
                            y: 140 - 15 - 160 * Math.cos(index * Math.PI * 2 / 7)
                            rotation: index * 360 / 7
                        }
                    }
                }
            }

            // Transit / Telemetry Log
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                color: "#1e293b"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Transit & Telemetry Log"; color: "#a78bfa"; font.bold: true }
                        Item { Layout.fillWidth: true }
                        Button {
                            text: "Send M.A.L.P Probe"
                            enabled: OmniPortalGate.open
                            onClicked: OmniPortalGate.sendProbe()
                            ToolTip.visible: hovered; ToolTip.text: "Send a robotic sensor drone through the active gate to report back"
                        }
                    }

                    ListView {
                        id: logView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: OmniPortalGate.transitLog

                        delegate: RowLayout {
                            width: logView.width
                            height: 25
                            spacing: 10

                            Label { text: "> " + modelData.msg; color: "#e2e8f0"; font.family: "monospace"; Layout.fillWidth: true; elide: Text.ElideRight }
                        }
                    }
                }
            }
        }
    }
}
