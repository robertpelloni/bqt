import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Hardware 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniHardware GPIO Header Monitor v41.0.0"
    color: "#0f172a" // Slate 900

    property string alertMessage: ""

    Connections {
        target: OmniGPIOController
        function onHardwareError(msg) {
            alertMessage = "HARDWARE FAULT: " + msg;
            alertTimer.start();
        }
    }

    Timer {
        id: alertTimer
        interval: 3000
        onTriggered: alertMessage = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Embedded Systems Lab"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Interact directly with physical GPIO pins and microcontrollers"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: alertMessage
                color: "#f43f5e"
                font.bold: true
                visible: alertMessage !== ""
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // TOP PANEL: Hardware Layout Map
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 350
            color: "#1e293b"
            radius: 8
            border.color: "#334155"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15

                Label { text: "Raspberry Pi Style 40-Pin Header"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // The dual row GPIO drawing
                GridView {
                    id: pinGrid
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    cellWidth: width / 20
                    cellHeight: height / 2
                    model: OmniGPIOController.pins

                    delegate: Rectangle {
                        width: pinGrid.cellWidth - 4
                        height: pinGrid.cellHeight - 4
                        color: {
                            if (modelData.direction === "5V") return "#ef4444"
                            if (modelData.direction === "3.3V") return "#f97316"
                            if (modelData.direction === "GND") return "#000000"
                            if (modelData.direction === "IN") return "#475569"
                            if (modelData.direction === "PWM") return "#a855f7"

                            return modelData.value ? "#22c55e" : "#1e293b" // OUT active/inactive
                        }
                        border.color: "#64748b"
                        border.width: 1
                        radius: 2

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 2
                            Label { text: modelData.id; color: "#94a3b8"; font.pixelSize: 10; Layout.alignment: Qt.AlignHCenter }
                            Label {
                                text: modelData.direction
                                color: "#f8fafc"
                                font.pixelSize: 10
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Label {
                                text: modelData.voltage.toFixed(1) + "v"
                                color: "#cbd5e1"
                                font.pixelSize: 9
                                Layout.alignment: Qt.AlignHCenter
                                visible: modelData.direction !== "GND"
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            ToolTip.visible: containsMouse
                            ToolTip.text: "Pin " + modelData.id + " | Mode: " + modelData.direction + " | Val: " + (modelData.value ? "HIGH" : "LOW")
                        }
                    }
                }
            }
        }

        // BOTTOM PANEL: Logic Controls
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#0f172a"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15
                    spacing: 15

                    Label { text: "Digital Logic Control"; color: "#fbbf24"; font.bold: true; font.pixelSize: 18 }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        TextField { id: pinId; placeholderText: "Pin Number (e.g. 11)"; Layout.fillWidth: true; validator: IntValidator{bottom: 1; top: 40} }
                        ComboBox {
                            id: dirSelect
                            model: ["IN", "OUT", "PWM"]
                            Layout.fillWidth: true
                        }
                        Button {
                            text: "Configure Pin"
                            onClicked: {
                                if (pinId.text) {
                                    OmniGPIOController.setPinDirection(parseInt(pinId.text), dirSelect.currentText)
                                }
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Set multiplexer pin behavior mode"
                        }
                    }

                    Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        TextField { id: togglePinId; placeholderText: "Pin Number"; Layout.fillWidth: true; validator: IntValidator{bottom: 1; top: 40} }
                        Button {
                            text: "Toggle HIGH/LOW"
                            onClicked: {
                                if (togglePinId.text) OmniGPIOController.togglePin(parseInt(togglePinId.text))
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Flip a configured OUT pin's logic state"
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        TextField { id: pwmPin; placeholderText: "PWM Pin"; Layout.fillWidth: true; validator: IntValidator{bottom: 1; top: 40} }
                        Slider {
                            id: pwmDuty
                            from: 0; to: 100; value: 50
                            Layout.fillWidth: true
                        }
                        Button {
                            text: "Set Duty Cycle (" + Math.round(pwmDuty.value) + "%)"
                            onClicked: {
                                if (pwmPin.text) OmniGPIOController.setPWM(parseInt(pwmPin.text), Math.round(pwmDuty.value))
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Apply analog wave emulation via Pulse Width Modulation"
                        }
                    }
                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}
