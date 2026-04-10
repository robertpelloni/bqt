// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUICore/qfile.h>
#include <BobUICore/qdebug.h>

BOBUI_BEGIN_NAMESPACE

class TestStaticPlugin : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "TestStaticPlugin" URI "bobui.teststaticplugin")
public:
    TestStaticPlugin() = default;
    Q_INVOKABLE bool checkResources()
    {
        return QFile::exists(":/teststaticplugin1/testfile1.txt")
                && QFile::exists(":/teststaticplugin2/testfile2.txt");
    }
};

BOBUI_END_NAMESPACE

#include "pluginmain.moc"
