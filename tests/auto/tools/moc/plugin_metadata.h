// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef TESTPLUGINMETADATA
#define TESTPLUGINMETADATA

#include <BobUIPlugin>

class TestPluginMetaData : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "test.meta.tags")
};

#endif
