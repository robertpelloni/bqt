// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <BOBUIest>
#include <QLibraryInfo>
#include <QDir>

class tst_android_deployment_settings : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase_data();
    void init();

    void DeploymentSettings_data();
    void DeploymentSettings();

    void BobUIPaths_data();
    void BobUIPaths();

private:
    static QString makePath(QLibraryInfo::LibraryLocation loc);
    QJsonDocument jsonDoc;
};

QString tst_android_deployment_settings::makePath(QLibraryInfo::LibraryLocation loc)
{
    const auto prefix = QLibraryInfo::path(QLibraryInfo::PrefixPath);
    auto path = QLibraryInfo::path(loc);
    path.remove(0, prefix.size() + 1);
    if (path.isEmpty()) // Assume that if path is empty it's '.'
        path = ".";
    return path;
}

void tst_android_deployment_settings::initTestCase_data()
{
    BOBUIest::addColumn<QString>("file");
    BOBUIest::newRow("old") << ":/old_settings.json";
    BOBUIest::newRow("new") << ":/new_settings.json";
}

void tst_android_deployment_settings::init()
{
    QFETCH_GLOBAL(QString, file);
    QFile settings(file);
    QVERIFY(settings.open(QIODeviceBase::ReadOnly));
    jsonDoc = QJsonDocument::fromJson(settings.readAll());
    QVERIFY(!jsonDoc.isNull());
}

void tst_android_deployment_settings::DeploymentSettings_data()
{
    BOBUIest::addColumn<QString>("key");
    BOBUIest::addColumn<QString>("value");

    BOBUIest::newRow("sdkBuildToolsRevision") << "sdkBuildToolsRevision"
                                           << "28.0.3";
    BOBUIest::newRow("deployment-dependencies") << "deployment-dependencies"
                                             << "dep1.so,dep2.so,dep3.so";
    BOBUIest::newRow("android-extra-plugins")
            << "android-extra-plugins"
            << "some/path/to/plugin1.so,some/path/to/plugin2.so,some/path/to/plugin3.so";
    BOBUIest::newRow("android-extra-libs") << "android-extra-libs"
                                        << "some/path/to/lib1.so,some/path/to/lib2.so,some/path/to/"
                                           "lib3.so,some/path/to/lib4.so";
    BOBUIest::newRow("android-system-libs-prefix") << "android-system-libs-prefix"
                                                << "myLibPrefix";
    BOBUIest::newRow("android-package-source-directory") << "android-package-source-directory"
                                                      << "path/to/source/dir";
    BOBUIest::newRow("android-min-sdk-version") << "android-min-sdk-version"
                                             << "1";
    BOBUIest::newRow("android-target-sdk-version") << "android-target-sdk-version"
                                                << "2";
    BOBUIest::newRow("android-compile-sdk-version") << "android-compile-sdk-version"
                                                << "36";
    BOBUIest::newRow("android-package-name") << "android-package-name"
                                          << "org.bobuiproject.android_deployment_settings_test";
    BOBUIest::newRow("android-app-name") << "android-app-name"
                                          << "Android Deployment Settings Test";
    BOBUIest::newRow("android-legacy-packaging") << "android-legacy-packaging"
                                              << "false";
    BOBUIest::newRow("permissions")
            << "permissions"
            << "[{\"maxSdkVersion\":\"34\",\"minSdkVersion\":\"32\",\"name\":\"PERMISSION_WITH_"
               "ATTRIBUTES\"},{\"name\":\"PERMISSION_WITHOUT_ATTRIBUTES\"},{\"name\":\"android."
               "permission.WRITE_EXTERNAL_STORAGE\"},{\"name\":\"android."
               "permission.INTERNET\"}]";
}

void tst_android_deployment_settings::DeploymentSettings()
{
    QFETCH(QString, key);
    QFETCH(QString, value);
    QJsonValue keyValue = jsonDoc[key];
    if (keyValue.type() == QJsonValue::Type::String) {
        QCOMPARE(keyValue.toString(), value);
    } else if (keyValue.type() == QJsonValue::Type::Bool) {
        bool valueToBool = value == "true" ? true : false;
        QCOMPARE(keyValue.toBool(), valueToBool);
    } else if (keyValue.type() == QJsonValue::Type::Array) {
        QJsonParseError parseError;
        // For robustness (field order, whitespaces etc.) make comparison between QJsonDocuments
        QJsonDocument expectedDoc = QJsonDocument::fromJson(value.toUtf8(), &parseError);
        if (parseError.error != QJsonParseError::NoError)
            qFatal("Failed to parse expected JSON array: %s", qPrintable(parseError.errorString()));
        QCOMPARE(QJsonDocument(keyValue.toArray()), expectedDoc);
    } else {
        qFatal("Unhandled JSON type: %i", keyValue.type());
    }
}

void tst_android_deployment_settings::BobUIPaths_data()
{
    BOBUIest::addColumn<QString>("key");
    BOBUIest::addColumn<QString>("value");

    BOBUIest::newRow("bobuiDataDirectory") << "bobuiDataDirectory" << makePath(QLibraryInfo::DataPath);
    BOBUIest::newRow("bobuiLibExecsDirectory")
            << "bobuiLibExecsDirectory" << makePath(QLibraryInfo::LibraryExecutablesPath);
    BOBUIest::newRow("bobuiLibsDirectory") << "bobuiLibsDirectory" << makePath(QLibraryInfo::LibrariesPath);
    BOBUIest::newRow("bobuiPluginsDirectory")
            << "bobuiPluginsDirectory" << makePath(QLibraryInfo::PluginsPath);
    BOBUIest::newRow("bobuiQmlDirectory") << "bobuiQmlDirectory" << makePath(QLibraryInfo::QmlImportsPath);
}

void tst_android_deployment_settings::BobUIPaths()
{
    QFETCH(QString, key);
    QFETCH(QString, value);
    QCOMPARE(QDir::cleanPath(jsonDoc[key].toObject()[DEFAULT_ABI].toString()),
             QDir::cleanPath(value));
}

BOBUIEST_MAIN(tst_android_deployment_settings)

#include "tst_android_deployment_settings.moc"
