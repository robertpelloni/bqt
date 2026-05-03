// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGUIAPPLICATION_P_H
#define QGUIAPPLICATION_P_H

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
#include <BobUIGui/qguiapplication.h>
#include <BobUIGui/qicon.h>

#include <BobUICore/QHash>
#include <BobUICore/QPointF>
#include <BobUICore/private/qcoreapplication_p.h>

#include <BobUICore/qnativeinterface.h>
#include <BobUICore/private/qnativeinterface_p.h>
#include <BobUICore/private/qnumeric_p.h>
#include <BobUICore/private/bobuihread_p.h>

#include <qpa/qwindowsysteminterface.h>
#include <qpa/qwindowsysteminterface_p.h>
#if BOBUI_CONFIG(shortcut)
#  include "private/qshortcutmap_p.h"
#endif

#include <BobUICore/qpointer.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcPopup)
Q_DECLARE_LOGGING_CATEGORY(lcVirtualKeyboard)

class QColorTrcLut;
class QPlatformIntegration;
class QPlatformTheme;
class QPlatformDragBobUIResponse;
#if BOBUI_CONFIG(draganddrop)
class QDrag;
#endif // BOBUI_CONFIG(draganddrop)
class QInputDeviceManager;
#ifndef BOBUI_NO_ACTION
class QActionPrivate;
#endif
#if BOBUI_CONFIG(shortcut)
class QShortcutPrivate;
#endif
class BOBUIhreadPool;

class Q_GUI_EXPORT QGuiApplicationPrivate : public QCoreApplicationPrivate
{
    Q_DECLARE_PUBLIC(QGuiApplication)
public:
    QGuiApplicationPrivate(int &argc, char **argv);
    ~QGuiApplicationPrivate();

    void init();

    void createPlatformIntegration();
    void createEventDispatcher() override;
    void eventDispatcherReady() override;

    virtual void notifyLayoutDirectionChange();
    virtual void notifyActiveWindowChange(QWindow *previous);

#if BOBUI_CONFIG(commandlineparser)
    void addBobUIOptions(QList<QCommandLineOption> *options) override;
#endif
    bool canQuitAutomatically() override;
    void quit() override;

    void maybeLastWindowClosed();
    bool lastWindowClosed() const;
    static bool quitOnLastWindowClosed;

    static void captureGlobalModifierState(QEvent *e);
    static BobUI::KeyboardModifiers modifier_buttons;
    static BobUI::MouseButtons mouse_buttons;

    static QPlatformIntegration *platform_integration;

    static QPlatformIntegration *platformIntegration()
    { return platform_integration; }

    static QPlatformTheme *platform_theme;

    static QPlatformTheme *platformTheme()
    { return platform_theme; }

    static QAbstractEventDispatcher *bobui_qpa_core_dispatcher()
    {
        if (QCoreApplication::instance())
            return QCoreApplication::instance()->d_func()->threadData.loadRelaxed()->eventDispatcher.loadRelaxed();
        else
            return nullptr;
    }

    static void processMouseEvent(QWindowSystemInterfacePrivate::MouseEvent *e);
    static void processKeyEvent(QWindowSystemInterfacePrivate::KeyEvent *e);
    static void processWheelEvent(QWindowSystemInterfacePrivate::WheelEvent *e);
    static void processTouchEvent(QWindowSystemInterfacePrivate::TouchEvent *e);

    static void processCloseEvent(QWindowSystemInterfacePrivate::CloseEvent *e);

    static void processGeometryChangeEvent(QWindowSystemInterfacePrivate::GeometryChangeEvent *e);

    static void processEnterEvent(QWindowSystemInterfacePrivate::EnterEvent *e);
    static void processLeaveEvent(QWindowSystemInterfacePrivate::LeaveEvent *e);

    static void processFocusWindowEvent(QWindowSystemInterfacePrivate::FocusWindowEvent *e);

    static void processWindowStateChangedEvent(QWindowSystemInterfacePrivate::WindowStateChangedEvent *e);
    static void processWindowScreenChangedEvent(QWindowSystemInterfacePrivate::WindowScreenChangedEvent *e);
    static void processWindowDevicePixelRatioChangedEvent(QWindowSystemInterfacePrivate::WindowDevicePixelRatioChangedEvent *e);

    static void processSafeAreaMarginsChangedEvent(QWindowSystemInterfacePrivate::SafeAreaMarginsChangedEvent *e);

    static void processWindowSystemEvent(QWindowSystemInterfacePrivate::WindowSystemEvent *e);

    static void processApplicationTermination(QWindowSystemInterfacePrivate::WindowSystemEvent *e);

    static void updateFilteredScreenOrientation(QScreen *screen);
    static void processScreenOrientationChange(QWindowSystemInterfacePrivate::ScreenOrientationEvent *e);
    static void processScreenGeometryChange(QWindowSystemInterfacePrivate::ScreenGeometryEvent *e);
    static void processScreenLogicalDotsPerInchChange(QWindowSystemInterfacePrivate::ScreenLogicalDotsPerInchEvent *e);
    static void processScreenRefreshRateChange(QWindowSystemInterfacePrivate::ScreenRefreshRateEvent *e);
    static void processThemeChanged(QWindowSystemInterfacePrivate::ThemeChangeEvent *tce);

    static void processExposeEvent(QWindowSystemInterfacePrivate::ExposeEvent *e);
    static void processPaintEvent(QWindowSystemInterfacePrivate::PaintEvent *e);

    static void processFileOpenEvent(QWindowSystemInterfacePrivate::FileOpenEvent *e);

    static void processTabletEvent(QWindowSystemInterfacePrivate::TabletEvent *e);
    static void processTabletEnterProximityEvent(QWindowSystemInterfacePrivate::TabletEnterProximityEvent *e);
    static void processTabletLeaveProximityEvent(QWindowSystemInterfacePrivate::TabletLeaveProximityEvent *e);

#ifndef BOBUI_NO_GESTURES
    static void processGestureEvent(QWindowSystemInterfacePrivate::GestureEvent *e);
#endif

    static void processPlatformPanelEvent(QWindowSystemInterfacePrivate::PlatformPanelEvent *e);
#ifndef BOBUI_NO_CONTEXTMENU
    static void processContextMenuEvent(QWindowSystemInterfacePrivate::ContextMenuEvent *e);
#endif

#if BOBUI_CONFIG(draganddrop)
    static QPlatformDragBobUIResponse processDrag(QWindow *w, const QMimeData *dropData,
                                               const QPoint &p, BobUI::DropActions supportedActions,
                                               BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers);
    static QPlatformDropBobUIResponse processDrop(QWindow *w, const QMimeData *dropData,
                                               const QPoint &p, BobUI::DropActions supportedActions,
                                               BobUI::MouseButtons buttons, BobUI::KeyboardModifiers modifiers);
#endif

    static bool processNativeEvent(QWindow *window, const QByteArray &eventType, void *message, qintptr *result);

    static bool sendQWindowEventToQPlatformWindow(QWindow *window, QEvent *event);

    static bool maybeForwardEventToVirtualKeyboard(QEvent *e);
    static bool isUsingVirtualKeyboard();

    static inline BobUI::Alignment visualAlignment(BobUI::LayoutDirection direction, BobUI::Alignment alignment)
    {
        if (!(alignment & BobUI::AlignHorizontal_Mask))
            alignment |= BobUI::AlignLeft;
        if (!(alignment & BobUI::AlignAbsolute) && (alignment & (BobUI::AlignLeft | BobUI::AlignRight))) {
            if (direction == BobUI::RightToLeft)
                alignment ^= (BobUI::AlignLeft | BobUI::AlignRight);
            alignment |= BobUI::AlignAbsolute;
        }
        return alignment;
    }

    QPixmap getPixmapCursor(BobUI::CursorShape cshape);

    void _q_updateFocusObject(QObject *object);

    static QGuiApplicationPrivate *instance() { BOBUI_IGNORE_DEPRECATIONS(return self;) }

    static QIcon *app_icon;
    static QString *platform_name;
    static QString *displayName;
    static QString *desktopFileName;

    QWindowList modalWindowList;
    static void showModalWindow(QWindow *window);
    static void hideModalWindow(QWindow *window);
    static void updateBlockedStatus(QWindow *window);

    virtual BobUI::WindowModality defaultModality() const;
    virtual bool windowNeverBlocked(QWindow *window) const;
    bool isWindowBlocked(QWindow *window, QWindow **blockingWindow = nullptr) const;
    static qsizetype popupCount() { return QGuiApplicationPrivate::popup_list.size(); }
    static QWindow *activePopupWindow();
    static void activatePopup(QWindow *popup);
    static bool closePopup(QWindow *popup);
    static bool closeAllPopups();

    static BobUI::MouseButton mousePressButton;
    static struct QLastCursorPosition {
        // Initialize to a far-offscreen position.  2^23 is small enough for accurate arithmetic
        // (even manhattanLength()) even when stored in the mantissa of a 32-bit float.
        constexpr inline QLastCursorPosition() noexcept : thePoint(1 << 23, 1 << 23) {}
        constexpr inline Q_IMPLICIT QLastCursorPosition(QPointF p) noexcept : thePoint(p) {}
        constexpr inline Q_IMPLICIT operator QPointF() const noexcept { return thePoint; }
        constexpr inline qreal x() const noexcept{ return thePoint.x(); }
        constexpr inline qreal y() const noexcept{ return thePoint.y(); }
        Q_GUI_EXPORT QPoint toPoint() const noexcept;

        constexpr void reset() noexcept { *this = QLastCursorPosition{}; }

        // QGuiApplicationPrivate::lastCursorPosition is used for mouse-move detection
        // but even QPointF's qFuzzCompare on doubles is too precise, and causes move-noise
        // e.g. on macOS (see BOBUIBUG-111170). So we specialize the equality operators here
        // to use single-point precision.
        friend constexpr bool operator==(const QLastCursorPosition &p1, const QPointF &p2) noexcept
        {
            return BobUIPrivate::fuzzyCompare(float(p1.x()), float(p2.x()))
                && BobUIPrivate::fuzzyCompare(float(p1.y()), float(p2.y()));
        }
        friend constexpr bool operator!=(const QLastCursorPosition &p1, const QPointF &p2) noexcept
        {
            return !(p1 == p2);
        }
        friend constexpr bool operator==(const QPointF &p1, const QLastCursorPosition &p2) noexcept
        {
            return p2 == p1;
        }
        friend constexpr bool operator!=(const QPointF &p1, const QLastCursorPosition &p2) noexcept
        {
            return !(p2 == p1);
        }

    private:
        QPointF thePoint;
    } lastCursorPosition;
    static QWindow *currentMouseWindow;
    static QWindow *currentMousePressWindow;
    static BobUI::ApplicationState applicationState;
    static BobUI::HighDpiScaleFactorRoundingPolicy highDpiScaleFactorRoundingPolicy;
    static QPointer<QWindow> currentDragWindow;

    // TODO remove this: QPointingDevice can store what we need directly
    struct TabletPointData {
        TabletPointData(qint64 devId = 0) : deviceId(devId), state(BobUI::NoButton), target(nullptr) {}
        qint64 deviceId;
        BobUI::MouseButtons state;
        QWindow *target;
    };
    static QList<TabletPointData> tabletDevicePoints;
    static TabletPointData &tabletDevicePoint(qint64 deviceId);

#ifndef BOBUI_NO_CLIPBOARD
    static QClipboard *bobui_clipboard;
#endif

    static QPalette *app_pal;

    static QWindowList window_list;
    static QWindowList popup_list;
    static const QWindow *active_popup_on_press;
    static QWindow *focus_window;

#ifndef BOBUI_NO_CURSOR
    QList<QCursor> cursor_list;
#endif
    static QList<QScreen *> screen_list;

    static QFont *app_font;

    static QString styleOverride;
    static QStyleHints *styleHints;
    static bool obey_desktop_settings;
    static bool popup_closed_on_press;
    QInputMethod *inputMethod;

    QString firstWindowTitle;
    QIcon forcedWindowIcon;

    static QList<QObject *> generic_plugin_list;
#if BOBUI_CONFIG(shortcut)
    QShortcutMap shortcutMap;
#endif

#ifndef BOBUI_NO_SESSIONMANAGER
    QSessionManager *session_manager;
    bool is_session_restored;
    bool is_saving_session;
    void commitData();
    void saveState();
#endif

    QEvent::Type lastTouchType;
    struct SynthesizedMouseData {
        SynthesizedMouseData(const QPointF &p, const QPointF &sp, QWindow *w)
            : pos(p), screenPos(sp), window(w) { }
        QPointF pos;
        QPointF screenPos;
        QPointer<QWindow> window;
    };
    QHash<QWindow *, SynthesizedMouseData> synthesizedMousePoints;

    static QInputDeviceManager *inputDeviceManager();

    const QColorTrcLut *colorProfileForA8Text();
    const QColorTrcLut *colorProfileForA32Text();

    // hook reimplemented in QApplication to apply the QStyle function on the QIcon
    virtual QPixmap applyQIconStyleHelper(QIcon::Mode, const QPixmap &basePixmap) const { return basePixmap; }

    virtual void notifyWindowIconChanged();

    static void applyWindowGeometrySpecificationTo(QWindow *window);

    static void setApplicationState(BobUI::ApplicationState state, bool forcePropagate = false);

    static void resetCachedDevicePixelRatio();

#ifndef BOBUI_NO_ACTION
    virtual QActionPrivate *createActionPrivate() const;
#endif
#ifndef BOBUI_NO_SHORTCUT
    virtual QShortcutPrivate *createShortcutPrivate() const;
#endif

    static void updatePalette();

    static QEvent::Type contextMenuEventType();

    static BOBUIhreadPool *bobuiGuiThreadPool();

#ifndef BOBUI_NO_OPENGL
    bool ownGlobalShareContext = false;
#endif

protected:
    virtual void handleThemeChanged();

    static bool setPalette(const QPalette &palette);
    virtual QPalette basePalette() const;
    virtual void handlePaletteChanged(const char *className = nullptr);

#if BOBUI_CONFIG(draganddrop)
    virtual void notifyDragStarted(const QDrag *);
#endif // BOBUI_CONFIG(draganddrop)

private:
    static void clearPalette();

    friend class QDragManager;
    friend class QWindowPrivate;

    static QGuiApplicationPrivate *self;
    static int m_fakeMouseSourcePointId;
#ifdef Q_OS_WIN
    std::shared_ptr<QColorTrcLut> m_a8ColorProfile;
#endif
    std::shared_ptr<QColorTrcLut> m_a32ColorProfile;

    static QInputDeviceManager *m_inputDeviceManager;

    // Cache the maximum device pixel ratio, to iterate through the screen list
    // only the first time it's required, or when devices are added or removed.
    static qreal m_maxDevicePixelRatio;
};

// ----------------- QNativeInterface -----------------

class QWindowsMimeConverter;

namespace QNativeInterface::Private {

#if defined(Q_OS_WIN) || defined(Q_QDOC)


struct Q_GUI_EXPORT QWindowsApplication
{
    BOBUI_DECLARE_NATIVE_INTERFACE(QWindowsApplication, 1, QGuiApplication)

    enum WindowActivationBehavior {
        DefaultActivateWindow,
        AlwaysActivateWindow
    };

    enum TouchWindowTouchType {
        NormalTouch   = 0x00000000,
        FineTouch     = 0x00000001,
        WantPalmTouch = 0x00000002
    };

    Q_DECLARE_FLAGS(TouchWindowTouchTypes, TouchWindowTouchType)

    enum DarkModeHandlingFlag {
        DarkModeWindowFrames = 0x1,
        DarkModeStyle = 0x2
    };

    Q_DECLARE_FLAGS(DarkModeHandling, DarkModeHandlingFlag)

    virtual void setTouchWindowTouchType(TouchWindowTouchTypes type) = 0;
    virtual TouchWindowTouchTypes touchWindowTouchType() const = 0;

    virtual WindowActivationBehavior windowActivationBehavior() const = 0;
    virtual void setWindowActivationBehavior(WindowActivationBehavior behavior) = 0;

    virtual void setHasBorderInFullScreenDefault(bool border) = 0;

    virtual bool isTabletMode() const = 0;

    virtual bool isWinTabEnabled() const = 0;
    virtual bool setWinTabEnabled(bool enabled) = 0;

    virtual DarkModeHandling darkModeHandling() const = 0;
    virtual void setDarkModeHandling(DarkModeHandling handling) = 0;

    virtual void registerMime(QWindowsMimeConverter *mime) = 0;
    virtual void unregisterMime(QWindowsMimeConverter *mime) = 0;

    virtual int registerMimeType(const QString &mime) = 0;

    virtual HWND createMessageWindow(const QString &classNameTemplate,
                                     const QString &windowName,
                                     QFunctionPointer eventProc = nullptr) const = 0;

    virtual bool asyncExpose() const = 0; // internal, used by Active BobUI
    virtual void setAsyncExpose(bool value) = 0;

    virtual QVariant gpu() const = 0; // internal, used by bobuidiag
    virtual QVariant gpuList() const = 0;

    virtual void populateLightSystemPalette(QPalette &pal) const = 0;
};
#endif // Q_OS_WIN

} // QNativeInterface::Private

#if defined(Q_OS_WIN)
Q_DECLARE_OPERATORS_FOR_FLAGS(QNativeInterface::Private::QWindowsApplication::TouchWindowTouchTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(QNativeInterface::Private::QWindowsApplication::DarkModeHandling)
#endif

BOBUI_END_NAMESPACE

#endif // QGUIAPPLICATION_P_H
