// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

import BobUIQuick.Controls

Window {

//! [iconFont]
ToolButton {
    id: muteButton
    checkable: true
    icon.name: checked ? "volume_off" : "volume_up"
}
//! [iconFont]

}
