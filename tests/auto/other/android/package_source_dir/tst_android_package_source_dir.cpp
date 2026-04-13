// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <QCoreApplication>
#include <QJniObject>
#include <BOBUIest>
#include <QDebug>

using namespace QNativeInterface;

Q_DECLARE_JNI_CLASS(ApplicationInfo, "android/content/pm/ApplicationInfo")
Q_DECLARE_JNI_CLASS(PackageManager, "android/content/pm/PackageManager")
Q_DECLARE_JNI_CLASS(CharSequence, "java/lang/CharSequence")

class tst_android_package_source_dir : public QObject
{
    Q_OBJECT

private slots:
    void applicationName();

private:
};

void tst_android_package_source_dir::applicationName()
{
    QJniObject appCtx = QAndroidApplication::context();
    QVERIFY(appCtx.isValid());

    const auto appInfo = appCtx.callMethod<BobUIJniTypes::ApplicationInfo>("getApplicationInfo");
    QVERIFY(appInfo.isValid());

    const auto packageManager = appCtx.callMethod<BobUIJniTypes::PackageManager>("getPackageManager");
    QVERIFY(packageManager.isValid());

    const auto appNameLabel =
            appInfo.callMethod<BobUIJniTypes::CharSequence>("loadLabel", packageManager);
    QVERIFY(appNameLabel.isValid());

    const auto appName = appNameLabel.callMethod<jstring>("toString").toString();

    QCOMPARE_EQ(appName, QString::fromLatin1(EXPECTED_APP_NAME));
}

BOBUIEST_MAIN(tst_android_package_source_dir);

#include "tst_android_package_source_dir.moc"
