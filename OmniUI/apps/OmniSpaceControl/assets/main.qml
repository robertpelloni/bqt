import BobUIQuick 2.15
import BobUIQuick.Layouts 1.15
import BobUIQuick.Controls 2.15
import OmniUI 1.0
import OmniSpace 1.0

ApplicationWindow {
    visible: true
    width: 1920
    height: 1080
    title: "OmniSpace Mission Control"
    color: "#050505"

    GridLayout {
        anchors.fill: parent
        columns: 3

        // Star Map
        Rectangle {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#000"
            border.color: "#333"

            Text { text: "SECTOR: " + StarMap.currentSector; color: "cyan"; font.pixelSize: 24; anchors.top: parent.top; anchors.left: parent.left; anchors.margins: 20 }

            // Mock visualization
            Canvas {
                anchors.fill: parent
                onPaint: {
                    var ctx = getContext("2d");
                    ctx.fillStyle = "white";
                    for(var i=0; i<StarMap.starCount; i++) {
                        var pos = StarMap.getStarPosition(i);
                        // Simple 3D projection
                        var x = width/2 + pos.x * 10;
                        var y = height/2 + pos.y * 10;
                        ctx.fillRect(x, y, 2, 2);
                    }
                }
            }
        }

        // Telemetry
        ColumnLayout {
            Layout.fillHeight: true
            Layout.preferredWidth: 400
            spacing: 20

            Rectangle {
                Layout.fillWidth: true
                height: 300
                color: "#111"
                border.color: "orange"

                ColumnLayout {
                    anchors.centerIn: parent
                    Text { text: "VELOCITY (km/s)"; color: "orange" }
                    Text { text: Telemetry.velocity.x.toFixed(2); font.pixelSize: 40; color: "white" }

                    Text { text: "ALTITUDE (km)"; color: "orange" }
                    Text { text: Telemetry.altitude.toFixed(1); font.pixelSize: 40; color: "white" }

                    Text { text: "G-FORCE"; color: "orange" }
                    Text { text: Telemetry.gForce.toFixed(2); font.pixelSize: 40; color: Telemetry.gForce > 5 ? "red" : "white" }
                }
            }

            // Propulsion Control
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#111"
                border.color: "red"

                ColumnLayout {
                    anchors.centerIn: parent

                    Text { text: "MAIN THRUSTER"; color: "red"; font.bold: true }

                    Dial {
                        id: throttleDial
                        from: 0; to: 100
                        value: Propulsion.throttle * 100
                        onMoved: Propulsion.throttle = value / 100
                    }

                    Text { text: Math.round(throttleDial.value) + "%"; color: "white" }

                    Button {
                        text: "EMERGENCY STOP"
                        palette.button: "red"
                        onClicked: Propulsion.emergencyStop()
                    }

                    Text { text: "FUEL: " + Propulsion.fuelLevel + "%"; color: "yellow" }
                }
            }
        }

        // Orbital Params
        Rectangle {
            Layout.columnSpan: 3
            Layout.preferredHeight: 200
            Layout.fillWidth: true
            color: "#111"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 50

                Column {
                    Text { text: "ECCENTRICITY"; color: "#888" }
                    Text { text: Mechanics.eccentricity.toFixed(4); color: "white"; font.pixelSize: 24 }
                }

                Column {
                    Text { text: "SEMI-MAJOR AXIS"; color: "#888" }
                    Text { text: Mechanics.semiMajorAxis.toFixed(0) + " km"; color: "white"; font.pixelSize: 24 }
                }

                Column {
                    Text { text: "INCLINATION"; color: "#888" }
                    Text { text: Mechanics.inclination.toFixed(2) + "°"; color: "white"; font.pixelSize: 24 }
                }

                Column {
                    Text { text: "PERIOD"; color: "#888" }
                    Text { text: (Mechanics.period / 3600).toFixed(2) + " hrs"; color: "white"; font.pixelSize: 24 }
                }
            }
        }
    }

    Timer {
        interval: 100; running: true; repeat: true
        onTriggered: Telemetry.updateState(0.1)
    }
}
