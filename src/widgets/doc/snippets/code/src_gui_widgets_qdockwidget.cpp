// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
if (dockWidget->features() & QDockWidget::DockWidgetVerticalTitleBar) {
    // I need to be vertical
} else {
    // I need to be horizontal
}
//! [0]
