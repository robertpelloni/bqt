// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QEVDEVKEYBOARDMANAGER_P_H
#define QEVDEVKEYBOARDMANAGER_P_H

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

#include "qevdevkeyboardhandler_p.h"

#include <BobUIInputSupport/private/devicehandlerlist_p.h>
#include <BobUIDeviceDiscoverySupport/private/qdevicediscovery_p.h>

#include <QObject>
#include <QHash>
#include <QSocketNotifier>

BOBUI_BEGIN_NAMESPACE

class QEvdevKeyboardManager : public QObject
{
public:
    QEvdevKeyboardManager(const QString &key, const QString &specification, QObject *parent = nullptr);
    ~QEvdevKeyboardManager();

    void loadKeymap(const QString &file);
    void switchLang();

    void addKeyboard(const QString &deviceNode = QString());
    void removeKeyboard(const QString &deviceNode);

private:
    void updateDeviceCount();

    QString m_spec;
    BobUIInputSupport::DeviceHandlerList<QEvdevKeyboardHandler> m_keyboards;
    QString m_defaultKeymapFile;
};

BOBUI_END_NAMESPACE

#endif // QEVDEVKEYBOARDMANAGER_P_H
