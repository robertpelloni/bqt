// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSWINDOWCLASSDESCRIPTION_H
#define QWINDOWSWINDOWCLASSDESCRIPTION_H

#include "bobuiwindowsglobal.h"

#include <BobUICore/qstring.h>

BOBUI_BEGIN_NAMESPACE

class QWindow;

struct QWindowsWindowClassDescription
{
    enum class WindowStyleOption
    {
        None = 0x00,
        GLSurface = 0x01,
        DropShadow = 0x02
    };
    Q_DECLARE_FLAGS(WindowStyleOptions, WindowStyleOption)

    static QWindowsWindowClassDescription fromName(QString name, WNDPROC procedure);
    static QWindowsWindowClassDescription fromWindow(const QWindow *window, WNDPROC procedure);

    QString name;
    WNDPROC procedure{ DefWindowProc };
    unsigned int style{ 0 };
    HBRUSH brush{ nullptr };
    bool hasIcon{ false };
    bool shouldAddPrefix{ true };

private:
    static QString classNameSuffix(BobUI::WindowFlags type, unsigned int style, bool hasIcon);
    static bool computeHasIcon(BobUI::WindowFlags flags, BobUI::WindowFlags type);
    static unsigned int computeWindowStyles(BobUI::WindowFlags flags, BobUI::WindowFlags type, WindowStyleOptions options);

    friend QDebug operator<<(QDebug dbg, const QWindowsWindowClassDescription &description);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QWindowsWindowClassDescription::WindowStyleOptions)

BOBUI_END_NAMESPACE

#endif // QWINDOWSWINDOWCLASSDESCRIPTION_H
