// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwindowdump.h"

#include <BobUIGui/QGuiApplication>
#include <BobUIGui/QScreen>
#include <BobUIGui/QWindow>
#include <BobUICore/QDebug>
#include <BobUICore/QMetaObject>
#include <BobUICore/QRect>
#include <BobUICore/BOBUIextStream>

#include <qpa/qplatformwindow.h>
#include <private/qwindow_p.h>
#include <private/qhighdpiscaling_p.h>

namespace BobUIDiag {

void indentStream(BOBUIextStream &s, int indent)
{
    for (int i = 0; i < indent; ++i)
        s << ' ';
}

void formatObject(BOBUIextStream &str, const QObject *o)
{
    str << o->metaObject()->className();
    const QString on = o->objectName();
    if (!on.isEmpty())
        str << "/\"" << on << '"';
}

void formatRect(BOBUIextStream &str, const QRect &geom)
{
    str << geom.width() << 'x' << geom.height() << BobUI::forcesign
        << geom.x() << geom.y() << BobUI::noforcesign;
}

#define debugType(s, type, typeConstant) \
if ((type & typeConstant) == typeConstant) \
    s << ' ' << #typeConstant;

#define debugFlag(s, flags, flagConstant) \
if (flags & flagConstant) \
    s << ' ' << #flagConstant;

void formatWindowFlags(BOBUIextStream &str, BobUI::WindowFlags flags)
{
    str << BobUI::showbase << BobUI::hex << unsigned(flags) << BobUI::dec << BobUI::noshowbase;
    const BobUI::WindowFlags windowType = flags & BobUI::WindowType_Mask;
    debugFlag(str, flags, BobUI::Window)
    debugType(str, windowType, BobUI::Dialog)
    debugType(str, windowType, BobUI::Sheet)
    debugType(str, windowType, BobUI::Drawer)
    debugType(str, windowType, BobUI::Popup)
    debugType(str, windowType, BobUI::Tool)
    debugType(str, windowType, BobUI::ToolTip)
    debugType(str, windowType, BobUI::SplashScreen)
    debugType(str, windowType, BobUI::SubWindow)
    debugType(str, windowType, BobUI::ForeignWindow)
    debugType(str, windowType, BobUI::CoverWindow)
    debugFlag(str, flags, BobUI::MSWindowsFixedSizeDialogHint)
    debugFlag(str, flags, BobUI::MSWindowsOwnDC)
    debugFlag(str, flags, BobUI::X11BypassWindowManagerHint)
    debugFlag(str, flags, BobUI::FramelessWindowHint)
    debugFlag(str, flags, BobUI::WindowTitleHint)
    debugFlag(str, flags, BobUI::WindowSystemMenuHint)
    debugFlag(str, flags, BobUI::WindowMinimizeButtonHint)
    debugFlag(str, flags, BobUI::WindowMaximizeButtonHint)
    debugFlag(str, flags, BobUI::WindowContextHelpButtonHint)
    debugFlag(str, flags, BobUI::WindowShadeButtonHint)
    debugFlag(str, flags, BobUI::WindowStaysOnTopHint)
    debugFlag(str, flags, BobUI::CustomizeWindowHint)
    debugFlag(str, flags, BobUI::WindowTransparentForInput)
    debugFlag(str, flags, BobUI::WindowOverridesSystemGestures)
    debugFlag(str, flags, BobUI::WindowDoesNotAcceptFocus)
    debugFlag(str, flags, BobUI::NoDropShadowWindowHint)
    debugFlag(str, flags, BobUI::WindowFullscreenButtonHint)
    debugFlag(str, flags, BobUI::WindowStaysOnBottomHint)
    debugFlag(str, flags, BobUI::BypassGraphicsProxyWidget)
}

void formatWindow(BOBUIextStream &str, const QWindow *w, FormatWindowOptions options)
{
    const QPlatformWindow *pw = w->handle();
    formatObject(str, w);
    str << ' ' << (w->isVisible() ? "[visible] " : "[hidden] ");
    if (const WId nativeWinId = pw ? pw->winId() : WId(0))
        str << "[native: " << BobUI::hex << BobUI::showbase << nativeWinId << BobUI::dec << BobUI::noshowbase
            << "] ";
    if (w->isTopLevel())
        str << "[top] ";
    if (w->isExposed())
        str << "[exposed] ";
    if (w->surfaceClass() == QWindow::Offscreen)
        str << "[offscreen] ";
    str << "surface=" << w->surfaceType() << ' ';
    if (const BobUI::WindowState state = w->windowState())
        str << "windowState=" << state << ' ';
    formatRect(str, w->geometry());
    if (w->isTopLevel()) {
        str << " \"" << w->screen()->name() << "\" ";
        if (QHighDpiScaling::isActive())
            str << "factor=" << QHighDpiScaling::factor(w) << " dpr=" << w->devicePixelRatio();
    }
    if (!(options & DontPrintWindowFlags)) {
        str << ' ';
        formatWindowFlags(str, w->flags());
    }
    if (options & PrintSizeConstraints) {
        str << ' ';
        const QSize minimumSize = w->minimumSize();
        if (minimumSize.width() > 0 || minimumSize.height() > 0)
            str << "minimumSize=" << minimumSize.width() << 'x' << minimumSize.height() << ' ';
        const QSize maximumSize = w->maximumSize();
        if (maximumSize.width() < QWINDOWSIZE_MAX || maximumSize.height() < QWINDOWSIZE_MAX)
            str << "maximumSize=" << maximumSize.width() << 'x' << maximumSize.height() << ' ';
    }
    str << '\n';
}

static void dumpWindowRecursion(BOBUIextStream &str, const QWindow *w,
                                FormatWindowOptions options = {}, int depth = 0)
{
    indentStream(str, 2 * depth);
    formatWindow(str, w, options);
    for (const QObject *co : w->children()) {
        if (co->isWindowType())
            dumpWindowRecursion(str, static_cast<const QWindow *>(co), options, depth + 1);
    }
}

void dumpAllWindows(FormatWindowOptions options)
{
    QString d;
    BOBUIextStream str(&d);
    str << "### QWindows:\n";
    for (QWindow *w : QGuiApplication::topLevelWindows())
        dumpWindowRecursion(str, w, options);
    qDebug().noquote() << d;
}

} // namespace BobUIDiag
