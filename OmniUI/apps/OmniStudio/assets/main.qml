import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Nodes 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "OmniStudio Visual Logic Editor v41.0.0"
    color: "#0f172a" // Slate 900

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Node-Based Workflow Designer"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Connect functional blocks to build complex behaviors without code"
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Reset Canvas"
                onClicked: nodeCanvas.clearCanvas()
                ToolTip.visible: hovered
                ToolTip.text: "Delete all nodes and reset view"
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        // LEFT PANEL: Node Palette
        Rectangle {
            Layout.preferredWidth: 250
            Layout.fillHeight: true
            color: "#1e293b"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Label { text: "Node Palette"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                TextField {
                    id: nodeTitleInput
                    Layout.fillWidth: true
                    placeholderText: "Custom Node Name"
                    color: "#f8fafc"
                }

                Button {
                    text: "+ Add Custom Node"
                    Layout.fillWidth: true
                    onClicked: {
                        var title = nodeTitleInput.text || "Unnamed Node";
                        // Place roughly in center of view
                        nodeCanvas.addNode(title, 200, 200);
                        nodeTitleInput.text = "";
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Spawns a new logic block onto the infinite canvas"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                Label { text: "Quick Connect"; color: "#94a3b8"; font.bold: true }

                RowLayout {
                    Layout.fillWidth: true
                    TextField { id: sourceIdx; placeholderText: "Src Index"; Layout.fillWidth: true; validator: IntValidator{bottom: 0} }
                    TextField { id: targetIdx; placeholderText: "Tgt Index"; Layout.fillWidth: true; validator: IntValidator{bottom: 0} }
                }

                Button {
                    text: "Connect Nodes"
                    Layout.fillWidth: true
                    onClicked: {
                        if (sourceIdx.text && targetIdx.text) {
                            nodeCanvas.connectNodes(parseInt(sourceIdx.text), parseInt(targetIdx.text));
                        }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Draw a spline curve linking the output of Source to the input of Target"
                }

                Item { Layout.fillHeight: true }

                Label { text: "Canvas Controls"; color: "#94a3b8"; font.bold: true }
                Label { text: "- Left Click + Drag: Move Nodes"; color: "#64748b"; font.pixelSize: 12 }
                Label { text: "- Right Click + Drag: Pan Canvas"; color: "#64748b"; font.pixelSize: 12 }
                Label { text: "- Scroll Wheel: Zoom In/Out"; color: "#64748b"; font.pixelSize: 12 }
            }
        }

        // RIGHT PANEL: The Infinite Node Canvas
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1e1e24"
            radius: 8
            border.color: "#334155"
            border.width: 2
            clip: true

            OmniNodeCanvas {
                id: nodeCanvas
                anchors.fill: parent

                Component.onCompleted: {
                    // Seed some default nodes
                    addNode("Input Event", 100, 100);
                    addNode("Transform Logic", 350, 150);
                    addNode("Renderer", 600, 200);
                    connectNodes(0, 1);
                    connectNodes(1, 2);
                }
            }

            // Mini-map / Zoom readout overlay
            Rectangle {
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 15
                width: 120
                height: 40
                color: "#1e293b"
                radius: 4
                border.color: "#334155"
                opacity: 0.8

                RowLayout {
                    anchors.centerIn: parent
                    Label { text: "Zoom: " + (nodeCanvas.zoom * 100).toFixed(0) + "%"; color: "#cbd5e1"; font.bold: true }
                }
            }
        }
    }
}
