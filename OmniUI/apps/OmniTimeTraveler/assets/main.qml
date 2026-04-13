<<<<<<< HEAD
import QtQuick 2.15
<<<<<<< HEAD
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
=======
import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
>>>>>>> origin/dev
import Omni.Time 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniTime Chronosphere Console v41.0.0"
    color: "#27272a" // Zinc 800

    header: ToolBar {
        background: Rectangle { color: "#18181b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Temporal Navigation"
                color: "#e4e4e7"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Navigate 4D spacetime and manage timeline branches"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Status: " + OmniTimeMachine.status
                color: "#fca5a5"
                font.bold: true
                font.pixelSize: 16
                ToolTip.visible: hovered
                ToolTip.text: "Current state of the chronosphere drive"
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // TOP PANEL: Master Clock & Controls
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 200
            color: "#18181b"
            radius: 8
            border.color: "#3f3f46"
=======
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniTime 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 720
    title: "OmniTime Traveler"
    color: "#1a1a2e"

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Control Panel
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#16213e"
>>>>>>> feature/omni-ui-framework-18001284211800334382

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
<<<<<<< HEAD
                spacing: 15

                RowLayout {
                    Layout.fillWidth: true

                    ColumnLayout {
                        spacing: 5
                        Label { text: "Current Timeline Branch"; color: "#a1a1aa"; font.bold: true }
                        Label {
                            text: OmniTimeline.currentBranch
                            color: "#c084fc"
                            font.pixelSize: 24
                            font.bold: true
                            ToolTip.visible: hovered
                            ToolTip.text: "The parallel timeline you are currently observing"
                        }
                    }

                    Item { Layout.fillWidth: true }

                    ColumnLayout {
                        spacing: 5
                        Label { text: "Temporal Coordinates (UTC)"; color: "#a1a1aa"; font.bold: true; Layout.alignment: BobUI.AlignRight }
                        Label {
                            text: OmniTimeline.formattedTime
                            color: "#38bdf8"
                            font.pixelSize: 32
                            font.bold: true
                            font.family: "monospace"
                            Layout.alignment: BobUI.AlignRight
                            ToolTip.visible: hovered
                            ToolTip.text: "Precise timestamp of the current simulation epoch"
                        }
                        Label {
                            text: "Unix Epoch: " + OmniTimeline.currentTimeMs + " ms"
                            color: "#52525b"
                            font.pixelSize: 12
                            font.family: "monospace"
                            Layout.alignment: BobUI.AlignRight
                        }
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Playback Controls
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 20

                    Button {
                        text: "⏪ Rewind (x10)"
                        onClicked: OmniTimeMachine.speed = -10.0
                        ToolTip.visible: hovered; ToolTip.text: "Reverse time flow"
                    }
                    Button {
                        text: "⏸ Pause"
                        onClicked: OmniTimeMachine.pause()
                        ToolTip.visible: hovered; ToolTip.text: "Halt temporal progression"
                    }
                    Button {
                        text: "▶ Play (Realtime)"
                        onClicked: OmniTimeMachine.play()
                        ToolTip.visible: hovered; ToolTip.text: "Resume 1x timeline flow"
                    }
                    Button {
                        text: "⏩ Fast Forward (x10)"
                        onClicked: OmniTimeMachine.speed = 10.0
                        ToolTip.visible: hovered; ToolTip.text: "Accelerate time flow"
                    }
                    Button {
                        text: "⏭ Super Speed (x1000)"
                        onClicked: OmniTimeMachine.speed = 1000.0
                        ToolTip.visible: hovered; ToolTip.text: "Massively accelerate time flow"
                    }
=======
                spacing: 20

                Text { text: "TEMPORAL CONTROLS"; color: "white"; font.bold: true; font.pixelSize: 20 }

                Text { text: "Current Epoch: " + new Date(Timeline.currentTime * 1000).toLocaleString(); color: "cyan" }

                Text { text: "Simulation Speed: " + TimeMachine.speed.toFixed(1) + "x"; color: "white" }
                OmniSlider {
                    from: -10; to: 10
                    value: TimeMachine.speed
                    onValueChanged: TimeMachine.speed = value
                    Layout.fillWidth: true
                }

                Button {
                    text: TimeMachine.active ? "PAUSE" : "RESUME"
                    Layout.fillWidth: true
                    onClicked: TimeMachine.active ? TimeMachine.pause() : TimeMachine.play()
                }

                Text { text: "Paradox Risk: " + (ParadoxDetector.probability * 100).toFixed(1) + "%"; color: "orange" }
                Text { text: "Status: " + ParadoxDetector.alert; color: ParadoxDetector.alert === "Stable" ? "green" : "red" }

                Button {
                    text: "Scan Causality"
                    Layout.fillWidth: true
                    onClicked: ParadoxDetector.analyzeCausality()
                }

                Item { Layout.fillHeight: true }

                Button {
                    text: "CREATE BRANCH"
                    Layout.fillWidth: true
                    onClicked: Timeline.createBranch("Beta", Timeline.currentTime)
>>>>>>> feature/omni-ui-framework-18001284211800334382
                }
            }
        }

<<<<<<< HEAD
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // LEFT PANEL: Branch Management
            Rectangle {
                Layout.preferredWidth: 300
                Layout.fillHeight: true
                color: "#18181b"
                radius: 8
                border.color: "#3f3f46"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15

                    Label { text: "Multiverse Branches"; color: "#fb7185"; font.bold: true; font.pixelSize: 18 }

                    ListView {
                        id: branchList
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: OmniTimeline.branches

                        delegate: Rectangle {
                            width: branchList.width
                            height: 40
                            color: modelData === OmniTimeline.currentBranch ? "#3f3f46" : "transparent"
                            radius: 4

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                Label {
                                    text: modelData
                                    color: modelData === OmniTimeline.currentBranch ? "#e4e4e7" : "#a1a1aa"
                                    font.bold: modelData === OmniTimeline.currentBranch
                                    Layout.fillWidth: true
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: OmniTimeline.currentBranch = modelData
                                hoverEnabled: true
                                ToolTip.visible: containsMouse
                                ToolTip.text: "Switch perspective to this timeline"
                            }
                        }
                    }

                    Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                    Label { text: "Create Temporal Divergence"; color: "#a1a1aa"; font.bold: true }

                    TextField {
                        id: branchInput
                        Layout.fillWidth: true
                        placeholderText: "New Branch Name (e.g. Gamma)"
                        color: "#e4e4e7"
                    }

                    Button {
                        text: "Branch & Jump -1 Year"
                        Layout.fillWidth: true
                        enabled: branchInput.text.length > 0
                        onClicked: {
                            // Calculate timestamp for 1 year ago (approx)
                            var ms = OmniTimeline.currentTimeMs - (365 * 24 * 60 * 60 * 1000);
                            OmniTimeMachine.branchAndJumpTo(branchInput.text, ms);
                            branchInput.text = "";
                        }
                        ToolTip.visible: hovered
                        ToolTip.text: "Travel to the past, creating a new timeline to prevent paradoxes"
                    }
                }
            }

            // RIGHT PANEL: Event History & Paradox Detector
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 20

                // Event History (Timeline)
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
                            text: "Causal Event History (" + OmniTimeline.currentBranch + ")"
                            color: "#34d399"
                            font.bold: true
                            font.pixelSize: 18
                        }

                        ListView {
                            id: eventList
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            model: OmniTimeline.events

                            delegate: Rectangle {
                                width: eventList.width
                                height: 60
                                color: index % 2 === 0 ? "#27272a" : "#18181b"
                                border.color: "#3f3f46"
                                border.width: 1

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 10
                                    spacing: 15

                                    Label {
                                        text: modelData.time
                                        color: "#93c5fd"
                                        font.family: "monospace"
                                        Layout.preferredWidth: 160
                                    }

                                    Rectangle { width: 2; Layout.fillHeight: true; color: "#3f3f46" }

                                    Label {
                                        text: modelData.desc
                                        color: "#e4e4e7"
                                        font.bold: true
                                        Layout.fillWidth: true
                                    }
                                }
                            }
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            TextField {
                                id: eventInput
                                Layout.fillWidth: true
                                placeholderText: "Record temporal event..."
                                color: "#e4e4e7"
                                onAccepted: eventBtn.clicked()
                            }
                            Button {
                                id: eventBtn
                                text: "Log Event"
                                onClicked: {
                                    if (eventInput.text.length > 0) {
                                        OmniTimeline.addEvent(eventInput.text);
                                        eventInput.text = "";
                                    }
                                }
                                ToolTip.visible: hovered; ToolTip.text: "Burn a causal event into the current timeline"
                            }
                        }
                    }
                }

                // Embedded Paradox Detector Panel
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 150
                    color: "#18181b"
                    radius: 8
                    border.color: OmniParadoxDetector.probability > 50 ? "#f43f5e" : "#3f3f46"
                    border.width: OmniParadoxDetector.probability > 50 ? 3 : 1

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 15

                        RowLayout {
                            Layout.fillWidth: true
                            Label { text: "Causality Paradox Monitor"; color: "#fca5a5"; font.bold: true }
                            Item { Layout.fillWidth: true }
                            Label {
                                text: OmniParadoxDetector.alert
                                color: OmniParadoxDetector.probability > 50 ? "#f43f5e" : "#34d399"
                                font.bold: true
                            }
                        }

                        ProgressBar {
                            Layout.fillWidth: true
                            value: OmniParadoxDetector.probability / 100.0
                            background: Rectangle { color: "#3f3f46"; radius: 2 }
                            ToolTip.visible: hovered; ToolTip.text: "Paradox Probability. If 100%, reality collapses."
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10
                            Button {
                                text: "Analyze Causal Link"
                                onClicked: OmniParadoxDetector.analyzeCausality()
                                ToolTip.visible: hovered; ToolTip.text: "Check current timeline actions for grandfather paradoxes"
                            }
                            Button {
                                text: "Purge Timeline Branch (TVA Prune)"
                                enabled: OmniParadoxDetector.probability > 0
                                onClicked: OmniParadoxDetector.purgeTimeline()
                                ToolTip.visible: hovered; ToolTip.text: "Erase the divergent timeline to restore causal stability"
                            }
                        }
                    }
                }
=======
        // Timeline View
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f3460"

            // Placeholder for branching visualization
            Canvas {
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.strokeStyle = "white";
                    ctx.lineWidth = 2;
                    ctx.beginPath();
                    // Main line
                    ctx.moveTo(0, height/2);
                    ctx.lineTo(width, height/2);
                    ctx.stroke();

                    // Branch
                    ctx.beginPath();
                    ctx.strokeStyle = "cyan";
                    ctx.moveTo(width/3, height/2);
                    ctx.quadraticCurveTo(width/2, height/2 - 50, width, height/2 - 100);
                    ctx.stroke();
                }
            }

            Text {
                text: "Current Branch: " + Timeline.currentBranch
                font.pixelSize: 24
                color: "white"
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 20
>>>>>>> feature/omni-ui-framework-18001284211800334382
            }
        }
    }
}
<<<<<<< HEAD
=======

    // Embedded Paradox Detector Panel (added to TimeTraveler)
    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 150
        color: "#18181b"
        radius: 8
        border.color: OmniParadoxDetector.probability > 50 ? "#f43f5e" : "#3f3f46"
        border.width: OmniParadoxDetector.probability > 50 ? 3 : 1

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 15

            RowLayout {
                Layout.fillWidth: true
                Label { text: "Causality Paradox Monitor"; color: "#fca5a5"; font.bold: true }
                Item { Layout.fillWidth: true }
                Label {
                    text: OmniParadoxDetector.alert
                    color: OmniParadoxDetector.probability > 50 ? "#f43f5e" : "#34d399"
                    font.bold: true
                }
            }

            ProgressBar {
                Layout.fillWidth: true
                value: OmniParadoxDetector.probability / 100.0
                background: Rectangle { color: "#3f3f46"; radius: 2 }
                ToolTip.visible: hovered; ToolTip.text: "Paradox Probability. If 100%, reality collapses."
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                Button {
                    text: "Analyze Causal Link"
                    onClicked: OmniParadoxDetector.analyzeCausality()
                    ToolTip.visible: hovered; ToolTip.text: "Check current timeline actions for grandfather paradoxes"
                }
                Button {
                    text: "Purge Timeline Branch (TVA Prune)"
                    enabled: OmniParadoxDetector.probability > 0
                    onClicked: OmniParadoxDetector.purgeTimeline()
                    ToolTip.visible: hovered; ToolTip.text: "Erase the divergent timeline to restore causal stability"
                }
            }
        }
    }
>>>>>>> feature/omni-ui-framework-18001284211800334382
