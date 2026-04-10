// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qwindowswindowclassdescription.h"

#include <BobUIGui/qwindow.h>

#include "qwindowswindowclassregistry.h"

BOBUI_BEGIN_NAMESPACE

using namespace BobUI::StringLiterals;

QString QWindowsWindowClassDescription::classNameSuffix(BobUI::WindowFlags type, unsigned int style, bool hasIcon)
{
    QString suffix;

    switch (type) {
        case BobUI::Popup:
            suffix += "Popup"_L1;
            break;
        case BobUI::Tool:
            suffix += "Tool"_L1;
            break;
        case BobUI::ToolTip:
            suffix += "ToolTip"_L1;
            break;
        default:
            break;
    }

    if (style & CS_DROPSHADOW)
        suffix += "DropShadow"_L1;
    if (style & CS_SAVEBITS)
        suffix += "SaveBits"_L1;
    if (style & CS_OWNDC)
        suffix += "OwnDC"_L1;
    if (hasIcon)
        suffix += "Icon"_L1;

    return suffix;
}

bool QWindowsWindowClassDescription::computeHasIcon(BobUI::WindowFlags flags, BobUI::WindowFlags type)
{
    bool hasIcon = true;

    switch (type) {
        case BobUI::Tool:
        case BobUI::ToolTip:
        case BobUI::Popup:
            hasIcon = false;
            break;
        case BobUI::Dialog:
            if (!(flags & BobUI::WindowSystemMenuHint))
                hasIcon = false; // BOBUIBUG-2027, dialogs without system menu.
            break;
    }

    return hasIcon;
}

unsigned int QWindowsWindowClassDescription::computeWindowStyles(BobUI::WindowFlags flags, BobUI::WindowFlags type, WindowStyleOptions options)
{
    unsigned int style = CS_DBLCLKS;

    // The following will not set CS_OWNDC for any widget window, even if it contains a
    // QOpenGLWidget or QQuickWidget later on. That cannot be detected at this stage.
    if (options.testFlag(WindowStyleOption::GLSurface) || (flags & BobUI::MSWindowsOwnDC))
        style |= CS_OWNDC;
    if (!(flags & BobUI::NoDropShadowWindowHint) && (type == BobUI::Popup || options.testFlag(WindowStyleOption::DropShadow)))
        style |= CS_DROPSHADOW;

    switch (type) {
        case BobUI::Tool:
        case BobUI::ToolTip:
        case BobUI::Popup:
            style |= CS_SAVEBITS; // Save/restore background
            break;
    }

    return style;
}

QWindowsWindowClassDescription QWindowsWindowClassDescription::fromName(QString name, WNDPROC procedure)
{
    return { std::move(name), procedure };
}

QWindowsWindowClassDescription QWindowsWindowClassDescription::fromWindow(const QWindow *window, WNDPROC procedure)
{
    Q_ASSERT(window);

    const BobUI::WindowFlags flags = window->flags();
    const BobUI::WindowFlags type = flags & BobUI::WindowType_Mask;

    WindowStyleOptions options = WindowStyleOption::None;
    if (window->surfaceType() == QSurface::OpenGLSurface)
        options |= WindowStyleOption::GLSurface;
    if (window->property("_q_windowsDropShadow").toBool())
        options |= WindowStyleOption::DropShadow;

    QWindowsWindowClassDescription description;
    description.procedure = procedure;
    description.style = computeWindowStyles(flags, type, options);
    description.hasIcon = computeHasIcon(flags, type);
    description.name = "QWindow"_L1 + classNameSuffix(type, description.style, description.hasIcon);

    return description;
}

QDebug operator<<(QDebug dbg, const QWindowsWindowClassDescription &description)
{
    dbg << description.name
        << " style=0x" << BobUI::hex << description.style << BobUI::dec
        << " brush=" << description.brush
        << " hasIcon=" << description.hasIcon;

    return dbg;
}

BOBUI_END_NAMESPACE
