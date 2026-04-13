import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.BCI 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniMind BCI Interface v41.0.0"
    color: "#18181b" // Zinc 900

    property string detectedIntention: "Awaiting Neural Command..."

    Connections {
        target: OmniBCIProcessor
        function onIntentionDetected(intent) {
            detectedIntention = intent;
            intentionClearTimer.start();
        }
    }

    Timer {
        id: intentionClearTimer
        interval: 2000
        onTriggered: detectedIntention = "Awaiting Neural Command..."
    }

    header: ToolBar {
        background: Rectangle { color: "#27272a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Brain-Computer Interface"
                color: "#e4e4e7"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Direct Neural Link Monitoring & Translation"
            }

            Item { Layout.fillWidth: true }

            Switch {
                text: "Headset Connection"
                checked: OmniBCIProcessor.isActive
                onCheckedChanged: checked ? OmniBCIProcessor.startProcessing() : OmniBCIProcessor.stopProcessing()
                ToolTip.visible: hovered
                ToolTip.text: "Toggle synthetic neural link connection"
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Mental States & Controls
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

                Label { text: "Cognitive State Analysis"; color: "#f472b6"; font.bold: true; font.pixelSize: 18 }

                // Mental State Label
                ColumnLayout {
                    spacing: 5
                    Label { text: "Current Meta-State:"; color: "#a1a1aa" }
                    Label {
                        text: OmniBrainState.mentalState
                        color: {
                            if (OmniBrainState.mentalState === "Flow State / High Focus") return "#60a5fa"
                            if (OmniBrainState.mentalState === "Overwhelmed / Anxious") return "#f43f5e"
                            if (OmniBrainState.mentalState === "Deep Relaxation / Sleep") return "#a78bfa"
                            return "#34d399"
                        }
                        font.pixelSize: 24
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Categorized mental state based on band ratios"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Focus Level
                ColumnLayout {
                    spacing: 5
                    Label { text: "Focus / Attention"; color: "#a1a1aa" }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniBrainState.focus
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Ratio of Beta/Gamma to lower frequency bands"
                }

                // Relaxation Level
                ColumnLayout {
                    spacing: 5
                    Label { text: "Relaxation / Meditation"; color: "#a1a1aa" }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniBrainState.relaxation
                        background: Rectangle { color: "#3f3f46"; radius: 2 }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Alpha wave dominance"
                }

                // Stress Level
                ColumnLayout {
                    spacing: 5
                    Label { text: "Cognitive Load / Stress"; color: "#a1a1aa" }
                    ProgressBar {
                        Layout.fillWidth: true
                        value: OmniBrainState.stress
                    }
                    ToolTip.visible: hovered; ToolTip.text: "High-frequency Gamma overloading"
                }

                Item { Layout.fillHeight: true }

                Label { text: "Simulate Neural Activity"; color: "#a1a1aa" }

                Button {
                    text: "Stimulate: High Focus (Math/Logic)"
                    Layout.fillWidth: true
                    onClicked: OmniBCIProcessor.stimulateFocus()
                    enabled: OmniBCIProcessor.isActive
                    ToolTip.visible: hovered; ToolTip.text: "Force Beta and Gamma waves to spike"
                }
                Button {
                    text: "Stimulate: Meditation (Alpha)"
                    Layout.fillWidth: true
                    onClicked: OmniBCIProcessor.stimulateRelaxation()
                    enabled: OmniBCIProcessor.isActive
                    ToolTip.visible: hovered; ToolTip.text: "Force Alpha waves to become dominant"
                }
                Button {
                    text: "Stimulate: Deep Sleep (Delta)"
                    Layout.fillWidth: true
                    onClicked: OmniBCIProcessor.stimulateDeepSleep()
                    enabled: OmniBCIProcessor.isActive
                    ToolTip.visible: hovered; ToolTip.text: "Force Delta and Theta waves to dominate"
                }
            }
        }

        // RIGHT PANEL: EEG Output and Intentions
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // Intention Translator
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                color: "#18181b"
                radius: 8
                border.color: "#3f3f46"
                border.width: 2

                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: 10
                    Label { text: "Thought-to-Text Translator"; color: "#a1a1aa"; Layout.alignment: BobUI.AlignHCenter }
                    Label {
                        text: detectedIntention
                        color: detectedIntention === "Awaiting Neural Command..." ? "#52525b" : "#fbbf24"
                        font.pixelSize: 28
                        font.bold: true
                        Layout.alignment: BobUI.AlignHCenter
                    }
                }
            }

            // Raw EEG Graph & Spectral Power
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#27272a"
                radius: 8
                border.color: "#3f3f46"
                clip: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label {
                        text: "Live EEG Telemetry & Spectral Power"
                        color: "#38bdf8"
                        font.bold: true
                        font.pixelSize: 18
                    }

                    // Spectral Power Bars
                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 150
                        spacing: 15

                        // Delta
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            ProgressBar {
                                value: OmniBCIProcessor.deltaPower / 50.0; Layout.fillWidth: true; Layout.fillHeight: true; orientation: BobUI.Vertical
                                ToolTip.visible: hovered; ToolTip.text: "0.5 - 4 Hz (Deep Sleep / Healing)"
                            }
                            Label { text: "Delta"; color: "#a1a1aa"; Layout.alignment: BobUI.AlignHCenter; font.bold: true }
                        }
                        // Theta
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            ProgressBar {
                                value: OmniBCIProcessor.thetaPower / 50.0; Layout.fillWidth: true; Layout.fillHeight: true; orientation: BobUI.Vertical
                                ToolTip.visible: hovered; ToolTip.text: "4 - 8 Hz (Creativity / Dreams / Memory)"
                            }
                            Label { text: "Theta"; color: "#a1a1aa"; Layout.alignment: BobUI.AlignHCenter; font.bold: true }
                        }
                        // Alpha
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            ProgressBar {
                                value: OmniBCIProcessor.alphaPower / 50.0; Layout.fillWidth: true; Layout.fillHeight: true; orientation: BobUI.Vertical
                                ToolTip.visible: hovered; ToolTip.text: "8 - 12 Hz (Relaxed / Calm / Awake)"
                            }
                            Label { text: "Alpha"; color: "#a1a1aa"; Layout.alignment: BobUI.AlignHCenter; font.bold: true }
                        }
                        // Beta
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            ProgressBar {
                                value: OmniBCIProcessor.betaPower / 50.0; Layout.fillWidth: true; Layout.fillHeight: true; orientation: BobUI.Vertical
                                ToolTip.visible: hovered; ToolTip.text: "12 - 30 Hz (Alert / Focused / Thinking)"
                            }
                            Label { text: "Beta"; color: "#a1a1aa"; Layout.alignment: BobUI.AlignHCenter; font.bold: true }
                        }
                        // Gamma
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5
                            ProgressBar {
                                value: OmniBCIProcessor.gammaPower / 50.0; Layout.fillWidth: true; Layout.fillHeight: true; orientation: BobUI.Vertical
                                ToolTip.visible: hovered; ToolTip.text: "> 30 Hz (Peak Focus / High Cognitive Load)"
                            }
                            Label { text: "Gamma"; color: "#a1a1aa"; Layout.alignment: BobUI.AlignHCenter; font.bold: true }
                        }
                    }

                    Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                    // Raw Time-domain Canvas
                    Canvas {
                        id: eegCanvas
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Connections {
                            target: OmniBCIProcessor
                            function onEegChanged() { eegCanvas.requestPaint(); }
                        }

                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.clearRect(0, 0, width, height);

                            var eeg = OmniBCIProcessor.rawEEGStream;
                            if (!eeg || eeg.length === 0) return;

                            // Draw grid
                            ctx.strokeStyle = "#3f3f46";
                            ctx.lineWidth = 1;
                            ctx.beginPath();
                            for (var gy = 0; gy < height; gy += 40) { ctx.moveTo(0, gy); ctx.lineTo(width, gy); }
                            for (var gx = 0; gx < width; gx += 40) { ctx.moveTo(gx, 0); ctx.lineTo(gx, height); }
                            ctx.stroke();

                            ctx.beginPath();
                            ctx.strokeStyle = "#34d399";
                            ctx.lineWidth = 2;

                            var step = width / eeg.length;
                            var midY = height / 2;

                            for (var i = 0; i < eeg.length; i++) {
                                var x = i * step;
                                var y = midY - (eeg[i] * 2.0); // scale amp

                                if (i === 0) ctx.moveTo(x, y);
                                else ctx.lineTo(x, y);
                            }
                            ctx.stroke();
                        }
                    }
                }
            }
        }
    }
}
