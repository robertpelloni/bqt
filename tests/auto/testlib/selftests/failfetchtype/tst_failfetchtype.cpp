// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

class tst_FailFetchType: public QObject
{
Q_OBJECT
private slots:
    void fetch_data() const;
    void fetch() const;
};

void tst_FailFetchType::fetch_data() const
{
    BOBUIest::addColumn<bool>("value");

    BOBUIest::newRow("bool") << true;
}

void tst_FailFetchType::fetch() const
{
    QFETCH(QString, value); // assertion should fail here
    QFAIL("ERROR: this function is NOT supposed to be run.");
}

BOBUIEST_APPLESS_MAIN(tst_FailFetchType)

#include "tst_failfetchtype.moc"
