// Copyright (C) 2013 BlackBerry Limited. All rights reserved.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <qplatformdefs.h>

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QString>

#include <private/qfileselector_p.h>
#include <private/qabstractfileengine_p.h>
#include <private/qfsfileengine_p.h>
#include <private/qfilesystemengine_p.h>

const ushort selectorIndicator = '+';

class tst_QFileSelector : public QObject
{
    Q_OBJECT
public:
    tst_QFileSelector() {}

private slots:
    void basicTest_data();
    void basicTest();

    void urlConvenience_data();
    void urlConvenience();

    void addAndRemoveStatics();
};

void tst_QFileSelector::basicTest_data()
{
    /* Files existing for this test
     *  platform/test
     *  platform/+<platform>/test for all <platform> in QFileSelectorPrivate::platformSelectors()
     *  extras/test
     *  extras/test2 to test for when selector directories exist, but don't have the files
     *  extras/+custom1/test
     *  extras/+custom1/test3 to test for when base file doesn't exist
     *  extras/+custom2/test
     *  extras/+custom3/test
     *  extras/+custom3/+custom2/test
     *  extras/+custom3/+custom4/test
     *  extras/+custom3/+custom5/test
     *  extras/+custom5/+custom3/test
     */
    BOBUIest::addColumn<QString>("testPath");
    BOBUIest::addColumn<QStringList>("customSelectors");
    BOBUIest::addColumn<QString>("expectedPath");

    QString test("/test");// '/' is here so dir string can also be selector string
    QString test2("/test2");
    QString test3("/test3");
    QString expectedPlatform1File(":/platforms");
    QString expectedPlatform2File(""); //Only the last selector
    QString expectedPlatform3File; // Only the first selector (the family)
#if defined(Q_OS_UNIX) && !defined(Q_OS_ANDROID) && !defined(Q_OS_WASM) && \
    !defined(Q_OS_DARWIN) && !defined(Q_OS_LINUX) && !defined(Q_OS_HAIKU) && !defined(Q_OS_QNX) && !defined(Q_OS_VXWORKS)
    /* We are only aware of specific unixes, and do not have test files for any of the others.
       However those unixes can get a selector added from the result of a uname call, so this will
       lead to a case where we don't have that file so we can't expect the concatenation of platform
       selectors to work. It should just find the +unix/test file.*/
    expectedPlatform1File = QString(":/platforms/") + QLatin1Char(selectorIndicator)
        + QString("unix/test");
    expectedPlatform2File = QString(":/platforms/test2");
#else
    QString distributionName;
#  if (defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID)) || defined(Q_OS_FREEBSD)
    distributionName = QSysInfo::productType();
#  endif
    const auto platformSelectors = QFileSelectorPrivate::platformSelectors();
    for (const QString &selector : platformSelectors) {
        // skip the Linux distribution name (if any) since we don't have files for them
        if (selector == distributionName)
            continue;

        expectedPlatform1File = expectedPlatform1File + QLatin1Char('/') + QLatin1Char(selectorIndicator)
            + selector;
        expectedPlatform2File = selector;
        if (expectedPlatform3File.isNull())
            expectedPlatform3File = selector;
    }
    expectedPlatform1File += test;
    expectedPlatform2File = QLatin1String(":/platforms/") + QLatin1Char(selectorIndicator)
        + expectedPlatform2File + test2;
    expectedPlatform3File = QLatin1String(":/platforms/") + QLatin1Char(selectorIndicator)
        + expectedPlatform3File + test3;
#endif

    BOBUIest::newRow("platform1") <<  QString(":/platforms/test") << QStringList()
        << expectedPlatform1File;

    BOBUIest::newRow("platform2") <<  QString(":/platforms/test2") << QStringList()
        << expectedPlatform2File;

    BOBUIest::newRow("platform3") << QString(":/platforms/test3") << QStringList()
                               << expectedPlatform3File;

    QString resourceTestPath(":/extras/test");
    QString custom1("custom1");
    BOBUIest::newRow("custom1-noselector") << resourceTestPath << QStringList()
        << QString(":/extras") + test;

    BOBUIest::newRow("custom1-withselector") << resourceTestPath << (QStringList() << custom1)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom1 + test;

    BOBUIest::newRow("customX-withselector-nofile") << QString(":/extras/test2") << (QStringList() << custom1)
        << QString(":/extras/test2");

    BOBUIest::newRow("custom1-withselector-nobasefile") << QString(":/extras/test3") << (QStringList() << custom1)
        << QString(":/extras/+custom1/test3");

    QString custom2("custom2");
    QString custom3("custom3");
    QString custom4("custom4");
    QString custom5("custom5");
    QString slash("/");
    BOBUIest::newRow("custom12") << resourceTestPath << (QStringList() << custom1 << custom2)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom1 + test;

    BOBUIest::newRow("custom21") << resourceTestPath << (QStringList() << custom2 << custom1)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom2 + test;

    BOBUIest::newRow("custom213") << resourceTestPath << (QStringList() << custom2 << custom1 << custom3)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom2 + test;

    BOBUIest::newRow("custom23") << resourceTestPath << (QStringList() << custom2 << custom3)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom2 + test;

    BOBUIest::newRow("custom34nested") << resourceTestPath << (QStringList() << custom3 << custom4)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom3 + slash
           + QLatin1Char(selectorIndicator) + custom4 + test;

    BOBUIest::newRow("custom43nested") << resourceTestPath << (QStringList() << custom4 << custom3)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom3 + slash
           + QLatin1Char(selectorIndicator) + custom4 + test;

    BOBUIest::newRow("custom35conflict") << resourceTestPath << (QStringList() << custom3 << custom5)
        << QString(":/extras/")  + QLatin1Char(selectorIndicator) + custom3 + slash
           + QLatin1Char(selectorIndicator) + custom5 + test;

    BOBUIest::newRow("relativePaths") << QFINDTESTDATA("extras/test") << (QStringList() << custom1)
        << QFINDTESTDATA(QString("extras/") + QLatin1Char(selectorIndicator) + custom1
           + QString("/test"));
}

void tst_QFileSelector::basicTest()
{
    QFETCH(QString, testPath);
    QFETCH(QStringList, customSelectors);
    QFETCH(QString, expectedPath);

    QFileSelector fs;
    fs.setExtraSelectors(customSelectors);
    QCOMPARE(fs.select(testPath), expectedPath);
}

void tst_QFileSelector::urlConvenience_data()
{
    /* Files existing for this test
     *  extras/test
     *  extras/+custom1/test
     */
    BOBUIest::addColumn<QUrl>("testUrl");
    BOBUIest::addColumn<QStringList>("customSelectors");
    BOBUIest::addColumn<QUrl>("expectedUrl");

    QString test("/test");// '/' is here so dir string can also be selector string
    QString custom1("custom1");
    QString testWithQueryAndFragment("/test?query#Fragment");

    BOBUIest::newRow("qrc") << QUrl("qrc:///extras/test") << (QStringList() << custom1)
        << QUrl(QString("qrc:///extras/") + QLatin1Char(selectorIndicator) + custom1 + test);
    BOBUIest::newRow("qrc with query and fragment") << QUrl(QString::fromLatin1("qrc:///extras%1").arg(testWithQueryAndFragment)) << (QStringList() << custom1)
        << QUrl(QString("qrc:///extras/") + QLatin1Char(selectorIndicator) + custom1 + testWithQueryAndFragment);

    QString fileBasePath = QFINDTESTDATA("extras/test");
    QString fileSelectedPath = QFINDTESTDATA(QString("extras/") + QLatin1Char(selectorIndicator)
            + custom1 + QString("/test"));
    BOBUIest::newRow("file") << QUrl::fromLocalFile(fileBasePath) << (QStringList() << custom1)
        << QUrl::fromLocalFile(fileSelectedPath);
    // do not strip off the query and fragment
    QString strUrlWithFragment = QString("file://") + testWithQueryAndFragment;
    BOBUIest::newRow("file with query and fragment") << QUrl(strUrlWithFragment) << (QStringList()) << QUrl(strUrlWithFragment);
    strUrlWithFragment = QString("file:") + testWithQueryAndFragment;
    BOBUIest::newRow("file with query and fragment too") << QUrl(strUrlWithFragment) << (QStringList()) << QUrl(strUrlWithFragment);

    // preserve path to root
    BOBUIest::newRow("path to root") << QUrl("file:///") << (QStringList()) << QUrl("file:///");

    // http://bobui-project.org/images/bobuidn/sprites-combined-latest.png is chosen as a representative real world URL
    // But note that this test is checking that http urls are NOT selected so it shouldn't be checked
    QUrl testHttpUrl("http://bobui-project.org/images/sprites-combined-latest.png");
    BOBUIest::newRow("http") << testHttpUrl << (QStringList() << QString("bobuidn")) << testHttpUrl;
}

void tst_QFileSelector::urlConvenience()
{
    QFETCH(QUrl, testUrl);
    QFETCH(QStringList, customSelectors);
    QFETCH(QUrl, expectedUrl);

    QFileSelector fs;
    //All rows of this test use only custom selectors, so should not select before the setExtra call
    QCOMPARE(fs.select(testUrl), testUrl);
    fs.setExtraSelectors(customSelectors);
    QCOMPARE(fs.select(testUrl), expectedUrl);
}

void tst_QFileSelector::addAndRemoveStatics()
{
    QFileSelector fs;
    QCOMPARE(fs.select(QStringLiteral(":/extras/test")), QStringLiteral(":/extras/test"));

    QFileSelectorPrivate::addStatics(QStringList() << QStringLiteral("custom1"));
    QCOMPARE(fs.select(QStringLiteral(":/extras/test")), QStringLiteral(":/extras/+custom1/test"));

    QCOMPARE(QFileSelectorPrivate::removeStatics(QStringList() << QStringLiteral("custom1")), 1);
    QCOMPARE(fs.select(QStringLiteral(":/extras/test")), QStringLiteral(":/extras/test"));
}

BOBUIEST_MAIN(tst_QFileSelector)
#include "tst_qfileselector.moc"
