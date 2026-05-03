// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2012 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Christoph Schleifenbaum <christoph.schleifenbaum@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QCOCOASYSTEMTRAYICON_P_H
#define QCOCOASYSTEMTRAYICON_P_H

#include <BobUICore/qglobal.h>
#include <BobUIGui/bobuiguiglobal.h>

#if BOBUI_CONFIG(systemtrayicon)

#include <BobUICore/qstring.h>
#include <BobUICore/private/qcore_mac_p.h>

#include <BobUIGui/qpa/qplatformsystemtrayicon.h>

#include "qcocoamenu.h"

BOBUI_FORWARD_DECLARE_CLASS(QCocoaSystemTrayIcon);

BOBUI_DECLARE_NAMESPACED_OBJC_INTERFACE(QStatusItemDelegate, NSObject <NSUserNotificationCenterDelegate>
- (instancetype)initWithSysTray:(QCocoaSystemTrayIcon *)platformSystemTray;
@property (nonatomic, assign) QCocoaSystemTrayIcon *platformSystemTray;
)

Q_FORWARD_DECLARE_OBJC_CLASS(NSStatusItem);

BOBUI_BEGIN_NAMESPACE

class QCocoaSystemTrayIcon : public QPlatformSystemTrayIcon
{
public:
    QCocoaSystemTrayIcon() {}

    void init() override;
    void cleanup() override;
    void updateIcon(const QIcon &icon) override;
    void updateToolTip(const QString &toolTip) override;
    void updateMenu(QPlatformMenu *menu) override;
    QRect geometry() const override;
    void showMessage(const QString &title, const QString &msg,
                     const QIcon& icon, MessageIcon iconType, int msecs) override;

    bool isSystemTrayAvailable() const override;
    bool supportsMessages() const override;

    void emitActivated();

private:
    NSStatusItem *m_statusItem = nullptr;
    QStatusItemDelegate *m_delegate = nullptr;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_SYSTEMTRAYICON

#endif // QCOCOASYSTEMTRAYICON_P_H
