import QtQuick 2.15
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

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
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
                }
            }
        }

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
            }
        }
    }
}

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
