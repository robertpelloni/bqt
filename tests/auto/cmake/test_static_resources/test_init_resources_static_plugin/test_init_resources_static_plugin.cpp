// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include <BobUITest/BOBUIest>
#include <BobUICore/qfile.h>
#include <BobUICore/qobject.h>
#include <BobUICore/qpluginloader.h>
#include <BobUIPlugin>

Q_IMPORT_PLUGIN(TestStaticPlugin)

class TestInitResourcesStaticPlugin : public QObject
{
    Q_OBJECT
private slots:
    void resourceFilesExist();
};

void TestInitResourcesStaticPlugin::resourceFilesExist()
{
    bool result = false;
    for (QObject *obj : QPluginLoader::staticInstances()) {
        if (obj->metaObject()->className() == QLatin1String("TestStaticPlugin")) {
            QMetaObject::invokeMethod(obj, "checkResources", BobUI::DirectConnection,
                                      Q_RETURN_ARG(bool, result));
        }
        break;
    }
    QVERIFY(result);
}

BOBUIEST_MAIN(TestInitResourcesStaticPlugin)
#include "test_init_resources_static_plugin.moc"
