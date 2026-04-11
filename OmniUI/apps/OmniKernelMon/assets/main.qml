import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniKernel 1.0
import OmniCharts 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: "OmniKernel Monitor"

    MenuBar {
        Menu {
            title: "File"
            Action { text: "Exit"; onTriggered: Qt.quit() }
        }
        Menu {
            title: "Kernel"
            Action { text: "Panic"; onTriggered: console.log("Kernel Panic Triggered!") }
            Action { text: "GC"; onTriggered: console.log("Garbage Collection Started") }
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Sidebar
        Rectangle {
            Layout.preferredWidth: 250
            Layout.fillHeight: true
            color: "#333"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10

                Text { text: "SYSTEM STATUS"; color: "white"; font.bold: true }

                Text {
                    text: "Processes: " + Kernel.processCount
                    color: "lightgreen"
                }

                Text {
                    text: "CPU Usage: " + Kernel.totalCpuUsage.toFixed(1) + "%"
                    color: Kernel.totalCpuUsage > 80 ? "red" : "white"
                }

                ProgressBar {
                    Layout.fillWidth: true
                    value: Kernel.totalCpuUsage / 100.0
                }

                Text {
                    text: "Memory: " + (Kernel.totalMemoryUsage * 100).toFixed(1) + "%"
                    color: "white"
                }

                ProgressBar {
                    Layout.fillWidth: true
                    value: Kernel.totalMemoryUsage
                }

                Item { Layout.fillHeight: true }

                Button {
                    text: "Spawn Process"
                    Layout.fillWidth: true
                    onClicked: Kernel.createProcess("UserTask", "Normal")
                }
            }
        }

        // Main Content
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10

            TabBar {
                id: bar
                width: parent.width
                TabButton { text: "Processes" }
                TabButton { text: "Performance" }
                TabButton { text: "eBPF" }
            }

            StackLayout {
                currentIndex: bar.currentIndex
                Layout.fillWidth: true
                Layout.fillHeight: true

                // Process List
                ListView {
                    model: 10 // Mock model for now, needs QAbstractListModel wrapper for QMap
                    delegate: Rectangle {
                        width: parent.width
                        height: 40
                        color: index % 2 == 0 ? "#eee" : "#fff"
                        border.color: "#ccc"

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            Text { text: "PID: " + (1000 + index); Layout.preferredWidth: 60 }
                            Text { text: "Name: SystemTask_" + index; Layout.fillWidth: true }
                            Text { text: "State: Running" }
                            Button { text: "Kill"; onClicked: Kernel.killProcess(1000 + index) }
                        }
                    }
                }

                // Performance Charts
                GridLayout {
                    columns: 2

                    Rectangle {
                        Layout.fillWidth: true; Layout.fillHeight: true
                        border.color: "#999"
                        ColumnLayout {
                            anchors.centerIn: parent
                            Text { text: "CPU History" }
                            Oscilloscope {
                                Layout.preferredWidth: 400
                                Layout.preferredHeight: 200
                                running: true
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true; Layout.fillHeight: true
                        border.color: "#999"
                        ColumnLayout {
                            anchors.centerIn: parent
                            Text { text: "Memory Allocation Map" }
                            // Placeholder for heatmap
                            Rectangle { width: 400; height: 200; color: "#ddd" }
                        }
                    }
                }

                // eBPF
                ColumnLayout {
                    Text { text: "eBPF Program Loader"; font.pixelSize: 20 }

                    RowLayout {
                        TextField {
                            id: bpfPath
                            placeholderText: "Path to .o file"
                            Layout.fillWidth: true
                        }
                        Button {
                            text: "Load"
                            onClicked: EBPF.loadProgram("test_prog", "kprobe", bpfPath.text)
                        }
                    }

                    TextArea {
                        Layout.fillWidth: true; Layout.fillHeight: true
                        text: EBPF.lastLog
                        readOnly: true
                        background: Rectangle { color: "#222" }
                        color: "#0f0"
                        font.family: "Monospace"
                    }
                }
            }
        }
    }
}
