import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Audio 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniSynth Advanced DSP Engine v41.0.0"
    color: "#18181b" // Zinc 900

    header: ToolBar {
        background: Rectangle { color: "#27272a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "OmniSynth Oscillator 1"
                color: "#e4e4e7"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Digital Signal Processing audio generator"
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Controls
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

                Label { text: "Synthesizer Parameters"; color: "#a78bfa"; font.bold: true; font.pixelSize: 18 }

                // Waveform Selector
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Wavetable Type"; color: "#a1a1aa" }
                    ComboBox {
                        Layout.fillWidth: true
                        model: ["Sine", "Square", "Triangle", "Saw", "Noise"]
                        onCurrentTextChanged: OmniWavetableSynth.waveType = currentText
                        ToolTip.visible: hovered; ToolTip.text: "Select the base mathematical shape of the sound wave"
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#3f3f46" }

                // Frequency Control
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Frequency (Pitch)"; color: "#a1a1aa" }
                    RowLayout {
                        Layout.fillWidth: true
                        Slider {
                            Layout.fillWidth: true
                            from: 20.0
                            to: 2000.0
                            value: OmniWavetableSynth.frequency
                            onValueChanged: OmniWavetableSynth.frequency = value
                            ToolTip.visible: hovered; ToolTip.text: "Adjust pitch in Hertz (cycles per second)"
                        }
                        Label {
                            text: Math.round(OmniWavetableSynth.frequency) + " Hz"
                            color: "#60a5fa"
                            font.bold: true
                            Layout.preferredWidth: 60
                        }
                    }
                }

                // Amplitude Control
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Amplitude (Volume)"; color: "#a1a1aa" }
                    RowLayout {
                        Layout.fillWidth: true
                        Slider {
                            Layout.fillWidth: true
                            from: 0.0
                            to: 1.0
                            value: OmniWavetableSynth.amplitude
                            onValueChanged: OmniWavetableSynth.amplitude = value
                            ToolTip.visible: hovered; ToolTip.text: "Adjust the loudness multiplier of the wave"
                        }
                        Label {
                            text: (OmniWavetableSynth.amplitude * 100).toFixed(0) + " %"
                            color: "#fbbf24"
                            font.bold: true
                            Layout.preferredWidth: 60
                        }
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // RIGHT PANEL: Oscilloscope
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#09090b" // Very dark zinc
            radius: 8
            border.color: "#3f3f46"
            clip: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label {
                    text: "Live Oscilloscope Analysis"
                    color: "#34d399"
                    font.bold: true
                    font.pixelSize: 18
                }

                // Waveform Canvas
                Canvas {
                    id: scopeCanvas
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Connections {
                        target: OmniWavetableSynth
                        function onBufferChanged() { scopeCanvas.requestPaint(); }
                    }

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        // Draw Grid
                        ctx.strokeStyle = "#27272a";
                        ctx.lineWidth = 1;
                        ctx.beginPath();
                        for (var gy = 0; gy < height; gy += 40) { ctx.moveTo(0, gy); ctx.lineTo(width, gy); }
                        for (var gx = 0; gx < width; gx += 40) { ctx.moveTo(gx, 0); ctx.lineTo(gx, height); }
                        ctx.stroke();

                        // Center zero-line
                        ctx.beginPath();
                        ctx.moveTo(0, height/2); ctx.lineTo(width, height/2);
                        ctx.strokeStyle = "#3f3f46";
                        ctx.stroke();

                        // Draw Audio Buffer
                        var buf = OmniWavetableSynth.waveformBuffer;
                        if (!buf || buf.length === 0) return;

                        ctx.beginPath();
                        ctx.strokeStyle = "#34d399";
                        ctx.lineWidth = 2;

                        // Glow effect approx
                        ctx.shadowColor = "#34d399";
                        ctx.shadowBlur = 10;

                        var step = width / buf.length;
                        var midY = height / 2;

                        for (var i = 0; i < buf.length; i++) {
                            var x = i * step;
                            // buf values are -1.0 to 1.0, scale to height
                            var y = midY - (buf[i] * (height / 2.2));

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
