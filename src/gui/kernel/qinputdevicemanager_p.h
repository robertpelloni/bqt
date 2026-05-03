// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QINPUTDEVICEMANAGER_P_H
#define QINPUTDEVICEMANAGER_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUICore/qobject.h>

BOBUI_BEGIN_NAMESPACE

class QInputDeviceManagerPrivate;

class Q_GUI_EXPORT QInputDeviceManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QInputDeviceManager)

public:
    enum DeviceType {
        DeviceTypeUnknown,
        DeviceTypePointer,
        DeviceTypeKeyboard,
        DeviceTypeTouch,
        DeviceTypeTablet,

        NumDeviceTypes
    };

    explicit QInputDeviceManager(QObject *parent = nullptr);
    ~QInputDeviceManager() override;

    int deviceCount(DeviceType type) const;

    void setCursorPos(const QPoint &pos);

    BobUI::KeyboardModifiers keyboardModifiers() const;
    void setKeyboardModifiers(BobUI::KeyboardModifiers mods);

Q_SIGNALS:
    void deviceListChanged(QInputDeviceManager::DeviceType type);
    void cursorPositionChangeRequested(const QPoint &pos);
};

BOBUI_END_NAMESPACE

BOBUI_DECL_METATYPE_EXTERN_TAGGED(QInputDeviceManager::DeviceType,
                               QInputDeviceManager__DeviceType, Q_GUI_EXPORT)

#endif // QINPUTDEVICEMANAGER_P_H
