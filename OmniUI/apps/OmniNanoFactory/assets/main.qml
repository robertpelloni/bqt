import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Nano 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniNano Assembly Interface v41.0.0"
    color: "#0f172a" // Slate 900

    property string errorMsg: ""

    Connections {
        target: OmniLattice
        function onStructureFailed(msg) {
            errorMsg = msg;
            errorTimer.start();
        }
    }

    Connections {
        target: OmniAssembler
        function onErrorOccurred(msg) {
            errorMsg = msg;
            errorTimer.start();
        }
    }

    Timer {
        id: errorTimer
        interval: 3000
        onTriggered: errorMsg = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Molecular Assembler"
                color: "#f8fafc"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Scanning Tunneling Microscope (STM) atomic manipulation interface"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: errorMsg
                color: "#f43f5e"
                font.bold: true
                visible: errorMsg !== ""
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Assembler Tool Controls
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#1e293b"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Label { text: "STM Probe Controls"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Temperature
                ColumnLayout {
                    spacing: 5
                    Label { text: "Cryo-Chamber Temp (K)"; color: "#94a3b8" }
                    RowLayout {
                        Layout.fillWidth: true
                        Slider {
                            id: tempSlider
                            Layout.fillWidth: true
                            from: 0.1
                            to: 400.0
                            value: OmniAssembler.temperature
                            onValueChanged: OmniAssembler.temperature = value
                            ToolTip.visible: hovered; ToolTip.text: "Adjust base temperature. High temps prevent atomic bonding."
                        }
                        Label {
                            text: OmniAssembler.temperature.toFixed(1) + " K"
                            color: OmniAssembler.temperature > 100 ? "#ef4444" : "#38bdf8"
                            font.bold: true
                        }
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // Status
                ColumnLayout {
                    spacing: 5
                    Label { text: "Tool State:"; color: "#94a3b8" }
                    Label {
                        text: OmniAssembler.toolState
                        color: "#fbbf24"
                        font.pixelSize: 16
                        font.bold: true
                        ToolTip.visible: hovered; ToolTip.text: "Current activity of the STM probe"
                    }
                }

                // Held Item
                ColumnLayout {
                    spacing: 5
                    Label { text: "Tip Payload:"; color: "#94a3b8" }
                    Label {
                        text: OmniAssembler.heldAtomSymbol
                        color: OmniAssembler.heldAtom === 0 ? "#94a3b8" : "#2dd4bf"
                        font.pixelSize: 18
                        font.bold: true
                        ToolTip.visible: hovered; ToolTip.text: "The atom currently held via Van der Waals forces by the probe tip"
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                Label { text: "Manual Operation"; color: "#94a3b8"; font.bold: true }

                // Move Coordinates
                RowLayout {
                    Layout.fillWidth: true
                    TextField { id: xIn; placeholderText: "X (nm)"; Layout.fillWidth: true; validator: DoubleValidator{} }
                    TextField { id: yIn; placeholderText: "Y (nm)"; Layout.fillWidth: true; validator: DoubleValidator{} }
                    TextField { id: zIn; placeholderText: "Z (nm)"; Layout.fillWidth: true; validator: DoubleValidator{} }
                }

                Button {
                    text: "Move Tip"
                    Layout.fillWidth: true
                    onClicked: {
                        if (xIn.text && yIn.text && zIn.text) {
                            OmniAssembler.moveTip(parseFloat(xIn.text), parseFloat(yIn.text), parseFloat(zIn.text));
                        }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Move probe to absolute nanometer coordinates"
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: "Pick Carbon (C)"
                        Layout.fillWidth: true
                        onClicked: OmniAssembler.pickAtom(6)
                        ToolTip.visible: hovered; ToolTip.text: "Extract one Carbon atom from the molecular hopper"
                    }
                    Button {
                        text: "Place Atom"
                        Layout.fillWidth: true
                        onClicked: OmniAssembler.placeAtom()
                        ToolTip.visible: hovered; ToolTip.text: "Bond the currently held atom to the lattice at the current probe coordinates"
                    }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // RIGHT PANEL: Lattice View
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
                        text: "Atomic Lattice Workspace"
                        color: "#a78bfa"
                        font.bold: true
                        font.pixelSize: 18
                    }
                    Item { Layout.fillWidth: true }
                    Button {
                        text: "Macro: Build Diamondoid"
                        onClicked: OmniLattice.generateDiamondoid(4)
                        ToolTip.visible: hovered; ToolTip.text: "Automated sequence: Print a 4x4x4 block of Carbon lattice"
                    }
                    Button {
                        text: "Clear Workspace"
                        onClicked: OmniAssembler.clearLattice()
                        ToolTip.visible: hovered; ToolTip.text: "Purge all atoms from the workspace"
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Total Atoms: " + OmniLattice.atomCount; color: "#94a3b8" }
                    Label { text: "Est. Covalent Bonds: " + OmniLattice.bondCount; color: "#94a3b8" }
                }

                // Simplified 2D projection of 3D lattice
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "#020617"
                    border.color: "#1e293b"

                    Canvas {
                        id: latticeView
                        anchors.fill: parent

                        Connections {
                            target: OmniLattice
                            function onLatticeChanged() { latticeView.requestPaint(); }
                        }

                        Connections {
                            target: OmniAssembler
                            function onTipChanged() { latticeView.requestPaint(); }
                        }

                        onPaint: {
                            var ctx = getContext("2d");
                            ctx.clearRect(0, 0, width, height);

                            var cx = width / 2;
                            var cy = height / 2;
                            var scale = 150; // pixels per nm

                            // Draw Grid
                            ctx.strokeStyle = "#1e293b";
                            ctx.lineWidth = 1;
                            ctx.beginPath();
                            for (var i = 0; i < width; i += 50) { ctx.moveTo(i, 0); ctx.lineTo(i, height); }
                            for (var j = 0; j < height; j += 50) { ctx.moveTo(0, j); ctx.lineTo(width, j); }
                            ctx.stroke();

                            var atoms = OmniLattice.atoms;

                            // Draw atoms
                            for (var k = 0; k < atoms.length; k++) {
                                var a = atoms[k];
                                var ax = cx + (a.x * scale);
                                var ay = cy - (a.y * scale);

                                ctx.beginPath();
                                ctx.arc(ax, ay, 8, 0, 2 * Math.PI);
                                ctx.fillStyle = a.symbol === "C" ? "#cbd5e1" : "#fca5a5";
                                ctx.fill();

                                // Slight depth shading based on Z
                                ctx.strokeStyle = "rgba(0,0,0,0.5)";
                                ctx.lineWidth = 2;
                                ctx.stroke();
                            }

                            // Draw Probe Tip Crosshair
                            var tip = OmniAssembler.tipPosition;
                            var tx = cx + (tip.x * scale);
                            var ty = cy - (tip.y * scale);

                            ctx.beginPath();
                            ctx.moveTo(tx - 15, ty); ctx.lineTo(tx + 15, ty);
                            ctx.moveTo(tx, ty - 15); ctx.lineTo(tx, ty + 15);
                            ctx.strokeStyle = "#38bdf8";
                            ctx.lineWidth = 2;
                            ctx.stroke();

                            // Draw held atom shadow
                            if (OmniAssembler.heldAtom > 0) {
                                ctx.beginPath();
                                ctx.arc(tx, ty, 8, 0, 2 * Math.PI);
                                ctx.fillStyle = "rgba(56, 189, 248, 0.5)"; // Blue ghost
                                ctx.fill();
                            }
                        }
                    }
                }
            }
        }
    }
}
