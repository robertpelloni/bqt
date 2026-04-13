// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QAPPLICATION_H
#define QAPPLICATION_H

#include <BobUIWidgets/bobuiwidgetsglobal.h>
#include <BobUICore/qcoreapplication.h>
#include <BobUIGui/qwindowdefs.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qsize.h>
#include <BobUIGui/qcursor.h>
#include <BobUIGui/qguiapplication.h>

BOBUI_BEGIN_NAMESPACE


class QStyle;
class QEventLoop;
class QIcon;
class QLocale;
class QPlatformNativeInterface;

class QApplication;
class QApplicationPrivate;
#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<QApplication *>(QCoreApplication::instance()))

class Q_WIDGETS_EXPORT QApplication : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(int cursorFlashTime READ cursorFlashTime WRITE setCursorFlashTime)
    Q_PROPERTY(int doubleClickInterval  READ doubleClickInterval WRITE setDoubleClickInterval)
    Q_PROPERTY(int keyboardInputInterval READ keyboardInputInterval WRITE setKeyboardInputInterval)
#if BOBUI_CONFIG(wheelevent)
    Q_PROPERTY(int wheelScrollLines  READ wheelScrollLines WRITE setWheelScrollLines)
#endif
    Q_PROPERTY(int startDragTime  READ startDragTime WRITE setStartDragTime)
    Q_PROPERTY(int startDragDistance  READ startDragDistance WRITE setStartDragDistance)
#ifndef BOBUI_NO_STYLE_STYLESHEET
    Q_PROPERTY(QString styleSheet READ styleSheet WRITE setStyleSheet)
#endif
    Q_PROPERTY(bool autoSipEnabled READ autoSipEnabled WRITE setAutoSipEnabled)

public:
#ifdef Q_QDOC
    QApplication(int &argc, char **argv);
#else
    QApplication(int &argc, char **argv, int = ApplicationFlags);
#endif
    virtual ~QApplication();

    static QStyle *style();
    static void setStyle(QStyle*);
    static QStyle *setStyle(const QString&);

    using QGuiApplication::palette;
    static QPalette palette(const QWidget *);
    static QPalette palette(const char *className);
    static void setPalette(const QPalette &, const char* className = nullptr);
    static QFont font();
    static QFont font(const QWidget*);
    static QFont font(const char *className);
    static void setFont(const QFont &, const char* className = nullptr);

#if BOBUI_DEPRECATED_SINCE(6,0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Use QFontMetricsF(qApp->font()) instead.")
    static QFontMetrics fontMetrics();
#endif

    static QWidgetList allWidgets();
    static QWidgetList topLevelWidgets();

    static QWidget *activePopupWidget();
    static QWidget *activeModalWidget();
    static QWidget *focusWidget();

    static QWidget *activeWindow();

#if BOBUI_DEPRECATED_SINCE(6, 5)
    BOBUI_DEPRECATED_VERSION_X_6_5("Use QWidget::activateWindow() instead.")
    static void setActiveWindow(QWidget* act);
#endif

    static QWidget *widgetAt(const QPoint &p);
    static inline QWidget *widgetAt(int x, int y) { return widgetAt(QPoint(x, y)); }
    static QWidget *topLevelAt(const QPoint &p);
    static inline QWidget *topLevelAt(int x, int y)  { return topLevelAt(QPoint(x, y)); }

    static void beep();
    static void alert(QWidget *widget, int duration = 0);

    static void setCursorFlashTime(int);
    static int cursorFlashTime();

    static void setDoubleClickInterval(int);
    static int doubleClickInterval();

    static void setKeyboardInputInterval(int);
    static int keyboardInputInterval();

#if BOBUI_CONFIG(wheelevent)
    static void setWheelScrollLines(int);
    static int wheelScrollLines();
#endif

    static void setStartDragTime(int ms);
    static int startDragTime();
    static void setStartDragDistance(int l);
    static int startDragDistance();

    static bool isEffectEnabled(BobUI::UIEffect);
    static void setEffectEnabled(BobUI::UIEffect, bool enable = true);

    static int exec();
    bool notify(QObject *, QEvent *) override;

#ifdef BOBUI_KEYPAD_NAVIGATION
    static void setNavigationMode(BobUI::NavigationMode mode);
    static BobUI::NavigationMode navigationMode();
#endif

    BOBUI_DECLARE_NATIVE_INTERFACE_ACCESSOR(QApplication)

Q_SIGNALS:
    void focusChanged(QWidget *old, QWidget *now);

public:
    QString styleSheet() const;
    bool autoSipEnabled() const;
public Q_SLOTS:
#ifndef BOBUI_NO_STYLE_STYLESHEET
    void setStyleSheet(const QString& sheet);
#endif
    void setAutoSipEnabled(const bool enabled);
    static void closeAllWindows();
    static void aboutBobUI();

protected:
    bool event(QEvent *) override;
#  if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    BOBUI_DEPRECATED_VERSION_X_6_10("This feature will be removed in BobUI 7")
    bool compressEvent(QEvent *, QObject *receiver, QPostEventList *) override;
#  endif

private:
    Q_DISABLE_COPY(QApplication)
    Q_DECLARE_PRIVATE(QApplication)

    friend class QGraphicsWidget;
    friend class QGraphicsItem;
    friend class QGraphicsScene;
    friend class QGraphicsScenePrivate;
    friend class QWidget;
    friend class QWidgetPrivate;
    friend class QWidgetWindow;
    friend class BOBUIranslator;
    friend class QWidgetAnimator;
#ifndef BOBUI_NO_SHORTCUT
    friend class QShortcut;
    friend class QLineEdit;
    friend class QWidgetTextControl;
#endif
    friend class QAction;

#ifndef BOBUI_NO_GESTURES
    friend class QGestureManager;
#endif
};

BOBUI_END_NAMESPACE

#endif // QAPPLICATION_H
