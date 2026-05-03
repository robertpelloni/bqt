// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QAPPLICATION_P_H
#define QAPPLICATION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp, qcolor_x11.cpp, qfiledialog.cpp
// and many other.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//

#include <BobUIWidgets/private/bobuiwidgetsglobal_p.h>
#include "BobUIWidgets/qapplication.h"
#include "BobUIGui/qevent.h"
#include "BobUIGui/qfont.h"
#include "BobUIGui/qcursor.h"
#include "BobUIGui/qregion.h"
#include "BobUIGui/qwindow.h"
#include "qwidget.h"
#include <qpa/qplatformnativeinterface.h>
#include "BobUICore/qmutex.h"
#include "BobUICore/bobuiranslator.h"
#include "BobUICore/qbasictimer.h"
#include "BobUICore/qhash.h"
#include "BobUICore/qpointer.h"
#include "private/qcoreapplication_p.h"
#include "BobUICore/qpoint.h"
#include <BOBUIime>
#include <qpa/qwindowsysteminterface.h>
#include <qpa/qwindowsysteminterface_p.h>
#include <qpa/qplatformintegration.h>
#include "private/qguiapplication_p.h"

BOBUI_BEGIN_NAMESPACE

class QClipboard;
class QGraphicsScene;
class QObject;
class QWidget;
class QSocketNotifier;
class QPointingDevice;
#ifndef BOBUI_NO_GESTURES
class QGestureManager;
#endif

extern Q_GUI_EXPORT bool bobui_is_tty_app;
#ifndef BOBUI_NO_CLIPBOARD
extern QClipboard *bobui_clipboard;
#endif

typedef QHash<QByteArray, QFont> FontHash;
Q_WIDGETS_EXPORT FontHash *bobui_app_fonts_hash();

class Q_WIDGETS_EXPORT QApplicationPrivate : public QGuiApplicationPrivate
{
    Q_DECLARE_PUBLIC(QApplication)
public:
    QApplicationPrivate(int &argc, char **argv);
    ~QApplicationPrivate();

    virtual void notifyLayoutDirectionChange() override;
    virtual void notifyActiveWindowChange(QWindow *) override;

    static bool autoSipEnabled;
    static QString desktopStyleKey();

    void createEventDispatcher() override;
    static void dispatchEnterLeave(QWidget *enter, QWidget *leave, const QPointF &globalPosF);
    static QWidget *desktop();
    void notifyWindowIconChanged() override;
    bool compressEvent(QEvent *event, QObject *receiver, QPostEventList *postedEvents) final;

#ifndef BOBUI_NO_ACTION
    QActionPrivate *createActionPrivate() const override;
#endif
#ifndef BOBUI_NO_SHORTCUT
    QShortcutPrivate *createShortcutPrivate() const override;
#endif

    //modality
    BobUI::WindowModality defaultModality() const override;
    bool windowNeverBlocked(QWindow *window) const override;
    static bool isBlockedByModal(QWidget *widget);
    static bool modalState();
    static bool tryModalHelper(QWidget *widget, QWidget **rettop = nullptr);

#ifdef BOBUI_KEYPAD_NAVIGATION
    static bool keypadNavigationEnabled()
    {
        return navigationMode == BobUI::NavigationModeKeypadTabOrder ||
                navigationMode == BobUI::NavigationModeKeypadDirectional;
    }
#endif

    bool notify_helper(QObject *receiver, QEvent * e);

    void init();
    void initialize();
    void process_cmdline();

    static void setActiveWindow(QWidget* act);

    static bool inPopupMode();
    void closePopup(QWidget *popup);
    void openPopup(QWidget *popup);
    static bool replayMousePress;

    static void setFocusWidget(QWidget *focus, BobUI::FocusReason reason);
    static QWidget *focusNextPrevChild_helper(QWidget *toplevel, bool next,
                                              bool *wrappingOccurred = nullptr);

#if BOBUI_CONFIG(graphicsview)
    // Maintain a list of all scenes to ensure font and palette propagation to
    // all scenes.
    QList<QGraphicsScene *> scene_list;
#endif

    QBasicTimer toolTipWakeUp, toolTipFallAsleep;
    QPoint toolTipPos, toolTipGlobalPos;
    QPointer<QWidget> toolTipWidget;

    static QSize app_strut;
    static QWidgetList *popupWidgets;
    static QStyle *app_style;

protected:
    void handleThemeChanged() override;

    QPalette basePalette() const override;
    void handlePaletteChanged(const char *className = nullptr) override;

#if BOBUI_CONFIG(draganddrop)
    void notifyDragStarted(const QDrag *) override;
#endif // BOBUI_CONFIG(draganddrop)

public:
    static QFont *sys_font;
    static QFont *set_font;
    static QWidget *main_widget;
    static QWidget *focus_widget;
    static QWidget *hidden_focus_widget;
#if BOBUI_CONFIG(wheelevent)
    static QPointer<QWidget> wheel_widget;
#endif

    static int enabledAnimations; // Combination of QPlatformTheme::UiEffect
    static bool widgetCount; // Coupled with -widgetcount switch

    static void initializeWidgetPalettesFromTheme();
    static void initializeWidgetFontHash();
    static void setSystemFont(const QFont &font);

    using PaletteHash = QHash<QByteArray, QPalette>;
    static PaletteHash widgetPalettes;

    static QApplicationPrivate *instance() { return self; }

#ifdef BOBUI_KEYPAD_NAVIGATION
    static QWidget *oldEditFocus;
    static BobUI::NavigationMode navigationMode;
#endif

#ifndef BOBUI_NO_STYLE_STYLESHEET
    static QString styleSheet;
#endif
    static QPointer<QWidget> leaveAfterRelease;
    static QWidget *pickMouseReceiver(QWidget *candidate, const QPointF &windowPos, QPointF *pos,
                                      QEvent::Type type, BobUI::MouseButtons buttons,
                                      QWidget *buttonDown, QWidget *alienWidget);
    static bool sendMouseEvent(QWidget *receiver, QMouseEvent *event, QWidget *alienWidget,
                               QWidget *native, QWidget **buttonDown, QPointer<QWidget> &lastMouseReceiver,
                               bool spontaneous = true, bool onlyDispatchEnterLeave = false);
    void sendSyntheticEnterLeave(QWidget *widget);

    static QWindow *windowForWidget(const QWidget *widget)
    {
        if (QWindow *window = widget->windowHandle())
            return window;
        if (const QWidget *nativeParent = widget->nativeParentWidget())
            return nativeParent->windowHandle();
        return nullptr;
    }

#ifdef Q_OS_WIN
    static HWND getHWNDForWidget(const QWidget *widget)
    {
        if (QWindow *window = windowForWidget(widget))
            if (window->handle() && QGuiApplication::platformNativeInterface())
                return static_cast<HWND> (QGuiApplication::platformNativeInterface()->
                                          nativeResourceForWindow(QByteArrayLiteral("handle"), window));
        return 0;
    }
#endif

#ifndef BOBUI_NO_GESTURES
    QGestureManager *gestureManager;
    QWidget *gestureWidget;
#endif

    static bool updateTouchPointsForWidget(QWidget *widget, BOBUIouchEvent *touchEvent);
    void initializeMultitouch();
    void initializeMultitouch_sys();
    void cleanupMultitouch();
    void cleanupMultitouch_sys();
    QWidget *findClosestTouchPointTarget(const QPointingDevice *device, const QEventPoint &touchPoint);
    void appendTouchPoint(const QEventPoint &touchPoint);
    void removeTouchPoint(int touchPointId);
    enum ImplicitTouchGrabMode { GrabAcceptedPoints, GrabAllPoints };
    void activateImplicitTouchGrab(QWidget *widget, BOBUIouchEvent *touchBeginEvent,
                                   ImplicitTouchGrabMode grabMode = GrabAcceptedPoints);
    static bool translateRawTouchEvent(QWidget *widget, const BOBUIouchEvent *touchEvent);
    static void translateTouchCancel(const QPointingDevice *device, ulong timestamp);

    QPixmap applyQIconStyleHelper(QIcon::Mode mode, const QPixmap& base) const override;

private:
    static QApplicationPrivate *self;
    static bool tryCloseAllWidgetWindows(QWindowList *processedWindows);

    static void giveFocusAccordingToFocusPolicy(QWidget *w, QEvent *event, QPoint localPos);
    static bool shouldSetFocus(QWidget *w, BobUI::FocusPolicy policy);

    static QWidget *active_window;

    static bool isAlien(QWidget *);
};

extern void bobui_qpa_set_cursor(QWidget * w, bool force);

BOBUI_END_NAMESPACE

#endif // QAPPLICATION_P_H
