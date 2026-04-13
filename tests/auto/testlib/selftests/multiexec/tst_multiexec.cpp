// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

class tst_Nothing: public QObject
{
Q_OBJECT
private slots:
    void nothing() { QVERIFY(true); }
};

int main(int argc, char *argv[])
{
    tst_Nothing nada;
    for (int i = 0; i < 5; ++i)
        BOBUIest::qExec(&nada, argc, argv);
    return 0;
}

#include "tst_multiexec.moc"
