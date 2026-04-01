import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Spirit 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniSpirit Astral Projection Deck v41.0.0"
    color: "#18181b" // Zinc 900

    property string alertMessage: ""
    property bool isBreached: OmniEtherealPlane.breached

    Connections {
        target: OmniEtherealPlane
        function onVeilTorn() {
            alertMessage = "CRITICAL: The dimensional veil has collapsed.";
            breachTimer.start();
        }
        function onEntityManifested(type) {
            alertMessage = "WARNING: Unbound entity detected - " + type;
            breachTimer.start();
        }
    }

    Timer {
        id: breachTimer
        interval: 4000
        onTriggered: alertMessage = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#27272a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Ethereal Plane Link"
                color: "#e4e4e7"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Metaphysical connection to the astral dimensions"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: alertMessage
                color: OmniEtherealPlane.breached ? "#f43f5e" : "#fbbf24"
                font.bold: true
                visible: alertMessage !== ""
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Plane Dynamics
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#27272a"
            radius: 8
            border.color: isBreached ? "#f43f5e" : "#3f3f46"
            border.width: isBreached ? 3 : 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Dimensional Metrics"; color: "#a78bfa"; font.bold: true; font.pixelSize: 18 }

                // Resonance (Light/Dark)
                ColumnLayout {
                    spacing: 5
                    Label { text: "Collective Resonance"; color: "#a1a1aa" }
                    Label {
                        text: OmniEtherealPlane.alignment
                        color: {
                            if (OmniEtherealPlane.collectiveResonance > 60) return "#fbbf24" // light
                            if (OmniEtherealPlane.collectiveResonance < 40) return "#f43f5e" // dark
                            return "#e4e4e7" // neutral
                        }
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniEtherealPlane.collectiveResonance / 100.0
                    }
                    ToolTip.visible: hovered; ToolTip.text: "0% is Abyssal Dark, 100% is Ascendant Light"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // The Veil
                ColumnLayout {
                    spacing: 5
                    Label { text: "Boundary Veil Thickness"; color: "#a1a1aa" }
                    Label {
                        text: OmniEtherealPlane.veilThickness.toFixed(1) + "%"
                        color: OmniEtherealPlane.veilThickness < 30 ? "#f43f5e" : "#60a5fa"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniEtherealPlane.veilThickness / 100.0
                        background: Rectangle { color: "#3f3f46"; radius: 2 }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "If the veil reaches 0%, realities merge and horrors spawn."
                }

                // Entities
                ColumnLayout {
                    spacing: 5
                    Label { text: "Active Manifestations"; color: "#a1a1aa" }
                    Label {
                        text: OmniEtherealPlane.activeEntities
                        color: OmniEtherealPlane.activeEntities > 0 ? "#f472b6" : "#64748b"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Spirits currently loose in the physical plane"
                }

                Item { Layout.fillHeight: true }

                Label { text: "Metaphysical Interaction"; color: "#a1a1aa" }

                Button {
                    text: "Channel Positive Energy (50 THz)"
                    Layout.fillWidth: true
                    onClicked: OmniEtherealPlane.channelEnergy(50.0)
                    ToolTip.visible: hovered; ToolTip.text: "Increases resonance towards Light, but drains the veil boundary."
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Dark Ritual"
                        Layout.fillWidth: true
                        onClicked: OmniEtherealPlane.performRitual("Summoning")
                        ToolTip.visible: hovered; ToolTip.text: "Drastically lowers resonance and severely damages the veil."
                    }
                    Button {
                        text: "Meditate"
                        Layout.fillWidth: true
                        onClicked: OmniEtherealPlane.performRitual("Meditation")
                        ToolTip.visible: hovered; ToolTip.text: "Slowly restores the veil and neutralizes resonance."
                    }
                }

                Button {
                    text: "PURGE PLANE"
                    Layout.fillWidth: true
                    enabled: OmniEtherealPlane.activeEntities > 0 || isBreached
                    onClicked: OmniEtherealPlane.cleansePlane()
                    ToolTip.visible: hovered; ToolTip.text: "Emergency banishment of all entities and restoration of the veil."
                }
            }
        }

        // RIGHT PANEL: Astral Monitor Log
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#18181b"
            radius: 8
            border.color: "#3f3f46"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label {
                    text: "Astral Manifestation Log"
                    color: "#34d399"
                    font.bold: true
                    font.pixelSize: 18
                }

                ListView {
                    id: astralLog
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniEtherealPlane.manifestationLog

                    delegate: Rectangle {
                        width: astralLog.width
                        height: 40
                        color: index % 2 === 0 ? "#27272a" : "#18181b"
                        border.color: "#3f3f46"
                        border.width: 1
                        radius: 4

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 15

                            Label {
                                text: "[" + modelData.time + "]"
                                color: "#64748b"
                                font.family: "monospace"
                                Layout.preferredWidth: 100
                            }

                            Rectangle { width: 1; Layout.fillHeight: true; color: "#3f3f46" }

                            Label {
                                text: modelData.msg
                                color: modelData.msg.indexOf("CRITICAL") !== -1 ? "#f43f5e" : "#e4e4e7"
                                font.bold: modelData.msg.indexOf("CRITICAL") !== -1
                                font.family: "monospace"
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }
    }
}
