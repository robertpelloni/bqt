// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QApplication>
#include <BOBUIextBrowser>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

//! [0]
    BOBUIextBrowser browser;
    QColor linkColor(BobUI::red);
    QString sheet = QString::fromLatin1("a { text-decoration: underline; color: %1 }").arg(linkColor.name());
    browser.document()->setDefaultStyleSheet(sheet);
//! [0]
    browser.setSource(QUrl("../../../html/index.html"));
    browser.resize(800, 600);
    browser.show();

    return app.exec();
}

