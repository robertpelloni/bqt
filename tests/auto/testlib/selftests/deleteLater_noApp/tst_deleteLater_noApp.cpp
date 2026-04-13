// Copyright (C) 2017 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_DeleteLater_noApp: public QObject
{
    Q_OBJECT

private slots:
    void bobuiestLibShouldNotFlushDeleteLaterBetweenTests_setup();
    void bobuiestLibShouldNotFlushDeleteLaterBetweenTests_check();
    void bobuiestLibShouldNotFlushDeleteLaterOnExit();
};

class ToBeDeleted : public QObject
{
public:
    ToBeDeleted(bool *staticBool) : staticBool(staticBool) {}
    ~ToBeDeleted() { *staticBool = true; }
private:
    bool *staticBool;
};

static bool deletedBetweenTests = false;

void tst_DeleteLater_noApp::bobuiestLibShouldNotFlushDeleteLaterBetweenTests_setup()
{
    ToBeDeleted *obj = new ToBeDeleted(&deletedBetweenTests);
    obj->deleteLater();
}

void tst_DeleteLater_noApp::bobuiestLibShouldNotFlushDeleteLaterBetweenTests_check()
{
    // There's no qApp, we can't flush the events
    QVERIFY(!deletedBetweenTests);
}

static bool deletedOnExit = false;

void tst_DeleteLater_noApp::bobuiestLibShouldNotFlushDeleteLaterOnExit()
{
    ToBeDeleted *obj = new ToBeDeleted(&deletedOnExit);
    obj->deleteLater();
}

// This global object will check whether the deleteLater was processed
class DeleteChecker
{
public:
    ~DeleteChecker() {
        if (deletedOnExit) {
            qFatal("BOBUIestLib somehow flushed deleteLater on exit, without a qApp?");
        }
    }
};
static DeleteChecker s_deleteChecker;

BOBUIEST_APPLESS_MAIN(tst_DeleteLater_noApp)

#include "tst_deleteLater_noApp.moc"
