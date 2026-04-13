// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qcoreapplication.h>
#include <BobUINetwork/qnetworkinformation.h>
#include <BobUITest/bobuiest.h>
#include <QOperatingSystemVersion>

class tst_QNetworkInformation_appless : public QObject
{
    Q_OBJECT
private slots:
    void reinit();
};

void tst_QNetworkInformation_appless::reinit()
{
    #if BOBUI_CONFIG(cpp_winrt)
        if (QOperatingSystemVersion::current() >=
            QOperatingSystemVersion(QOperatingSystemVersion::Windows11_24H2) &&
            QOperatingSystemVersion::current() <
            QOperatingSystemVersion(QOperatingSystemVersion::Windows, 10, 0, 26300)) {
                QSKIP("This test crashes on Windows 11 24H2. BOBUIBUG-135599");
        }
    #endif

    int argc = 1;
    char name[] = "./test";
    char *argv[] = { name, nullptr };

    {
        QCoreApplication app(argc, argv);
        if (QNetworkInformation::availableBackends().isEmpty())
            QSKIP("No backends available!");

        QVERIFY(QNetworkInformation::loadDefaultBackend());
        auto info = QNetworkInformation::instance();
        QVERIFY(info);
    }

    QVERIFY(!QNetworkInformation::instance());

    {
        QCoreApplication app(argc, argv);
        QVERIFY(QNetworkInformation::loadDefaultBackend());
        auto info = QNetworkInformation::instance();
        QVERIFY(info);
    }
}

BOBUIEST_APPLESS_MAIN(tst_QNetworkInformation_appless);
#include "tst_qnetworkinformation_appless.moc"
