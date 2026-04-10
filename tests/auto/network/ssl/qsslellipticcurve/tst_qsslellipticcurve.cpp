// Copyright (C) 2014 Governikus GmbH & Co. KG.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>

#include <BobUINetwork/bobuinetworkglobal.h>

#if BOBUI_CONFIG(ssl)
#include <QSslSocket>
#endif // ssl

#include <QSslEllipticCurve>
#include <QSslConfiguration>

class tst_QSslEllipticCurve : public QObject
{
    Q_OBJECT

#if BOBUI_CONFIG(ssl)
private Q_SLOTS:
    void initTestCase();
    void constExpr();
    void construction();
    void fromShortName_data();
    void fromShortName();
    void fromLongName_data();
    void fromLongName();
#endif // Feature 'ssl'.
};

#if BOBUI_CONFIG(ssl)

void tst_QSslEllipticCurve::initTestCase()
{
    // At the moment only OpenSSL backend properly supports
    // QSslEllipticCurve.
    if (QSslSocket::activeBackend() != QStringLiteral("openssl"))
        QSKIP("The active TLS backend does not support QSslEllipticCurve");
}

void tst_QSslEllipticCurve::constExpr()
{
    // check that default ctor and op ==/!= are constexpr:
    char array1[QSslEllipticCurve() == QSslEllipticCurve() ?  1 : -1];
    char array2[QSslEllipticCurve() != QSslEllipticCurve() ? -1 :  1];
    Q_UNUSED(array1);
    Q_UNUSED(array2);
}

void tst_QSslEllipticCurve::construction()
{
    QSslEllipticCurve curve;
    QCOMPARE(curve.isValid(), false);
    QCOMPARE(curve.shortName(), QString());
    QCOMPARE(curve.longName(), QString());
    QCOMPARE(curve.isTlsNamedCurve(), false);
}

void tst_QSslEllipticCurve::fromShortName_data()
{
    BOBUIest::addColumn<QString>("shortName");
    BOBUIest::addColumn<QSslEllipticCurve>("curve");
    BOBUIest::addColumn<bool>("valid");

    BOBUIest::newRow("QString()") << QString() << QSslEllipticCurve() << false;
    BOBUIest::newRow("\"\"") << QString("") << QSslEllipticCurve() << false;
    BOBUIest::newRow("does-not-exist") << QStringLiteral("does-not-exist") << QSslEllipticCurve() << false;
    const auto supported = QSslConfiguration::supportedEllipticCurves();
    for (QSslEllipticCurve ec : supported) {
        const QString sN = ec.shortName();
        BOBUIest::newRow(qPrintable("supported EC \"" + sN + '"')) << sN << ec << true;
        // At least in the OpenSSL impl, the short name is case-sensitive. That feels odd.
        //const QString SN = sN.toUpper();
        //BOBUIest::newRow(qPrintable("supported EC \"" + SN + '"')) << SN << ec << true;
        //const QString sn = sN.toLower();
        //BOBUIest::newRow(qPrintable("supported EC \"" + sn + '"')) << sn << ec << true;
    }
}

void tst_QSslEllipticCurve::fromShortName()
{
    QFETCH(QString, shortName);
    QFETCH(QSslEllipticCurve, curve);
    QFETCH(bool, valid);

    const QSslEllipticCurve result = QSslEllipticCurve::fromShortName(shortName);
    QCOMPARE(result, curve);
    QCOMPARE(result.isValid(), valid);
    QCOMPARE(result.shortName(), curve.shortName());
    QCOMPARE(result.shortName(), valid ? shortName : QString());
}

void tst_QSslEllipticCurve::fromLongName_data()
{
    BOBUIest::addColumn<QString>("longName");
    BOBUIest::addColumn<QSslEllipticCurve>("curve");
    BOBUIest::addColumn<bool>("valid");

    BOBUIest::newRow("QString()") << QString() << QSslEllipticCurve() << false;
    BOBUIest::newRow("\"\"") << QString("") << QSslEllipticCurve() << false;
    BOBUIest::newRow("does-not-exist") << QStringLiteral("does-not-exist") << QSslEllipticCurve() << false;
    const auto supported = QSslConfiguration::supportedEllipticCurves();
    for (QSslEllipticCurve ec : supported) {
        const QString lN = ec.longName();
        BOBUIest::newRow(qPrintable("supported EC \"" + lN + '"')) << lN << ec << true;
    }
}

void tst_QSslEllipticCurve::fromLongName()
{
    QFETCH(QString, longName);
    QFETCH(QSslEllipticCurve, curve);
    QFETCH(bool, valid);

    const QSslEllipticCurve result = QSslEllipticCurve::fromLongName(longName);
    QCOMPARE(result, curve);
    QCOMPARE(result.isValid(), valid);
    QCOMPARE(result.longName(), curve.longName());
    QCOMPARE(result.longName(), valid ? longName : QString());
}

#endif // Feature 'ssl'.

BOBUIEST_MAIN(tst_QSslEllipticCurve)
#include "tst_qsslellipticcurve.moc"
