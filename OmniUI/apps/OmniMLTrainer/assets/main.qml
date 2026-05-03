import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.ML 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniML Model Training Dashboard v41.0.0"
    color: "#0f172a" // Slate 900

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Deep Learning Tensor Studio"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Neural Network Training & Loss Optimization Tracker"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: OmniNeuralNetwork.training ? "Status: ACTIVELY TRAINING" : "Status: IDLE"
                color: OmniNeuralNetwork.training ? "#34d399" : "#fbbf24"
                font.bold: true
                font.pixelSize: 16
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Controls & Stats
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

                Label { text: "Model Telemetry"; color: "#818cf8"; font.bold: true; font.pixelSize: 18 }

                // Epoch
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Current Epoch"; color: "#94a3b8" }
                    Label {
                        text: OmniNeuralNetwork.currentEpoch
                        color: "#f8fafc"
                        font.pixelSize: 36
                        font.bold: true
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Number of full passes through the training dataset"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // Loss
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Loss Function (Error Rate)"; color: "#94a3b8" }
                    Label {
                        text: OmniNeuralNetwork.currentLoss.toFixed(4)
                        color: OmniNeuralNetwork.currentLoss > 1.0 ? "#f43f5e" : "#34d399"
                        font.pixelSize: 32
                        font.bold: true
                        font.family: "monospace"
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Current error calculation. Goal is to minimize this to 0.0"
                }

                // Accuracy
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Validation Accuracy"; color: "#94a3b8" }
                    RowLayout {
                        Layout.fillWidth: true
                        ProgressBar {
                            Layout.fillWidth: true
                            value: OmniNeuralNetwork.currentAccuracy / 100.0
                            background: Rectangle { color: "#334155"; radius: 2 }
                        }
                        Label {
                            text: OmniNeuralNetwork.currentAccuracy.toFixed(1) + "%"
                            color: OmniNeuralNetwork.currentAccuracy > 90.0 ? "#60a5fa" : "#fb923c"
                            font.bold: true
                            Layout.preferredWidth: 60
                        }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Percentage of correct inferences on validation set"
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: OmniNeuralNetwork.training ? "Stop Epochs" : "Start Training"
                        Layout.fillWidth: true
                        onClicked: OmniNeuralNetwork.training ? OmniNeuralNetwork.stopTraining() : OmniNeuralNetwork.startTraining()
                        ToolTip.visible: hovered; ToolTip.text: "Toggle the gradient descent simulation loop"
                    }
                    Button {
                        text: "Reset Model Weights"
                        Layout.fillWidth: true
                        onClicked: { OmniNeuralNetwork.stopTraining(); OmniNeuralNetwork.resetModel(); }
                        ToolTip.visible: hovered; ToolTip.text: "Re-initialize neural net to random weights (high loss)"
                    }
                }
            }
        }

        // RIGHT PANEL: Graph Canvas
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#0f172a"
            radius: 8
            border.color: "#334155"
            clip: true

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                RowLayout {
                    Layout.fillWidth: true
                    Label {
                        text: "Loss vs Accuracy Convergence Graph"
                        color: "#a78bfa"
                        font.bold: true
                        font.pixelSize: 18
                    }
                    Item { Layout.fillWidth: true }
                    Label { text: "■ Loss"; color: "#f43f5e"; font.bold: true }
                    Label { text: "■ Accuracy"; color: "#60a5fa"; font.bold: true }
                }

                Canvas {
                    id: mlGraph
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Connections {
                        target: OmniNeuralNetwork
                        function onLogChanged() { mlGraph.requestPaint(); }
                    }

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        // Draw Grid
                        ctx.strokeStyle = "#1e293b";
                        ctx.lineWidth = 1;
                        ctx.beginPath();
                        for (var gy = 0; gy < height; gy += 40) { ctx.moveTo(0, gy); ctx.lineTo(width, gy); }
                        for (var gx = 0; gx < width; gx += 40) { ctx.moveTo(gx, 0); ctx.lineTo(gx, height); }
                        ctx.stroke();

                        var history = OmniNeuralNetwork.lossHistory;
                        if (!history || history.length === 0) return;

                        var step = width / 100; // max 100 points

                        // DRAW LOSS (0 to 3.0 scale roughly)
                        ctx.beginPath();
                        ctx.strokeStyle = "#f43f5e"; // Rose
                        ctx.lineWidth = 2;
                        for (var i = 0; i < history.length; i++) {
                            var lx = i * step;
                            // Invert Y axis, max loss scale ~3.0
                            var ly = height - (Math.min(history[i].loss, 3.0) / 3.0 * height);

                            if (i === 0) ctx.moveTo(lx, ly);
                            else ctx.lineTo(lx, ly);
                        }
                        ctx.stroke();

                        // DRAW ACCURACY (0 to 100 scale)
                        ctx.beginPath();
                        ctx.strokeStyle = "#60a5fa"; // Blue
                        ctx.lineWidth = 2;
                        for (var j = 0; j < history.length; j++) {
                            var ax = j * step;
                            // Invert Y axis, scale 0-100
                            var ay = height - (history[j].acc / 100.0 * height);

                            if (j === 0) ctx.moveTo(ax, ay);
                            else ctx.lineTo(ax, ay);
                        }
                        ctx.stroke();
                    }
                }
            }
        }
    }
}
