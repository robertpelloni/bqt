import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Omega 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "OmniOmega Unified Nexus v41.0.0"
    color: "#020617" // Slate 950

    property bool isRebooting: false

    Connections {
        target: OmniUniversalContext
        function onSystemRebooting() {
            isRebooting = true;
            rebootTimer.start();
        }
    }

    Timer {
        id: rebootTimer
        interval: 3000
        onTriggered: {
            isRebooting = false;
            OmniUniversalContext.setGlobalVar("Security Level", "Omega");
            OmniUniversalContext.setGlobalVar("System Entropy", 0.00);
        }
    }

    // Reboot Overlay
    Rectangle {
        anchors.fill: parent
        color: "black"
        z: 100
        visible: isRebooting

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20
            BusyIndicator { Layout.alignment: Qt.AlignHCenter; running: true }
            Label { text: "PERFORMING SYSTEM-WIDE METAPHYSICAL REBOOT..."; color: "#f8fafc"; font.pixelSize: 24; font.bold: true; font.letterSpacing: 3 }
        }
    }

    header: ToolBar {
        background: Rectangle { color: "#0f172a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "OmniOmega Nexus Core"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Central Integration hub connecting all OmniUI modules"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Global Focus: " + OmniUniversalContext.activeModule
                color: "#38bdf8"
                font.bold: true
                font.pixelSize: 16
                ToolTip.visible: hovered
                ToolTip.text: "Currently focused subsystem across the universal context"
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Module Registry
        Rectangle {
            Layout.preferredWidth: 400
            Layout.fillHeight: true
            color: "#0f172a"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Label { text: "Subsystem Registry"; color: "#818cf8"; font.bold: true; font.pixelSize: 18 }

                ListView {
                    id: moduleList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniUniversalContext.moduleRegistry

                    delegate: Rectangle {
                        width: moduleList.width
                        height: 50
                        color: index % 2 === 0 ? "#1e293b" : "#0f172a"
                        border.color: "#334155"
                        border.width: 1
                        radius: 4

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 10

                            Label {
                                text: modelData.id
                                color: "#f8fafc"
                                font.bold: true
                                Layout.preferredWidth: 150
                            }

                            Rectangle { width: 1; Layout.fillHeight: true; color: "#334155" }

                            Label {
                                text: modelData.status
                                color: {
                                    if (modelData.status === "Active") return "#34d399"
                                    if (modelData.status === "Suspended") return "#94a3b8"
                                    if (modelData.status === "WARNING") return "#f43f5e"
                                    return "#fbbf24"
                                }
                                Layout.preferredWidth: 80
                            }

                            Rectangle { width: 1; Layout.fillHeight: true; color: "#334155" }

                            Label {
                                text: modelData.memUsage.toFixed(1) + " MB"
                                color: "#cbd5e1"
                                font.pixelSize: 12
                                Layout.fillWidth: true
                                horizontalAlignment: Text.AlignRight
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: OmniUniversalContext.activeModule = modelData.id
                            hoverEnabled: true
                            ToolTip.visible: containsMouse
                            ToolTip.text: "Click to set " + modelData.id + " as the Global Focus target"
                        }
                    }
                }
            }
        }

        // RIGHT PANEL: Global State & Broadcasts
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // Global State Variables
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#0f172a"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label { text: "Universal Context Variables"; color: "#fb923c"; font.bold: true; font.pixelSize: 18 }

                    GridView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        cellWidth: parent.width / 2
                        cellHeight: 80
                        model: OmniUniversalContext.globalStateList
                        clip: true

                        delegate: Rectangle {
                            width: parent.width / 2 - 10
                            height: 70
                            color: "#1e293b"
                            radius: 4
                            border.color: "#334155"

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 5
                                Label { text: modelData.key; color: "#94a3b8"; font.bold: true }
                                Label { text: modelData.value; color: "#e2e8f0"; font.pixelSize: 16 }
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        TextField { id: varKey; placeholderText: "Key"; Layout.fillWidth: true; color: "#e2e8f0" }
                        TextField { id: varVal; placeholderText: "Value"; Layout.fillWidth: true; color: "#e2e8f0" }
                        Button {
                            text: "Set Global Var"
                            onClicked: {
                                if (varKey.text.length > 0) {
                                    OmniUniversalContext.setGlobalVar(varKey.text, varVal.text)
                                    varKey.text = ""; varVal.text = ""
                                }
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Inject a variable across the entire OmniUI ecosystem"
                        }
                    }
                }
            }

            // System Bus / Commands
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: "#0f172a"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label { text: "Inter-Process Communication Bus"; color: "#2dd4bf"; font.bold: true; font.pixelSize: 18 }

                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Last Broadcast:"; color: "#64748b" }
                        Label {
                            text: OmniUniversalContext.lastBroadcast
                            color: "#f8fafc"
                            font.bold: true
                            font.family: "monospace"
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        TextField {
                            id: bcastEvent
                            placeholderText: "Event Name (e.g., KERNEL_PANIC)"
                            Layout.preferredWidth: 200
                            color: "#e2e8f0"
                        }
                        TextField {
                            id: bcastData
                            placeholderText: "Payload Data"
                            Layout.fillWidth: true
                            color: "#e2e8f0"
                        }
                        Button {
                            text: "Broadcast"
                            onClicked: {
                                if (bcastEvent.text.length > 0) {
                                    OmniUniversalContext.broadcast(bcastEvent.text, bcastData.text)
                                    bcastEvent.text = ""; bcastData.text = ""
                                }
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Send a global Qt signal to all listening Omni modules"
                        }
                    }

                    Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                    Button {
                        text: "Emergency Metaphysical Reboot"
                        Layout.alignment: Qt.AlignRight
                        onClicked: OmniUniversalContext.systemReboot()
                        ToolTip.visible: hovered; ToolTip.text: "Wipe volatile context memory and reinitialize the OmniOmega core"
                    }
                }
            }
        }
    }
}
