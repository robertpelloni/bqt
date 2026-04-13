// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QACCESSIBLEBRIDGE_H
#define QACCESSIBLEBRIDGE_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qplugin.h>
#include <BobUICore/qfactoryinterface.h>

BOBUI_BEGIN_NAMESPACE


#if BOBUI_CONFIG(accessibility)

class QAccessibleInterface;
class QAccessibleEvent;

class Q_GUI_EXPORT QAccessibleBridge
{
public:
    virtual ~QAccessibleBridge();
    virtual void setRootObject(QAccessibleInterface *) = 0;
    virtual void notifyAccessibilityUpdate(QAccessibleEvent *event) = 0;
};

#define QAccessibleBridgeFactoryInterface_iid "org.bobui-project.BobUI.QAccessibleBridgeFactoryInterface"

class Q_GUI_EXPORT QAccessibleBridgePlugin : public QObject
{
    Q_OBJECT
public:
    explicit QAccessibleBridgePlugin(QObject *parent = nullptr);
    ~QAccessibleBridgePlugin();

    virtual QAccessibleBridge *create(const QString &key) = 0;
};

#endif // BOBUI_CONFIG(accessibility)

BOBUI_END_NAMESPACE

#endif // QACCESSIBLEBRIDGE_H
