import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Multiverse 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniMultiverse Array Monitor v41.0.0"
    color: "#18181b" // Zinc 900

    property string alertMessage: ""

    Connections {
        target: OmniUniverseManager
        function onIncursionDetected(msg) {
            alertMessage = msg;
            alertTimer.start();
        }
        function onUniverseAnnihilated(name) {
            alertMessage = "REALITY ANNIHILATED: " + name;
            alertTimer.start();
        }
    }

    Timer {
        id: alertTimer
        interval: 3000
        onTriggered: alertMessage = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#27272a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Multiversal Observatory"
                color: "#f4f4f5"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Manage and monitor parallel realities"
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

        // LEFT PANEL: Multiverse Meta-Stats & Creation
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#27272a"
            radius: 8
            border.color: "#3f3f46"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Meta-Reality Metrics"; color: "#a78bfa"; font.bold: true; font.pixelSize: 18 }

                // Total Realities
                ColumnLayout {
                    spacing: 5
                    Label { text: "Simulated Universes"; color: "#a1a1aa" }
                    Label {
                        text: OmniUniverseManager.universeCount
                        color: "#60a5fa"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Total number of active, isolated physics bubbles"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Incursions
                ColumnLayout {
                    spacing: 5
                    Label { text: "Membrane Collisions (Incursions)"; color: "#a1a1aa" }
                    Label {
                        text: OmniUniverseManager.multiCollisions
                        color: OmniUniverseManager.multiCollisions > 0 ? "#fbbf24" : "#a1a1aa"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Instances where two universes leaked physics/matter into each other"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Flux
                ColumnLayout {
                    spacing: 5
                    Label { text: "Cross-Reality Flux Coefficient"; color: "#a1a1aa" }
                    Label {
                        text: OmniUniverseManager.crossRealityFlux.toFixed(2) + " Ω"
                        color: "#f472b6"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Background radiation from multiverse spawning and destruction"
                }

                Item { Layout.fillHeight: true }

                Label { text: "Spawn Alternate Reality"; color: "#a1a1aa" }

                TextField {
                    id: uniName
                    Layout.fillWidth: true
                    placeholderText: "Universe Designation (e.g., Earth-838)"
                    color: "#f4f4f5"
                }

                ComboBox {
                    id: physicsModel
                    Layout.fillWidth: true
                    model: ["Standard Model", "String Theory (11D)", "Magical Realism", "Anti-Matter Dominant"]
                    ToolTip.visible: hovered; ToolTip.text: "Select foundational physics laws for new reality"
                }

                Button {
                    text: "Ignite Bubble Universe"
                    Layout.fillWidth: true
                    onClicked: {
                        if (uniName.text.length > 0) {
                            OmniUniverseManager.createUniverse(uniName.text, Math.random() * 50 + 50, physicsModel.currentText);
                            uniName.text = "";
                        }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Create an entirely new, isolated parallel universe"
                }
            }
        }

        // RIGHT PANEL: Reality Roster
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#27272a"
            radius: 8
            border.color: "#3f3f46"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label {
                    text: "Active Universes Matrix"
                    color: "#2dd4bf"
                    font.bold: true
                    font.pixelSize: 18
                }

                ListView {
                    id: uniList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    model: OmniUniverseManager.universes

                    delegate: Rectangle {
                        width: uniList.width
                        height: 80
                        color: modelData.isActive ? "#1e1e24" : "#27272a" // Highlight current reality
                        border.color: modelData.isActive ? "#60a5fa" : "#3f3f46"
                        border.width: modelData.isActive ? 2 : 1
                        radius: 8

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 15

                            ColumnLayout {
                                Layout.preferredWidth: 250
                                spacing: 2
                                Label {
                                    text: modelData.name + " [ID: " + modelData.id + "]"
                                    color: modelData.isActive ? "#60a5fa" : "#e5e5e5"
                                    font.bold: true
                                }
                                Label { text: "Physics: " + modelData.physics; color: "#a1a1aa"; font.pixelSize: 12 }
                                Label { text: "Energy Base: " + modelData.energy.toFixed(1) + " TJ"; color: "#fb923c"; font.pixelSize: 12 }
                            }

                            Rectangle { width: 1; Layout.fillHeight: true; color: "#3f3f46" }

                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2
                                Label { text: "Membrane Stability:"; color: "#a1a1aa"; font.pixelSize: 12 }
                                ProgressBar {
                                    Layout.fillWidth: true
                                    value: modelData.stability / 100
                                    // Turn red if unstable
                                    background: Rectangle { color: "#3f3f46"; radius: 2 }
                                }
                                Label { text: modelData.stability.toFixed(1) + "%"; color: modelData.stability < 30 ? "#f43f5e" : "#2dd4bf"; font.pixelSize: 12 }
                            }

                            Rectangle { width: 1; Layout.fillHeight: true; color: "#3f3f46" }

                            ColumnLayout {
                                spacing: 5
                                Button {
                                    text: modelData.isActive ? "Current Reality" : "Teleport Here"
                                    enabled: !modelData.isActive
                                    onClicked: OmniUniverseManager.switchUniverse(modelData.id)
                                    ToolTip.visible: hovered; ToolTip.text: "Shift your observer perspective to this universe"
                                }
                                Button {
                                    text: "Trigger Incursion"
                                    enabled: OmniUniverseManager.universeCount > 1
                                    onClicked: OmniUniverseManager.causeIncursion(OmniUniverseManager.currentUniverseId, modelData.id)
                                    ToolTip.visible: hovered; ToolTip.text: "Force the current universe to collide with this one, damaging stability"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
