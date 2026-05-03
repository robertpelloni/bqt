// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only
#include <BobUIPlugin>
#include <QObject>

class ReleasePlugin : public QObject
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "SomeIID")
public:
    ReleasePlugin() {}
};

#include "main.moc"
