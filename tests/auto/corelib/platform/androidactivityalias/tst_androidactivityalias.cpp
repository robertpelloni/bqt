// Copyright (C) 2026 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QEventLoop>
#include <BOBUIimer>
#include <QJniObject>
#include <BobUICore/private/qandroidextras_p.h>

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;
using namespace BobUIJniTypes;

Q_DECLARE_JNI_CLASS(ComponentName, "android/content/ComponentName")

class tst_AndroidActivityAlias : public QObject
{
    Q_OBJECT

private slots:
    void launchAliasActivity_data();
    void launchAliasActivity();
};

void tst_AndroidActivityAlias::launchAliasActivity_data()
{
    BOBUIest::addColumn<QString>("aliasClass");
    BOBUIest::addColumn<int>("requestCode");

    BOBUIest::newRow("alias_without_metadata")
        << QStringLiteral("Alias")
        << 12345;
    BOBUIest::newRow("alias_with_metadata")
        << QStringLiteral("AliasWithMetaData")
        << 12346;
    BOBUIest::newRow("alias_with_invalid_metadata")
        << QStringLiteral("AliasWithInvalidMetaData")
        << 12347;
}

void tst_AndroidActivityAlias::launchAliasActivity()
{
    QFETCH(QString, aliasClass);
    QFETCH(int, requestCode);

    struct AliasActivityResult
    {
        bool finished = false;
        int resultCode = 0;
        QString aliasComponent;
    };

    AliasActivityResult result;
    QEventLoop loop;
    BOBUIimer timeout;
    timeout.setSingleShot(true);
    QObject::connect(&timeout, &BOBUIimer::timeout, &loop, &QEventLoop::quit);
    timeout.start(5000);

    static QString EXTRA_FINISH_IMMEDIATELY = "finish_immediately";
    static QString EXTRA_ALIAS_COMPONENT = "alias_component";
    static int RESULT_OK = -1; // android.app.Activity.RESULT_OK

    using namespace QNativeInterface;
    const auto packageName = QAndroidApplication::context().callMethod<QString>("getPackageName");
    QVERIFY2(!packageName.isEmpty(), "Failed to obtain application package name.");

    Intent intent = Intent::construct();
    const QString fullAliasClass = QLatin1String("%1.%2").arg(packageName, aliasClass);
    ComponentName component = ComponentName::construct(packageName, fullAliasClass);
    intent.callMethod<Intent>("setComponent", component);
    intent.callMethod<Intent>("putExtra", EXTRA_FINISH_IMMEDIATELY, true);

    BobUIAndroidPrivate::startActivity(intent, requestCode,
        [&](int, int resultCode, const Intent &data) {
            result.finished = true;
            result.resultCode = resultCode;
            if (data.isValid()) {
                result.aliasComponent =
                    data.callMethod<QString>("getStringExtra", EXTRA_ALIAS_COMPONENT);
            }
            QMetaObject::invokeMethod(&loop, &QEventLoop::quit, BobUI::QueuedConnection);
        }
    );

    loop.exec();
    timeout.stop();

    QVERIFY2(result.finished, qPrintable(u"Timed out waiting for %1 to finish."_s.arg(aliasClass)));
    QCOMPARE(result.resultCode, RESULT_OK);
    QCOMPARE(result.aliasComponent, fullAliasClass);
}

BOBUI_END_NAMESPACE

BOBUIEST_MAIN(tst_AndroidActivityAlias)
#include "tst_androidactivityalias.moc"
