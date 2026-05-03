import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Void 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniVoid Abyssal Monitor v41.0.0"
    color: "#000000" // Pitch black

    // Spaghettification effect logic
    property bool isDead: false

    Connections {
        target: OmniAbyss
        function onSpaghettificationEvent() {
            isDead = true;
            console.warn("EVENT HORIZON CROSSED. OBSERVER CONSUMED.");
        }
        function onMadnessInduced() {
            // Screen shake effect
            madnessAnimation.start();
        }
    }

    SequentialAnimation {
        id: madnessAnimation
        NumberAnimation { target: mainWindow; property: "x"; to: mainWindow.x - 10; duration: 50 }
        NumberAnimation { target: mainWindow; property: "x"; to: mainWindow.x + 20; duration: 50 }
        NumberAnimation { target: mainWindow; property: "x"; to: mainWindow.x - 10; duration: 50 }
    }

    Item { id: mainWindow; anchors.fill: parent } // anchor for shake

    header: ToolBar {
        background: Rectangle { color: "#09090b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "The Abyss"
                color: "#71717a"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Anomalous singularity monitoring framework"
            }
        }
    }

    // Death Screen Overlay
    Rectangle {
        anchors.fill: parent
        color: "black"
        z: 100
        visible: isDead

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20
            Label {
                text: "YOU HAVE BEEN CONSUMED BY THE VOID"
                color: "#dc2626"
                font.pixelSize: 32
                font.bold: true
                font.letterSpacing: 5
            }
            Label {
                text: "Your mass has been added to the singularity."
                color: "#71717a"
                Layout.alignment: BobUI.AlignHCenter
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20
        visible: !isDead

        // LEFT PANEL: Void Parameters
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#09090b"
            radius: 8
            border.color: "#18181b"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Singularity Metrics"; color: "#a1a1aa"; font.bold: true; font.pixelSize: 18 }

                // Mass
                ColumnLayout {
                    spacing: 5
                    Label { text: "Event Horizon Mass"; color: "#71717a" }
                    Label {
                        text: OmniAbyss.mass.toFixed(4) + " M☉"
                        color: "#d4d4d8"
                        font.pixelSize: 32
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Total mass trapped inside the singularity"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#18181b" }

                // Hawking Radiation
                ColumnLayout {
                    spacing: 5
                    Label { text: "Hawking Radiation Evaporation"; color: "#71717a" }
                    Label {
                        text: OmniAbyss.hawkingRadiation.toFixed(6) + " rad/s"
                        color: "#93c5fd"
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Thermal radiation predicted to be spontaneously emitted by black holes"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#18181b" }

                // Distance
                ColumnLayout {
                    spacing: 5
                    Label { text: "Proximity to Event Horizon"; color: "#71717a" }
                    Label {
                        text: OmniAbyss.horizonProximity.toFixed(1) + " AU"
                        color: OmniAbyss.horizonProximity < 20 ? "#ef4444" : "#22c55e"
                        font.pixelSize: 28
                        font.bold: true
                    }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniAbyss.horizonProximity / 100
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Distance from the point of no return. Do not reach 0."
                }

                Item { Layout.fillHeight: true }

                Label { text: "Interact with the Void"; color: "#71717a" }

                RowLayout {
                    Layout.fillWidth: true
                    Button {
                        text: "Stare Into Abyss"
                        Layout.fillWidth: true
                        onPressed: OmniAbyss.stareInto()
                        onReleased: OmniAbyss.retreat()
                        ToolTip.visible: hovered; ToolTip.text: "Hold to observe. Warning: The abyss pulls you closer. Release to retreat."
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    TextField {
                        id: entityName
                        Layout.fillWidth: true
                        placeholderText: "Item to sacrifice..."
                        color: "#d4d4d8"
                    }
                    Button {
                        text: "Sacrifice"
                        onClicked: {
                            if (entityName.text.length > 0) {
                                OmniAbyss.feedMatter(entityName.text, Math.random() * 100 + 10);
                                entityName.text = "";
                            }
                        }
                        ToolTip.visible: hovered; ToolTip.text: "Feed matter into the black hole, increasing its mass and gravitational pull."
                    }
                }
            }
        }

        // RIGHT PANEL: The Void Visualizer
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#050505"
            radius: 8
            border.color: "#18181b"
            clip: true

            Label {
                text: "Visualizing the Unknowable"
                color: "#3f3f46"
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 10
            }

            // Simple Void visualization
            Rectangle {
                anchors.centerIn: parent
                // The larger the mass, the bigger the hole. The closer we are, the bigger it looks.
                width: {
                    var base = Math.min(parent.width, parent.height) * 0.2;
                    var massFactor = OmniAbyss.mass / 10000.0;
                    var proxFactor = (100.0 - OmniAbyss.horizonProximity) / 100.0 * 2.0;
                    return base * massFactor * (1.0 + proxFactor);
                }
                height: width
                radius: width / 2
                color: "black"
                border.color: OmniAbyss.horizonProximity < 20 ? "#7f1d1d" : "#1e1b4b"
                border.width: 4 + (Math.random() * 2) // flickering accretion disk illusion

                // Pulsing animation when close
                Behavior on width { NumberAnimation { duration: 100 } }
                Behavior on height { NumberAnimation { duration: 100 } }
                Behavior on radius { NumberAnimation { duration: 100 } }
            }

            // Consumed Entities Log overlay
            ColumnLayout {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 10

                Label { text: "Recently Consumed:"; color: "#52525b"; font.bold: true; Layout.alignment: BobUI.AlignRight }

                ListView {
                    Layout.preferredWidth: 200
                    Layout.preferredHeight: 150
                    Layout.alignment: BobUI.AlignRight
                    model: OmniAbyss.consumedEntities
                    interactive: false

                    delegate: Label {
                        text: "- " + modelData.entity + " (" + modelData.mass.toFixed(1) + " M☉)"
                        color: "#71717a"
                        font.pixelSize: 12
                    }
                }
            }
        }
    }
}
