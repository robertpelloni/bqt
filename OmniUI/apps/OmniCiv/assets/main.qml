import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Galactic 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "OmniGalactic Civilization Hub v41.0.0"
    color: "#020617" // Space black

    property int selectedSystem: 0

    header: ToolBar {
        background: Rectangle { color: "#0f172a" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Galactic Strategy Command"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Manage your intergalactic empire expansion"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: "Civilization: " + OmniCivilization.name
                color: "#60a5fa"
                font.bold: true
                font.pixelSize: 16
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Civilization Stats & Target Info
        Rectangle {
            Layout.preferredWidth: 350
            Layout.fillHeight: true
            color: "#0f172a"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                Label { text: "Empire Demographics"; color: "#fbbf24"; font.bold: true; font.pixelSize: 18 }

                // Population
                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Population:"; color: "#94a3b8"; Layout.preferredWidth: 100 }
                    Label { text: OmniCivilization.population.toFixed(1) + " Billion"; color: "#f8fafc"; font.bold: true; Layout.fillWidth: true }
                    ToolTip.visible: hovered; ToolTip.text: "Total sapient beings under your control"
                }

                // Tech
                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Tech Level:"; color: "#94a3b8"; Layout.preferredWidth: 100 }
                    Label { text: "Tier " + OmniCivilization.technologyLevel; color: "#34d399"; font.bold: true; Layout.fillWidth: true }
                    ToolTip.visible: hovered; ToolTip.text: "Technological advancement. Higher tiers increase hyperdrive speeds."
                }

                // Govt
                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Government:"; color: "#94a3b8"; Layout.preferredWidth: 100 }
                    Label { text: OmniCivilization.governmentType; color: "#f8fafc"; font.bold: true; Layout.fillWidth: true }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                Label { text: "Selected Star System [" + selectedSystem + "]"; color: "#c084fc"; font.bold: true; font.pixelSize: 18 }

                // System Info (dynamically fetched)
                Item {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 150

                    ColumnLayout {
                        anchors.fill: parent
                        property var sysData: OmniGalacticMap.getSystem(selectedSystem)

                        Label { text: "Name: " + sysData.name; color: "#e2e8f0"; font.bold: true; font.pixelSize: 16 }
                        Label { text: "Star Class: " + sysData.type; color: "#fb923c" }
                        Label { text: "Mass: " + sysData.mass + " M☉"; color: "#94a3b8" }
                        Label { text: "Distance: " + sysData.distance_to_core.toFixed(0) + " ly"; color: "#94a3b8" }
                        Label {
                            text: sysData.controlled ? "STATUS: SECURED" : "STATUS: UNCHARTED"
                            color: sysData.controlled ? "#22c55e" : "#f43f5e"
                            font.bold: true
                        }
                    }
                }

                Item { Layout.fillHeight: true }

                Label { text: "Fleet Command"; color: "#94a3b8"; font.bold: true }

                Button {
                    text: "Dispatch Colonization Fleet"
                    Layout.fillWidth: true
                    enabled: selectedSystem !== 0 // Cannot dispatch to homeworld
                    onClicked: {
                        OmniGalacticMap.plotHyperlaneRoute(0, selectedSystem, OmniCivilization.technologyLevel);
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Send a fleet via hyperlanes from Sol to secure the selected star system"
                }
            }
        }

        // RIGHT PANEL: Galaxy Map & Fleet Logs
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // Interactive Galaxy Map Canvas
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#050505"
                radius: 8
                border.color: "#334155"
                clip: true

                Label {
                    text: "Milky Way Local Group Map (Top-Down Projection)"
                    color: "#475569"
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.margins: 10
                    z: 10
                }

                Canvas {
                    id: galaxyMap
                    anchors.fill: parent

                    Connections {
                        target: OmniGalacticMap
                        function onMapChanged() { galaxyMap.requestPaint(); }
                    }

                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.clearRect(0, 0, width, height);

                        var cx = width / 2;
                        var cy = height / 2;

                        // Galaxy scale approx (drawing a 100,000 ly box)
                        var scale = Math.min(width, height) / 100000;

                        var systems = OmniGalacticMap.systems;

                        // Draw galactic core glow
                        var gradient = ctx.createRadialGradient(cx, cy, 0, cx, cy, 200);
                        gradient.addColorStop(0, "rgba(255, 255, 200, 0.8)");
                        gradient.addColorStop(0.5, "rgba(200, 200, 255, 0.2)");
                        gradient.addColorStop(1, "rgba(0, 0, 0, 0)");
                        ctx.beginPath();
                        ctx.arc(cx, cy, 200, 0, 2 * Math.PI);
                        ctx.fillStyle = gradient;
                        ctx.fill();

                        for (var i = 0; i < systems.length; i++) {
                            var s = systems[i];
                            var sx = cx + (s.x * scale);
                            var sy = cy - (s.y * scale);

                            // Draw connecting lines if controlled (trade routes)
                            if (s.controlled && s.id !== 0) {
                                var sol = systems[0];
                                ctx.beginPath();
                                ctx.moveTo(cx + sol.x*scale, cy - sol.y*scale);
                                ctx.lineTo(sx, sy);
                                ctx.strokeStyle = "rgba(56, 189, 248, 0.3)"; // Blue hyperlanes
                                ctx.lineWidth = 1;
                                ctx.stroke();
                            }

                            // Draw star
                            ctx.beginPath();
                            ctx.arc(sx, sy, s.id === selectedSystem ? 6 : 3, 0, 2 * Math.PI);

                            // Color by control and selection
                            if (s.id === selectedSystem) ctx.fillStyle = "#c084fc"; // Purple highlight
                            else if (s.controlled) ctx.fillStyle = "#38bdf8"; // Empire Blue
                            else {
                                // Natural star color
                                if (s.type === "O" || s.type === "B") ctx.fillStyle = "#93c5fd";
                                else if (s.type === "M") ctx.fillStyle = "#fca5a5";
                                else if (s.type === "BlackHole") ctx.fillStyle = "#000000";
                                else ctx.fillStyle = "#fef08a";
                            }

                            ctx.fill();

                            // Draw white border for black holes so they are visible
                            if (s.type === "BlackHole" && !s.controlled && s.id !== selectedSystem) {
                                ctx.strokeStyle = "white";
                                ctx.lineWidth = 1;
                                ctx.stroke();
                            }
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: (mouse) => {
                            var cx = width / 2;
                            var cy = height / 2;
                            var scale = Math.min(width, height) / 100000;
                            var systems = OmniGalacticMap.systems;

                            // Find closest star to click
                            var closestId = -1;
                            var minDist = 99999;

                            for (var i = 0; i < systems.length; i++) {
                                var s = systems[i];
                                var sx = cx + (s.x * scale);
                                var sy = cy - (s.y * scale);

                                var dx = mouse.x - sx;
                                var dy = mouse.y - sy;
                                var dist = Math.sqrt(dx*dx + dy*dy);

                                if (dist < 10 && dist < minDist) {
                                    minDist = dist;
                                    closestId = s.id;
                                }
                            }

                            if (closestId !== -1) {
                                selectedSystem = closestId;
                                galaxyMap.requestPaint(); // Redraw selection highlight
                            }
                        }
                    }
                }
            }

            // Transit / Fleet Log
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 180
                color: "#1e293b"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label { text: "Fleet Transit Logs"; color: "#34d399"; font.bold: true }

                    ListView {
                        id: logView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: OmniGalacticMap.routeLog

                        delegate: RowLayout {
                            width: logView.width
                            height: 25
                            spacing: 10

                            Label {
                                text: "[" + modelData.status + "]"
                                color: modelData.status === "ACTIVE" ? "#fbbf24" : "#38bdf8"
                                font.bold: true
                                font.family: "monospace"
                                Layout.preferredWidth: 80
                            }

                            Label {
                                text: modelData.msg
                                color: "#e2e8f0"
                                font.family: "monospace"
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }
    }
}
