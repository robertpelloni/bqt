import BobUIQuick 2.15
import BobUIQuick.Controls 2.15
import BobUIQuick.Layouts 1.15
import Omni.Web3 1.0

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "OmniWeb3 Blockchain Gateway v41.0.0"
    color: "#0f172a" // Slate 900

    property string alertMessage: ""

    Connections {
        target: OmniWallet
        function onErrorOccurred(msg) {
            alertMessage = "TRANSACTION FAILED: " + msg;
            alertTimer.start();
        }
        function onTransactionMined(hash) {
            alertMessage = "CONFIRMED: Block Mined " + hash;
            alertTimer.start();
        }
    }

    Timer {
        id: alertTimer
        interval: 4000
        onTriggered: alertMessage = ""
    }

    header: ToolBar {
        background: Rectangle { color: "#1e293b" }
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 20

            Label {
                text: "Decentralized Node Interactor"
                color: "#e2e8f0"
                font.pixelSize: 20
                font.bold: true
                ToolTip.visible: hovered
                ToolTip.text: "Interact with Smart Contracts and on-chain assets"
            }

            Item { Layout.fillWidth: true }

            Label {
                text: alertMessage
                color: alertMessage.indexOf("FAILED") !== -1 ? "#ef4444" : "#34d399"
                font.bold: true
                visible: alertMessage !== ""
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // LEFT PANEL: Wallet Info
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

                Label { text: "Wallet Profile"; color: "#38bdf8"; font.bold: true; font.pixelSize: 18 }

                // Address
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Public Key Address"; color: "#94a3b8" }
                    Label {
                        text: OmniWallet.address
                        color: "#f8fafc"
                        font.family: "monospace"
                        font.bold: true
                        font.pixelSize: 12
                        wrapMode: Text.WrapAnywhere
                        Layout.fillWidth: true
                    }
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // Balance
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Account Balance (ETH)"; color: "#94a3b8" }
                    Label {
                        text: "Ξ " + OmniWallet.balance.toFixed(4)
                        color: OmniWallet.balance > 0 ? "#22c55e" : "#ef4444"
                        font.pixelSize: 36
                        font.bold: true
                        font.family: "monospace"
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Current liquid assets available to pay for gas or transfers"
                }

                Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                // Network Stats
                ColumnLayout {
                    spacing: 5
                    Layout.fillWidth: true
                    Label { text: "Network Telemetry"; color: "#94a3b8" }
                    RowLayout {
                        Layout.fillWidth: true
                        Label { text: "Chain: " + OmniWallet.network; color: "#cbd5e1" }
                        Item { Layout.fillWidth: true }
                        Label { text: "Gas: " + OmniWallet.gasPrice.toFixed(1) + " gwei"; color: OmniWallet.gasPrice > 100 ? "#ef4444" : "#fbbf24" }
                    }
                    ToolTip.visible: hovered; ToolTip.text: "Current connected node and global mempool transaction fee"
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Button {
                        text: OmniWallet.connected ? "Disconnect Node" : "Connect MetaMask"
                        Layout.fillWidth: true
                        onClicked: OmniWallet.connected ? OmniWallet.disconnectWallet() : OmniWallet.connectWallet("metamask")
                        ToolTip.visible: hovered; ToolTip.text: "Authenticate with a Web3 provider to interact with the blockchain"
                    }
                }
            }
        }

        // RIGHT PANEL: Actions & Mempool Log
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // Interaction Testing
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 200
                color: "#0f172a"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label { text: "Smart Contract & Transfer Functions"; color: "#c084fc"; font.bold: true; font.pixelSize: 18 }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        TextField {
                            id: transferAddr
                            placeholderText: "Recipient 0x..."
                            Layout.fillWidth: true
                            color: "#e2e8f0"
                        }
                        TextField {
                            id: transferVal
                            placeholderText: "Amount (ETH)"
                            Layout.preferredWidth: 100
                            color: "#e2e8f0"
                            validator: DoubleValidator{bottom: 0.0}
                        }
                        Button {
                            text: "Send Transaction"
                            enabled: OmniWallet.connected
                            onClicked: {
                                if (transferAddr.text && transferVal.text) {
                                    OmniWallet.sendTransaction(transferAddr.text, parseFloat(transferVal.text));
                                    transferAddr.text = ""; transferVal.text = "";
                                }
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Sign and broadcast a transfer to the mempool"
                        }
                    }

                    Rectangle { height: 1; Layout.fillWidth: true; color: "#334155" }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10
                        TextField {
                            id: mintAmt
                            placeholderText: "Amount of OmniTokens to mint"
                            Layout.fillWidth: true
                            color: "#e2e8f0"
                            validator: DoubleValidator{bottom: 0.0}
                        }
                        Button {
                            text: "Call mint() Contract"
                            enabled: OmniWallet.connected
                            onClicked: {
                                if (mintAmt.text) {
                                    OmniWallet.mintTokens(parseFloat(mintAmt.text));
                                    mintAmt.text = "";
                                }
                            }
                            ToolTip.visible: hovered; ToolTip.text: "Interact with an ERC-20 contract to mint synthetic tokens (Costs gas)"
                        }
                    }
                }
            }

            // Ledger Log
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#1e293b"
                radius: 8
                border.color: "#334155"

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 15

                    Label {
                        text: "Local Transaction Ledger"
                        color: "#34d399"
                        font.bold: true
                    }

                    ListView {
                        id: ledgerView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: OmniWallet.transactions
                        spacing: 10

                        delegate: Rectangle {
                            width: ledgerView.width
                            height: 60
                            color: "#0f172a"
                            radius: 4
                            border.color: "#334155"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 10

                                ColumnLayout {
                                    Layout.fillWidth: true
                                    spacing: 2
                                    Label { text: "To: " + modelData.to; color: "#e2e8f0"; font.family: "monospace" }
                                    Label { text: "Tx: " + modelData.hash; color: "#64748b"; font.family: "monospace"; font.pixelSize: 10; elide: Text.ElideMiddle }
                                }

                                Rectangle { width: 1; Layout.fillHeight: true; color: "#334155" }

                                Label {
                                    text: "Ξ " + modelData.value.toFixed(4)
                                    color: "#f8fafc"
                                    font.family: "monospace"
                                    Layout.preferredWidth: 80
                                }

                                Rectangle { width: 1; Layout.fillHeight: true; color: "#334155" }

                                Label {
                                    text: modelData.status
                                    color: modelData.status === "Confirmed" ? "#10b981" : "#fbbf24"
                                    font.bold: true
                                    font.family: "monospace"
                                    Layout.preferredWidth: 80
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
