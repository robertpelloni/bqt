// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/bobuiest.h>
#include <BobUIWidgets/qmainwindow.h>

class test_widgets_app_deployment : public QObject
{
    Q_OBJECT
private slots:
    void canRun();
};

void test_widgets_app_deployment::canRun()
{
    QMainWindow mw;
    mw.show();
    QVERIFY(BOBUIest::qWaitForWindowActive(&mw));
}

BOBUIEST_MAIN(test_widgets_app_deployment)

#include "main.moc"
