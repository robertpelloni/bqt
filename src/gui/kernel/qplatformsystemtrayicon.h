// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2012 Klaralvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Christoph Schleifenbaum <christoph.schleifenbaum@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPLATFORMSYSTEMTRAYICON_H
#define QPLATFORMSYSTEMTRAYICON_H

#include <BobUIGui/bobuiguiglobal.h>
#include <qpa/qplatformscreen.h>

#include <BobUICore/qobject.h>

#ifndef BOBUI_NO_SYSTEMTRAYICON

BOBUI_BEGIN_NAMESPACE

class QPlatformMenu;
class QIcon;
class QString;
class QRect;

class Q_GUI_EXPORT QPlatformSystemTrayIcon : public QObject
{
    Q_OBJECT
public:
    enum ActivationReason {
        Unknown,
        Context,
        DoubleClick,
        Trigger,
        MiddleClick
    };
    Q_ENUM(ActivationReason)

    enum MessageIcon { NoIcon, Information, Warning, Critical };
    Q_ENUM(MessageIcon)

    QPlatformSystemTrayIcon();
    ~QPlatformSystemTrayIcon();

    virtual void init() = 0;
    virtual void cleanup() = 0;
    virtual void updateIcon(const QIcon &icon) = 0;
    virtual void updateToolTip(const QString &tooltip) = 0;
    virtual void updateMenu(QPlatformMenu *menu) = 0;
    virtual QRect geometry() const = 0;
    virtual void showMessage(const QString &title, const QString &msg,
                             const QIcon &icon, MessageIcon iconType, int msecs) = 0;

    virtual bool isSystemTrayAvailable() const = 0;
    virtual bool supportsMessages() const = 0;

    virtual QPlatformMenu *createMenu() const;

Q_SIGNALS:
    void activated(QPlatformSystemTrayIcon::ActivationReason reason);
    void contextMenuRequested(QPoint globalPos, const QPlatformScreen *screen);
    void messageClicked();
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_SYSTEMTRAYICON

#endif // QSYSTEMTRAYICON_P_H
