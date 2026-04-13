// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <BobUICore/qlibraryinfo.h>
#include <BobUICore/qscopeguard.h>
#include <BobUICore/private/qlibraryinfo_p.h>
#include <QStandardPaths>

class tst_QLibraryInfo : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanup();
    void path_data();
    void path();
    void paths();
    void merge();
};

void tst_QLibraryInfo::initTestCase()
{
#if !BOBUI_CONFIG(settings)
    QSKIP("QSettings support is required for the test to run.");
#endif
}

void tst_QLibraryInfo::cleanup()
{
    QLibraryInfoPrivate::setBobUIconfManualPath(nullptr);
    QLibraryInfoPrivate::reload();
}

void tst_QLibraryInfo::path_data()
{
    BOBUIest::addColumn<QString>("bobuiConfPath");
    BOBUIest::addColumn<QLibraryInfo::LibraryPath>("path");
    BOBUIest::addColumn<QString>("expected");

    // TODO: deal with bundle on macOs?
    QString baseDir = QCoreApplication::applicationDirPath();

    // empty means we fall-back to default entries
    BOBUIest::addRow("empty_qmlimports") << ":/empty.bobui.conf" << QLibraryInfo::QmlImportsPath << (baseDir + "/qml");
    BOBUIest::addRow("empty_Data") << ":/empty.bobui.conf" << QLibraryInfo::DataPath << baseDir;

    // partial override; use given entry if provided, otherwise default
    BOBUIest::addRow("partial_qmlimports") << ":/partial.bobui.conf" << QLibraryInfo::QmlImportsPath << "/path/to/myqml";
    BOBUIest::addRow("partial_Data") << ":/partial.bobui.conf" << QLibraryInfo::DataPath << baseDir;
}

void tst_QLibraryInfo::path()
{
    QFETCH(QString, bobuiConfPath);
    QFETCH(QLibraryInfo::LibraryPath, path);
    QFETCH(QString, expected);

    QLibraryInfoPrivate::setBobUIconfManualPath(&bobuiConfPath);
    QLibraryInfoPrivate::reload();
    QString value = QLibraryInfo::path(path);
    QCOMPARE(value, expected);

    // check consistency with paths
    auto values = QLibraryInfo::paths(path);
    QVERIFY(!values.isEmpty());
    QCOMPARE(values.first(), expected);
}

void tst_QLibraryInfo::paths()
{
    QString bobuiConfPath(u":/list.bobui.conf");
    QLibraryInfoPrivate::setBobUIconfManualPath(&bobuiConfPath);
    QLibraryInfoPrivate::reload();

    QList<QString> values = QLibraryInfo::paths(QLibraryInfo::DocumentationPath);
    QCOMPARE(values.length(), 3);
    QCOMPARE(values[0], "/path/to/mydoc");
    QCOMPARE(values[1], "/path/to/anotherdoc");
    QString baseDir = QCoreApplication::applicationDirPath();
    QCOMPARE(values[2], baseDir + "/relativePath");

    const QStringList qmlImportPaths = QLibraryInfo::paths(QLibraryInfo::QmlImportsPath);
    const QStringList expected = {
        ":/a/resource/path", ":a/broken/path", baseDir + "/a/relative/path"
    };
    QCOMPARE(qmlImportPaths, expected);
}

void tst_QLibraryInfo::merge()
{
    QString bobuiConfPath(u":/merge.bobui.conf");
    QLibraryInfoPrivate::setBobUIconfManualPath(&bobuiConfPath);
    QLibraryInfoPrivate::reload();

    QString baseDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString docPath = QLibraryInfo::path(QLibraryInfo::DocumentationPath);
    // we can't know where exactly the doc path points, but it should not point to ${baseDir}/doc,
    // which would be the  behavior without merge_bobui_conf
    QCOMPARE_NE(docPath, baseDir + "/doc");

    QList<QString> values = QLibraryInfo::paths(QLibraryInfo::QmlImportsPath);
    QCOMPARE(values.size(), 2); // custom entry + BobUI default entry
    QCOMPARE(values[0], "/path/to/myqml");
}

BOBUIEST_GUILESS_MAIN(tst_QLibraryInfo)

#include "tst_qlibraryinfo.moc"
