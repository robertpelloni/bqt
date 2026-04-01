import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import OmniUI 1.0
import OmniBioStorage 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniBioExplorer - DNA Storage Manager"

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // Left Panel: Encoder/Decoder
        ColumnLayout {
            SplitView.preferredWidth: 400
            SplitView.fillHeight: true
            Layout.margins: 10

            Text { text: "Data Encoding"; font.bold: true; font.pixelSize: 18 }

            TextArea {
                id: inputData
                Layout.fillWidth: true
                Layout.preferredHeight: 150
                placeholderText: "Enter text to encode..."
                background: Rectangle { color: "#eee" }
            }

            Button {
                text: "Encode to DNA"
                Layout.fillWidth: true
                onClicked: {
                    var dna = DNAEncoder.encodeText(inputData.text)
                    container.sequence = dna // Update binding logic? No, just set text
                    outputDNA.text = dna
                    container.name = "seq_" + Date.now()
                }
            }

            Text { text: "Encoded Sequence (ACGT)"; font.bold: true }

            TextArea {
                id: outputDNA
                Layout.fillWidth: true
                Layout.fillHeight: true
                readOnly: true
                wrapMode: Text.Wrap
                font.family: "Monospace"
                background: Rectangle { color: "#222" }
                color: "#0f0" // DNA Matrix look
            }

            RowLayout {
                TextField {
                    id: filenameInput
                    Layout.fillWidth: true
                    placeholderText: "Filename (e.g. data.dna)"
                    text: container.name
                }

                Button {
                    text: "Synthesize & Store"
                    onClicked: {
                        container.name = filenameInput.text
                        container.append(outputDNA.text) // Logic check: append duplicates?
                        // Actually, just save direct
                        BioDrive.storeFile(filenameInput.text, outputDNA.text)
                        console.log("Stored " + outputDNA.text.length + " bases")
                    }
                }
            }
        }

        // Right Panel: Storage Drive View
        ColumnLayout {
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            Layout.margins: 10

            Text { text: "Bio-Storage Drive Status"; font.bold: true; font.pixelSize: 18 }

            RowLayout {
                Text { text: "Files: " + BioDrive.fileCount }
                Text { text: "Used Space: " + BioDrive.usedSpace + " bases" }
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: BioDrive.files
                delegate: Rectangle {
                    width: parent.width
                    height: 50
                    color: index % 2 == 0 ? "#f0f0f0" : "#fff"
                    border.color: "#ccc"

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 5
                        Text { text: modelData; font.bold: true; Layout.fillWidth: true }

                        Button {
                            text: "Sequence (Read)"
                            onClicked: {
                                var seq = BioDrive.retrieveFile(modelData)
                                var decoded = DNADecoder.decodeToText(seq)
                                resultDialog.text = decoded
                                resultDialog.open()
                            }
                        }

                        Button {
                            text: "Delete"
                            onClicked: BioDrive.deleteFile(modelData)
                        }
                    }
                }
            }
        }
    }

    BioContainer {
        id: container
    }

    Dialog {
        id: resultDialog
        property alias text: msg.text
        title: "Decoded Data"
        standardButtons: Dialog.Ok
        anchors.centerIn: parent
        width: 400

        Text {
            id: msg
            anchors.centerIn: parent
            width: parent.width - 20
            wrapMode: Text.Wrap
        }
    }
}
