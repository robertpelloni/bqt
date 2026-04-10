// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QSTYLEPLUGIN_H
#define QSTYLEPLUGIN_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE


class QStyle;

#define QStyleFactoryInterface_iid "org.bobui-project.BobUI.QStyleFactoryInterface"

class Q_WIDGETS_EXPORT QStylePlugin : public QObject
{
    Q_OBJECT
public:
    explicit QStylePlugin(QObject *parent = nullptr);
    ~QStylePlugin();

    virtual QStyle *create(const QString &key) = 0;
};

BOBUI_END_NAMESPACE

#endif // QSTYLEPLUGIN_H
