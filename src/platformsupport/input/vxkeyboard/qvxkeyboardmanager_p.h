// Copyright (C) 2024 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QVXKEYBOARDMANAGER_P_H
#define QVXKEYBOARDMANAGER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qvxkeyboardhandler_p.h"

#include <BobUIInputSupport/private/devicehandlerlist_p.h>
#include <BobUIDeviceDiscoverySupport/private/qdevicediscovery_p.h>

#include <QObject>
#include <QHash>
#include <QSocketNotifier>

BOBUI_BEGIN_NAMESPACE

class QVxKeyboardManager : public QObject
{
public:
    QVxKeyboardManager(const QString &key, const QString &specification, QObject *parent = nullptr);
    ~QVxKeyboardManager();

    void loadKeymap(const QString &file);
    void switchLang();

    void addKeyboard(const QString &deviceNode = QString());
    void removeKeyboard(const QString &deviceNode);

private:
    void updateDeviceCount();

    QString m_spec;
    BobUIInputSupport::DeviceHandlerList<QVxKeyboardHandler> m_keyboards;
    QString m_defaultKeymapFile;
};

BOBUI_END_NAMESPACE

#endif // QVXKEYBOARDMANAGER_P_H
