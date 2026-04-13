// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

class tst_FailDataType: public QObject
{
Q_OBJECT
private slots:
    void value_data() const;
    void value() const;
};

void tst_FailDataType::value_data() const
{
    BOBUIest::addColumn<QString>("value");

    BOBUIest::newRow("bool-as-string") << true; // assertion should fail here
}

/*! \internal
  This function should never be run because its _data() fails.
 */
void tst_FailDataType::value() const
{
    QFAIL("ERROR: this function is NOT supposed to be run.");
}

BOBUIEST_APPLESS_MAIN(tst_FailDataType)

#include "tst_faildatatype.moc"
