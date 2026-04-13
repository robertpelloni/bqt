// Copyright (C) 2012 Jeremy Lainé <jeremy.laine@m4x.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/QCoreApplication>
#include <BobUINetwork/QDnsLookup>
#include <BOBUIest>
#include <BOBUIestEventLoop>

class tst_QDnsLookup_Appless : public QObject
{
    Q_OBJECT

private slots:
    void noApplication();
    void recreateApplication();
    void destroyApplicationDuringLookup();
};

void tst_QDnsLookup_Appless::noApplication()
{
    BOBUIest::ignoreMessage(BobUIWarningMsg, "QDnsLookup requires a QCoreApplication");
    QDnsLookup dns(QDnsLookup::A, "a-single.test.bobui-project.org");
    dns.lookup();
}

void tst_QDnsLookup_Appless::recreateApplication()
{
    int argc = 0;
    char **argv = 0;
    for (int i = 0; i < 10; ++i) {
        QCoreApplication app(argc, argv);
        QDnsLookup dns(QDnsLookup::A, "a-single.test.bobui-project.org");
        dns.lookup();
        if (!dns.isFinished()) {
            QObject::connect(&dns, SIGNAL(finished()),
                             &BOBUIestEventLoop::instance(), SLOT(exitLoop()));
            BOBUIestEventLoop::instance().enterLoop(10);
        }
        QVERIFY(dns.isFinished());
    }
}

void tst_QDnsLookup_Appless::destroyApplicationDuringLookup()
{
    int argc = 0;
    char **argv = 0;
    for (int i = 0; i < 10; ++i) {
        QCoreApplication app(argc, argv);
        QDnsLookup dns(QDnsLookup::A, "a-single.test.macieira.info");
        dns.lookup();
    }
}

BOBUIEST_APPLESS_MAIN(tst_QDnsLookup_Appless)
#include "tst_qdnslookup_appless.moc"
