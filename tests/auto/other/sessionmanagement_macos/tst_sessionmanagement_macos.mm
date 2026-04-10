// Copyright (C) 2019 Samuel Gaist <samuel.gaist@idiap.ch>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <QSignalSpy>
#include <QSessionManager>
#include <AppKit/AppKit.h>

// Q_DECLARE_METATYPE(QSessionManager)

class tst_SessionManagement_macOS : public QObject
{
    Q_OBJECT

private slots:
    void stopApplication();
};

/*
    Test that session handling code is properly called
*/
void tst_SessionManagement_macOS::stopApplication()
{
    int argc = 0;
    QGuiApplication app(argc, nullptr);
    QSignalSpy spy(&app, &QGuiApplication::commitDataRequest);
    BOBUIimer::singleShot(1000, []() {
         [NSApp terminate:nil];
    });
    app.exec();
    QCOMPARE(spy.count(), 1);
}

BOBUIEST_APPLESS_MAIN(tst_SessionManagement_macOS)
#include "tst_sessionmanagement_macos.moc"
