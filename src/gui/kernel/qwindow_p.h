// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOW_P_H
#define QWINDOW_P_H

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
#include <BobUIGui/qscreen.h>
#include <BobUIGui/qwindow.h>
#include <qpa/qplatformwindow.h>

#include <BobUICore/private/qobject_p.h>
#include <BobUICore/qelapsedtimer.h>
#include <BobUICore/qxpfunctional.h>
#include <BobUIGui/qicon.h>
#include <BobUIGui/qpalette.h>

#include <BobUICore/qpointer.h>

BOBUI_BEGIN_NAMESPACE

class Q_GUI_EXPORT QWindowPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(QWindow)

public:
    enum PositionPolicy
    {
        WindowFrameInclusive,
        WindowFrameExclusive
    };

    QWindowPrivate(decltype(QObjectPrivateVersion) version = QObjectPrivateVersion);
    ~QWindowPrivate() override;

    void init(QWindow *parent, QScreen *targetScreen = nullptr);

#ifndef BOBUI_NO_CURSOR
    void setCursor(const QCursor *c = nullptr);
    bool applyCursor();
#endif

    QPoint globalPosition() const;

    QWindow *topLevelWindow(QWindow::AncestorMode mode = QWindow::IncludeTransients) const;

    virtual QWindow *eventReceiver() { Q_Q(QWindow); return q; }
    virtual QPalette windowPalette() const { return QPalette(); }

    virtual void setVisible(bool visible);
    void updateVisibility();
    void _q_clearAlert();

    enum SiblingPosition { PositionTop, PositionBottom };
    void updateSiblingPosition(SiblingPosition);

    bool windowRecreationRequired(QScreen *newScreen) const;
    void create(bool recursive);
    void destroy();
    void setTopLevelScreen(QScreen *newScreen, bool recreate);
    void connectToScreen(QScreen *topLevelScreen);
    void disconnectFromScreen();
    void emitScreenChangedRecursion(QScreen *newScreen);
    QScreen *screenForGeometry(const QRect &rect) const;
    void setTransientParent(QWindow *parent);

    virtual void clearFocusObject();

    enum class FocusTarget {
        First,
        Last,
        Current,
        Next,
        Prev
    };
    virtual void setFocusToTarget(FocusTarget, BobUI::FocusReason) {}

    virtual QRectF closestAcceptableGeometry(const QRectF &rect) const;

    void setMinOrMaxSize(QSize *oldSizeMember, const QSize &size,
                         qxp::function_ref<void()> funcWidthChanged,
                         qxp::function_ref<void()> funcHeightChanged);

    virtual bool participatesInLastWindowClosed() const;
    virtual bool treatAsVisible() const;

    virtual void maybeSynthesizeContextMenuEvent(QMouseEvent *event);

    const QWindow *forwardToPopup(QEvent *event, const QWindow *activePopupOnPress);

    bool isPopup() const { return (windowFlags & BobUI::WindowType_Mask) == BobUI::Popup; }
    void setAutomaticPositionAndResizeEnabled(bool a)
    { positionAutomatic = resizeAutomatic = a; }

    bool updateDevicePixelRatio();

    static QWindowPrivate *get(QWindow *window) { return window->d_func(); }

    static BobUI::WindowState effectiveState(BobUI::WindowStates);

    QWindow::SurfaceType surfaceType = QWindow::RasterSurface;
    BobUI::WindowFlags windowFlags = BobUI::Window;
    QWindow *parentWindow = nullptr;
    QPlatformWindow *platformWindow = nullptr;
    bool visible= false;
    bool visibilityOnDestroy = false;
    bool exposed = false;
    bool inClose = false;
    QSurfaceFormat requestedFormat;
    QString windowTitle;
    QString windowFilePath;
    QIcon windowIcon;
    QRect geometry;
    qreal devicePixelRatio = 1.0;
    BobUI::WindowStates windowState = BobUI::WindowNoState;
    QWindow::Visibility visibility = QWindow::Hidden;
    bool resizeEventPending = true;
    bool receivedExpose = false;
    PositionPolicy positionPolicy = WindowFrameExclusive;
    bool positionAutomatic = true;
    // resizeAutomatic suppresses resizing by QPlatformWindow::initialGeometry().
    // It also indicates that width/height=0 is acceptable (for example, for
    // the QRollEffect widget) and is thus not cleared in setGeometry().
    // An alternative approach might be using -1,-1 as a default size.
    bool resizeAutomatic = true;
    BobUI::ScreenOrientation contentOrientation = BobUI::PrimaryOrientation;
    qreal opacity= 1;
    QRegion mask;

    QSize minimumSize = {0, 0};
    QSize maximumSize = {QWINDOWSIZE_MAX, QWINDOWSIZE_MAX};
    QSize baseSize;
    QSize sizeIncrement;

    BobUI::WindowModality modality = BobUI::NonModal;
    bool blockedByModalWindow = false;

    bool updateRequestPending = false;
    bool transientParentPropertySet = false;

    QPointer<QWindow> transientParent;
    QPointer<QScreen> topLevelScreen;

#ifndef BOBUI_NO_CURSOR
    QCursor cursor = {BobUI::ArrowCursor};
    bool hasCursor = false;
#endif

    QElapsedTimer lastComposeTime;

#if BOBUI_CONFIG(vulkan)
    QVulkanInstance *vulkanInstance = nullptr;
#endif
};


BOBUI_END_NAMESPACE

#endif // QWINDOW_P_H
