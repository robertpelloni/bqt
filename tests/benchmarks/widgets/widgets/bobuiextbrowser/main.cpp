// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QDebug>
#include <BOBUIextBrowser>
#include <bobuiest.h>

class tst_BOBUIextBrowser : public QObject
{
    Q_OBJECT
private slots:
    void largeDocumentsLazyLayout();
};

void tst_BOBUIextBrowser::largeDocumentsLazyLayout()
{
    QString sl;
    for (int i = 0; i < 150000; ++i) {
        sl.append("long long text\n");
    }

    QBENCHMARK {
        BOBUIextBrowser browser;
        browser.setPlainText(sl);
        browser.show();
    }
}

BOBUIEST_MAIN(tst_BOBUIextBrowser)

#include "main.moc"
