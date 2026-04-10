// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

//! [0]
w->setWindowState(w->windowState() ^ BobUI::WindowFullScreen);
//! [0]


//! [1]
w->setWindowState((w->windowState() & ~BobUI::WindowMinimized) | BobUI::WindowActive);
//! [1]


//! [2]
width = baseSize().width() + i * sizeIncrement().width();
height = baseSize().height() + j * sizeIncrement().height();
//! [2]


//! [3]
aWidget->window()->setWindowTitle("New Window Title");
//! [3]


//! [6]
setCursor(BobUI::IBeamCursor);
//! [6]


//! [7]
QPixmap pixmap(widget->size());
widget->render(&pixmap);
//! [7]


//! [8]
QPainter painter(this);
...
painter.end();
myWidget->render(this);
//! [8]


//! [9]
setTabOrder(a, b); // a to b
setTabOrder(b, c); // a to b to c
setTabOrder(c, d); // a to b to c to d
//! [9]


//! [9.list]
setTabOrder({a, b, c, d}); // a to b to c to d
//! [9.list]


//! [10]
// WRONG
setTabOrder(c, d); // c to d
setTabOrder(a, b); // a to b AND c to d
setTabOrder(b, c); // a to b to c, but not c to d
//! [10]


//! [11]
void MyWidget::closeEvent(QCloseEvent *event)
{
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("geometry", saveGeometry());
    QWidget::closeEvent(event);
}
//! [11]


//! [12]
QSettings settings("MyCompany", "MyApp");
myWidget->restoreGeometry(settings.value("myWidget/geometry").toByteArray());
//! [12]


//! [13]
setUpdatesEnabled(false);
bigVisualChanges();
setUpdatesEnabled(true);
//! [13]
