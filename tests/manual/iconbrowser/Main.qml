// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

import BobUIQuick
import BobUIQuick.Controls
import BobUIQuick.Layouts

Rectangle {
    anchors.fill: parent
    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        ToolBar {
            RowLayout {
                anchors.fill: parent
                ToolButton {
                    id: normalButton
                    checkable: true
                    icon.name: checked ? iconNameOn.text : iconNameOff.text
                }
                ToolButton {
                    id: checkedButton
                    checked: true
                    checkable: true
                    icon.name: checked ? iconNameOn.text : iconNameOff.text
                }
                ToolButton {
                    id: disabledButton
                    enabled: false
                    icon.name: checked ? iconNameOn.text : iconNameOff.text
                }
            }
        }

        RowLayout {
            Label {
                text: "Off:"
            }
            TextField {
                id: iconNameOff
                text: "mail-mark-read"
            }
            Label {
                text: "On:"
            }
            TextField {
                id: iconNameOn
                text: "mail-mark-unread"
            }
        }
    }
}
