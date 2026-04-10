// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QWINDOWSWINDOWCLASSREGISTRY_H
#define QWINDOWSWINDOWCLASSREGISTRY_H

#include "bobuiwindowsglobal.h"

#include <BobUICore/qloggingcategory.h>
#include <BobUICore/qset.h>
#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQpaWindowClass)

class QWindow;
struct QWindowsWindowClassDescription;

class QWindowsWindowClassRegistry
{
    Q_DISABLE_COPY_MOVE(QWindowsWindowClassRegistry)
public:
    QWindowsWindowClassRegistry(WNDPROC defaultProcedure);
    ~QWindowsWindowClassRegistry();

    static QWindowsWindowClassRegistry *instance();

    QString registerWindowClass(const QWindowsWindowClassDescription &description);
    QString registerWindowClass(const QWindow *window);
    QString registerWindowClass(QString name, WNDPROC procedure);

private:
    static QString classNamePrefix();

    bool shouldDecorateWindowClassName(const QWindowsWindowClassDescription &description) const;
    bool shouldDecorateWindowClassName(const QString &name, WNDPROC procedure) const;
    void unregisterWindowClasses();

    static QWindowsWindowClassRegistry *m_instance;

    WNDPROC m_defaultProcedure;
    QSet<QString> m_registeredWindowClassNames;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSWINDOWCLASSREGISTRY_H
