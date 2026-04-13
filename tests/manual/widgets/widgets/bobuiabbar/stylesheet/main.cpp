// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

// This test is for checking that when there is padding set on the stylesheet and the elide mode is
// set that it is correctly shown as elided and not clipped.

#include <QApplication>
#include <BOBUIabBar>
#include <QIcon>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setStyleSheet("BOBUIabBar::tab { padding-left: 20px; }\n");
    QIcon icon(":/v.ico");

    BOBUIabBar b;
    b.setElideMode(BobUI::ElideRight);
    b.addTab(icon, "some text");
    b.resize(80,32);
    b.show();

    return app.exec();
}
