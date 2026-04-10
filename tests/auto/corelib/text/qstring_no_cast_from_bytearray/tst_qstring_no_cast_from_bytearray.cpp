// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/BobUICore>

class tst_QString_NoCastFromByteArray: public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();
};

void tst_QString_NoCastFromByteArray::initTestCase()
{
    qWarning("This is a compile test only");
}

BOBUIEST_APPLESS_MAIN(tst_QString_NoCastFromByteArray)

#include "tst_qstring_no_cast_from_bytearray.moc"
