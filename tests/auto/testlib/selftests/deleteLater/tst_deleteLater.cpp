// Copyright (C) 2017 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author David Faure <david.faure@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BobUICore/QCoreApplication>
#include <BOBUIest>

class tst_DeleteLater: public QObject
{
    Q_OBJECT

private slots:
    void bobuiestLibShouldFlushDeleteLaterBetweenTests_setup();
    void bobuiestLibShouldFlushDeleteLaterBetweenTests_check();
    void bobuiestLibShouldFlushDeleteLaterOnExit();
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

void tst_DeleteLater::bobuiestLibShouldFlushDeleteLaterBetweenTests_setup()
{
    ToBeDeleted *obj = new ToBeDeleted(&deletedBetweenTests);
    obj->deleteLater();
}

void tst_DeleteLater::bobuiestLibShouldFlushDeleteLaterBetweenTests_check()
{
    QVERIFY(deletedBetweenTests);
}

static bool deletedOnExit = false;

void tst_DeleteLater::bobuiestLibShouldFlushDeleteLaterOnExit()
{
    ToBeDeleted *obj = new ToBeDeleted(&deletedOnExit);
    obj->deleteLater();
}

// This global object will check whether the deleteLater was processed
class DeleteChecker
{
public:
    ~DeleteChecker() {
        if (!deletedOnExit) {
            qFatal("BOBUIestLib failed to flush deleteLater on exit");
        }
    }
};
static DeleteChecker s_deleteChecker;

BOBUIEST_MAIN(tst_DeleteLater)

#include "tst_deleteLater.moc"
