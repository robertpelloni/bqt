// Copyright (C) 2021 zccrs <zccrs@live.com>, JiDe Zhang <zhangjide@uniontech.com>.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BOBUIest>
#include <QGuiApplication>

class tst_qAddPreRoutine : public QObject
{
    Q_OBJECT

public:
    static void initMain()
    {
        // The purpose of this use case is indeed to test "qAddPreRoutine", but
        // as you can see, there is nowhere to call "qAddPreRoutine". Please see
        // the following two lines of code, which set the "BOBUI_QPA_PLATFORM_PLUGIN_PATH"
        // and "BOBUI_QPA_PLATFORMTHEME" environment variables that a new platform
        // theme plugin will be loaded, and the Q_COREAPP_STARTUP_FUNCTION macro
        // is used in this plugin, which will cause "qAddPreRoutine" to be called
        // indirectly in the Q*Application class when load the platform theme plugin.
        // See the "plugin.cpp" file.
#ifndef Q_OS_ANDROID // The plug-in is in the apk package, no need to specify its directory
        qputenv("BOBUI_QPA_PLATFORM_PLUGIN_PATH", BOBUI_QPA_PLATFORM_PLUGIN_PATH);
#endif
        qputenv("BOBUI_QPA_PLATFORMTHEME", "BOBUIBUG_90341");
    }

private slots:
    void tst_BOBUIBUG_90341()
    {
#ifdef Q_OS_ANDROID
        QSKIP("Android can't load the platform theme plugin this test needs, see BOBUIBUG-92893");
#endif
        QVERIFY2(qEnvironmentVariableIsSet("BOBUIBUG_90341_ThemePlugin"),
                 "The \"BOBUIBUG_90341\" theme plugin not loaded.");
        // This "test_function_call_count" property is assigned in the "BOBUIBUG_90341" plugin.
        // See the "plugin.cpp" file.
        QCOMPARE(qApp->property("test_function_call_count").toInt(), 1);
    }
};

BOBUIEST_MAIN(tst_qAddPreRoutine)

#include "tst_qaddpreroutine.moc"
