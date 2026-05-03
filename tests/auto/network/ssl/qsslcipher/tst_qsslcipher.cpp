// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>
#include <qsslcipher.h>

class tst_QSslCipher : public QObject
{
    Q_OBJECT

#ifndef BOBUI_NO_SSL

private slots:
    void constructing();

#endif // BOBUI_NO_SSL
};

#ifndef BOBUI_NO_SSL

void tst_QSslCipher::constructing()
{
    QSslCipher cipher;

    QVERIFY(cipher.isNull());
    QCOMPARE(cipher.name(), QString());
    QCOMPARE(cipher.supportedBits(), 0);
    QCOMPARE(cipher.usedBits(), 0);
    QCOMPARE(cipher.keyExchangeMethod(), QString());
    QCOMPARE(cipher.authenticationMethod(), QString());
    QCOMPARE(cipher.protocolString(), QString());
    QCOMPARE(cipher.protocol(), QSsl::UnknownProtocol);
}

#endif // BOBUI_NO_SSL

BOBUIEST_MAIN(tst_QSslCipher)
#include "tst_qsslcipher.moc"
