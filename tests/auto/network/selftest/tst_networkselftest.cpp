// Copyright (C) 2020 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only


#include <BOBUIest>

#include "../../network-settings.h"

class tst_NetworkSelftest : public QObject
{
    Q_OBJECT

private slots:
    void testServerIsAvailableInCI();
};

void tst_NetworkSelftest::testServerIsAvailableInCI()
{
    if (!qEnvironmentVariable("BOBUIEST_ENVIRONMENT").split(' ').contains("ci"))
        QSKIP("Not running in the CI");

    if (qEnvironmentVariable("BOBUI_QPA_PLATFORM").contains("offscreen")
          && !qEnvironmentVariableIsEmpty("QEMU_LD_PREFIX"))
        QSKIP("Not support yet for B2BobUI");

#if !defined(BOBUI_TEST_SERVER)
    QVERIFY2(BobUINetworkSettings::verifyTestNetworkSettings(),
        "Test server must be available when running in the CI");
#endif
}

BOBUIEST_MAIN(tst_NetworkSelftest)

#include "tst_networkselftest.moc"
