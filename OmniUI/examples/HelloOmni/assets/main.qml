import QtQuick 2.15
<<<<<<< HEAD
import QtQuick.Window 2.15
import OmniUI 1.0
import OmniLayout 1.0
import OmniData 1.0
import OmniNet 1.0
import OmniAudio 1.0

Window {
    id: rootWindow
    visible: true
    width: 1280
    height: 720
    title: "OmniOS v4.4.0 - The Multi-Cursor Desktop Environment"
    color: ThemeManager.backgroundColor

    property bool isLoggedIn: false

    Component.onCompleted: {
        Database.connect("omni_os_system.db");
        TimeMachine.initializeLedger("omni_os_ledger.db");
        MasterClock.start();
        ThemeManager.setCyberpunkMode();
    }

    Item {
        id: loginScreen
        anchors.fill: parent
        visible: !rootWindow.isLoggedIn
        z: 100

        Rectangle {
            anchors.fill: parent
            color: ThemeManager.surfaceColor
            opacity: 0.9
        }

        Column {
            anchors.centerIn: parent
            spacing: 20
            width: 300

            Text {
                text: "O M N I . O S"
                font.pixelSize: 42
                font.bold: true
                color: ThemeManager.primaryColor
                anchors.horizontalCenter: parent.horizontalCenter
            }

            TextField {
                id: usernameField
                width: parent.width
                placeholderText: "Username (admin)"
            }

            TextField {
                id: passwordField
                width: parent.width
                placeholderText: "Password (admin)"
            }

            Button {
                width: parent.width
                text: "INITIALIZE KERNEL"
                onClicked: {
                    if (usernameField.text === "admin" && passwordField.text === "admin") {
                        NotificationCenter.showToast("System Online", "Authentication successful. Welcome to OmniOS.", "success");
                        rootWindow.isLoggedIn = true;
                    } else {
                        NotificationCenter.showToast("Access Denied", "Invalid credentials.", "error");
                    }
                }
            }
        }
    }

    Item {
        id: desktopArea
        anchors.fill: parent
        visible: rootWindow.isLoggedIn

        Item {
            id: windowContainer
            anchors.fill: parent
        }

        Rectangle {
            id: taskbar
            height: 48
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            color: Qt.darker(ThemeManager.surfaceColor, 1.2)

            Row {
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                spacing: 10

                Button {
                    text: "Start"
                    width: 80
                    height: 36
                    onClicked: startMenu.visible = !startMenu.visible
                }
                
                Button {
                    text: "Dev Mode: " + (InputManager.devMode ? "ON" : "OFF")
                    width: 120
                    height: 36
                    omniStyle: "secondary"
                    onClicked: InputManager.devMode = !InputManager.devMode
                }
            }

            Text {
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                text: "v4.4.0 | DSP Sync Active"
                color: ThemeManager.textColor
                font.pixelSize: 12
            }
        }

        Rectangle {
            id: startMenu
            visible: false
            width: 250
            height: 350
            anchors.bottom: taskbar.top
            anchors.left: parent.left
            anchors.bottomMargin: 5
            anchors.leftMargin: 5
            color: ThemeManager.surfaceColor
            border.color: ThemeManager.primaryColor
            border.width: 1
            radius: 8

            Column {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Text { text: "Applications"; color: ThemeManager.textColor; font.bold: true }

                Button {
                    width: parent.width; text: "Terminal"
                    onClicked: { startMenu.visible = false; spawnApp("Terminal"); }
                }
                Button {
                    width: parent.width; text: "Code Editor"
                    onClicked: { startMenu.visible = false; spawnApp("CodeEditor"); }
                }
                Button {
                    width: parent.width; text: "Audio DSP (JUCE)"
                    onClicked: { startMenu.visible = false; spawnApp("AudioDSP"); }
                }

                Item { height: 20; width: 1 }

                Text { text: "Theme Engine"; color: ThemeManager.textColor; font.bold: true }
                
                Button { width: parent.width; text: "Cyberpunk Mode"; omniStyle: "secondary"; onClicked: ThemeManager.setCyberpunkMode() }
                Button { width: parent.width; text: "Liquid Glass Mode"; omniStyle: "secondary"; onClicked: ThemeManager.setLiquidGlassMode() }
                Button { width: parent.width; text: "ChronosShift Mode"; omniStyle: "secondary"; onClicked: ThemeManager.setChronosShiftMode() }
            }
        }
    }

    function spawnApp(appType) {
        var componentString = "";
        
        if (appType === "Terminal") {
            componentString = 'import QtQuick 2.15; import OmniUI 1.0; import OmniLayout 1.0; Window { title: "Root Terminal"; x: 50; y: 50; width: 600; height: 400; Terminal { anchors.fill: parent; anchors.topMargin: 30 } }';
        } else if (appType === "CodeEditor") {
            componentString = 'import QtQuick 2.15; import OmniUI 1.0; import OmniLayout 1.0; Window { title: "OmniCode v1.0"; x: 100; y: 100; width: 700; height: 500; CodeEditor { anchors.fill: parent; anchors.topMargin: 30; text: "#include <iostream>\n\nint main() {\n    std::cout << \\"Hello OmniOS!\\";\n    return 0;\n}" } }';
        } else if (appType === "AudioDSP") {
            componentString = 'import QtQuick 2.15; import OmniUI 1.0; import OmniLayout 1.0; import OmniAudio 1.0; Window { title: "JUCE DSP Bridge"; x: 200; y: 150; width: 500; height: 400; Column { anchors.fill: parent; anchors.topMargin: 40; spacing: 10; Slider { width: parent.width } Text { color: "white"; text: "Cutoff Frequency" } JuceView { width: parent.width; height: 200 } } }';
        }

        if (componentString !== "") {
            Qt.createQmlObject(componentString, windowContainer, "dynamicApp");
        }
    }
=======
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Omni.Gfx 1.0 // Map 3D to Gfx namespace since we created it in 3d folder

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "Omni3D Rendering Engine v41.0.0"
    color: "#0f172a" // Slate 900

    OmniCamera {
        id: mainCamera
        position: Qt.vector3d(0, 0, -10) // Looking from -Z towards +Z
        target: Qt.vector3d(0, 0, 0)
        fov: 60.0
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Software 3D Rasterizer"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Custom built CPU-based 3D rendering pipeline"
            }

            Item { Layout.fillWidth: true }

            Button {
                text: "Spawn Object"
                onClicked: {
                    var comp = Qt.createComponent("TestCube.qml");
                    var obj = comp.createObject(viewport);
                    viewport.addMesh(obj);
                }
            }
            Button {
                text: "Clear Scene"
                onClicked: viewport.clearScene()
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Camera & Scene Settings
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

                Label { text: "Scene Settings"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Camera Position X
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Label { text: "Camera X:"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: mainCamera.position.x.toFixed(1); color: "#f8fafc" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: -20; to: 20; value: mainCamera.position.x
                        onValueChanged: mainCamera.position = Qt.vector3d(value, mainCamera.position.y, mainCamera.position.z)
                    }
                }

                // Camera Position Y
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Label { text: "Camera Y (Elevation):"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: mainCamera.position.y.toFixed(1); color: "#f8fafc" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: -20; to: 20; value: mainCamera.position.y
                        onValueChanged: mainCamera.position = Qt.vector3d(mainCamera.position.x, value, mainCamera.position.z)
                    }
                }

                // Camera Position Z
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 5
                    RowLayout {
                        Label { text: "Camera Z (Depth):"; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: mainCamera.position.z.toFixed(1); color: "#f8fafc" }
                    }
                    Slider {
                        Layout.fillWidth: true
                        from: -20; to: 20; value: mainCamera.position.z
                        onValueChanged: mainCamera.position = Qt.vector3d(mainCamera.position.x, mainCamera.position.y, value)
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                Label { text: "Renderer Stats"; color: "#a1a1aa"; font.bold: true }

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Active Meshes:"; color: "#94a3b8" }
                    Item { Layout.fillWidth: true }
                    Label { text: viewport.activeMeshes; color: "#60a5fa"; font.bold: true }
                }
                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Vertex Count:"; color: "#94a3b8" }
                    Item { Layout.fillWidth: true }
                    Label { text: viewport.vertexCount; color: "#fb923c"; font.bold: true }
                }

                Item { Layout.fillHeight: true }
            }
        }

        // RIGHT PANEL: The Viewport
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#050505"
            radius: 8
            border.color: "#334155"
            border.width: 2
            clip: true

            OmniView3D {
                id: viewport
                anchors.fill: parent
                camera: mainCamera
                wireframeMode: true
                renderLoop: true

                Component.onCompleted: {
                    var comp = Qt.createComponent("TestCube.qml");
                    viewport.addMesh(comp.createObject(viewport, { "position": Qt.vector3d(0, 0, 0) }));
                }
            }
        }
    }
>>>>>>> feature/omni-ui-framework-18001284211800334382
}
