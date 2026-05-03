// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

class tst_TestA : public QObject
{
    Q_OBJECT

private slots:
    void slotName() const
    {
        QVERIFY(true);
    }

    void aDifferentSlot() const
    {
        QVERIFY(false);
    }
};

class tst_TestB : public QObject
{
    Q_OBJECT

private slots:
    void slotName() const
    {
        QVERIFY(true);
    }

    void aSecondDifferentSlot() const
    {
        QVERIFY(false);
    }
};

int main()
{
    char *argv[] = { const_cast<char *>("appName"), const_cast<char *>("slotName") };
    int argc = 2;

    tst_TestA testA;
    BOBUIest::qExec(&testA, argc, argv);
    BOBUIest::qExec(&testA, argc, argv);

    tst_TestB testB;
    BOBUIest::qExec(&testB, argc, argv);

    return 0;
}

#include "tst_differentexec.moc"
