import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniQuantum 1.0
import Omni3D 1.0

ApplicationWindow {
    visible: true
    width: 1280
    height: 800
    title: "OmniQuantum IDE"

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Gate Toolbox
        Rectangle {
            Layout.preferredWidth: 200
            Layout.fillHeight: true
            color: "#333"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Text { text: "Quantum Gates"; color: "white"; font.bold: true }

                Repeater {
                    model: ["H", "X", "Y", "Z", "CNOT", "Toffoli"]
                    delegate: Button {
                        text: modelData
                        Layout.fillWidth: true
                        onClicked: circuit.addGate(createGate(modelData))
                    }
                }

                Item { Layout.fillHeight: true }

                Button {
                    text: "Run Simulation"
                    Layout.fillWidth: true
                    highlighted: true
                    onClicked: Simulator.runCircuit(circuit)
                }
            }
        }

        // Circuit Composer
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#fff"

            ColumnLayout {
                anchors.centerIn: parent
                Text { text: "Quantum Circuit Composer (Stub)"; font.pixelSize: 24; color: "#ccc" }
                // Implementation would involve a grid of wires and droppable gates
            }
        }

        // Bloch Sphere Visualization
        Rectangle {
            Layout.preferredWidth: 400
            Layout.fillHeight: true
            color: "#000"

            ColumnLayout {
                anchors.fill: parent

                Text {
                    text: "Bloch Sphere Visualization"
                    color: "white"
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                    Layout.margins: 10
                }

                View3D {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    backgroundColor: "#111"

                    // Sphere mesh would go here
                    Mesh {
                        source: "sphere.obj"
                        scale: Qt.vector3d(2, 2, 2)
                    }
                }

                // Result Probabilities
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 150
                    color: "#222"

                    RowLayout {
                        anchors.centerIn: parent
                        spacing: 20

                        Column {
                            Text { text: "|0>"; color: "white" }
                            Rectangle { width: 30; height: 100 * (Simulator.getQubitState(0) ? Simulator.getQubitState(0).probabilityZero : 1); color: "cyan" }
                        }

                        Column {
                            Text { text: "|1>"; color: "white" }
                            Rectangle { width: 30; height: 100 * (Simulator.getQubitState(0) ? Simulator.getQubitState(0).probabilityOne : 0); color: "magenta" }
                        }
                    }
                }
            }
        }
    }

    QuantumCircuit {
        id: circuit
        qubitCount: 3
    }

    function createGate(type) {
        // Factory helper
        // This logic would normally be handled by the composer UI
        var component = Qt.createComponent("Gate.qml"); // Hypothetical
        return null;
    }
}
