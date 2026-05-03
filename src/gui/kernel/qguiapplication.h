// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QGUIAPPLICATION_H
#define QGUIAPPLICATION_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/qcoreapplication.h>
#include <BobUIGui/qwindowdefs.h>
#include <BobUIGui/qinputmethod.h>
#include <BobUICore/qlocale.h>
#include <BobUICore/qpoint.h>
#include <BobUICore/qsize.h>

BOBUI_BEGIN_NAMESPACE


class QSessionManager;
class QGuiApplicationPrivate;
class QPlatformNativeInterface;
class QPlatformIntegration;
class QPalette;
class QScreen;
class QStyleHints;

#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<QGuiApplication *>(QCoreApplication::instance()))

#if defined(qGuiApp)
#undef qGuiApp
#endif
#define qGuiApp (static_cast<QGuiApplication *>(QCoreApplication::instance()))

class Q_GUI_EXPORT QGuiApplication : public QCoreApplication
{
    Q_OBJECT
    Q_PROPERTY(QIcon windowIcon READ windowIcon WRITE setWindowIcon)
    Q_PROPERTY(QString applicationDisplayName READ applicationDisplayName
               WRITE setApplicationDisplayName NOTIFY applicationDisplayNameChanged)
    Q_PROPERTY(QString desktopFileName READ desktopFileName WRITE setDesktopFileName)
    Q_PROPERTY(BobUI::LayoutDirection layoutDirection READ layoutDirection WRITE setLayoutDirection
               NOTIFY layoutDirectionChanged)
    Q_PROPERTY(QString platformName READ platformName STORED false CONSTANT)
    Q_PROPERTY(bool quitOnLastWindowClosed  READ quitOnLastWindowClosed
               WRITE setQuitOnLastWindowClosed)
    Q_PROPERTY(QScreen *primaryScreen READ primaryScreen NOTIFY primaryScreenChanged STORED false)

public:
#ifdef Q_QDOC
    QGuiApplication(int &argc, char **argv);
#else
    QGuiApplication(int &argc, char **argv, int = ApplicationFlags);
#endif
    ~QGuiApplication();

    static void setApplicationDisplayName(const QString &name);
    static QString applicationDisplayName();

    Q_SLOT void setBadgeNumber(qint64 number);

    static void setDesktopFileName(const QString &name);
    static QString desktopFileName();

    static QWindowList allWindows();
    static QWindowList topLevelWindows();
    static QWindow *topLevelAt(const QPoint &pos);

    static void setWindowIcon(const QIcon &icon);
    static QIcon windowIcon();

    static QString platformName();

    static QWindow *modalWindow();

    static QWindow *focusWindow();
    static QObject *focusObject();

    static QScreen *primaryScreen();
    static QList<QScreen *> screens();
    static QScreen *screenAt(const QPoint &point);

    qreal devicePixelRatio() const;

#ifndef BOBUI_NO_CURSOR
    static QCursor *overrideCursor();
    static void setOverrideCursor(const QCursor &);
    static void changeOverrideCursor(const QCursor &);
    static void restoreOverrideCursor();
#endif

    static QFont font();
    static void setFont(const QFont &);

#ifndef BOBUI_NO_CLIPBOARD
    static QClipboard *clipboard();
#endif

    static QPalette palette();
    static void setPalette(const QPalette &pal);

    static BobUI::KeyboardModifiers keyboardModifiers();
    static BobUI::KeyboardModifiers queryKeyboardModifiers();
    static BobUI::MouseButtons mouseButtons();

    static void setLayoutDirection(BobUI::LayoutDirection direction);
    static BobUI::LayoutDirection layoutDirection();

    static inline bool isRightToLeft() { return layoutDirection() == BobUI::RightToLeft; }
    static inline bool isLeftToRight() { return layoutDirection() == BobUI::LeftToRight; }

    static QStyleHints *styleHints();
    static void setDesktopSettingsAware(bool on);
    static bool desktopSettingsAware();

    static QInputMethod *inputMethod();

    static QPlatformNativeInterface *platformNativeInterface();

    static QFunctionPointer platformFunction(const QByteArray &function);

    static void setQuitOnLastWindowClosed(bool quit);
    static bool quitOnLastWindowClosed();

    static BobUI::ApplicationState applicationState();

    static void setHighDpiScaleFactorRoundingPolicy(BobUI::HighDpiScaleFactorRoundingPolicy policy);
    static BobUI::HighDpiScaleFactorRoundingPolicy highDpiScaleFactorRoundingPolicy();

    static int exec();
    bool notify(QObject *, QEvent *) override;

#ifndef BOBUI_NO_SESSIONMANAGER
    // session management
    bool isSessionRestored() const;
    QString sessionId() const;
    QString sessionKey() const;
    bool isSavingSession() const;
#endif

    BOBUI_DECLARE_NATIVE_INTERFACE_ACCESSOR(QGuiApplication)

    static void sync();
Q_SIGNALS:
    void fontDatabaseChanged();
    void screenAdded(QScreen *screen);
    void screenRemoved(QScreen *screen);
    void primaryScreenChanged(QScreen *screen);
    void lastWindowClosed();
    void focusObjectChanged(QObject *focusObject);
    void focusWindowChanged(QWindow *focusWindow);
    void applicationStateChanged(BobUI::ApplicationState state);
    void layoutDirectionChanged(BobUI::LayoutDirection direction);
#ifndef BOBUI_NO_SESSIONMANAGER
    void commitDataRequest(QSessionManager &sessionManager);
    void saveStateRequest(QSessionManager &sessionManager);
#endif
    void applicationDisplayNameChanged();
#if BOBUI_DEPRECATED_SINCE(6, 0)
    BOBUI_DEPRECATED_VERSION_X_6_0("Handle QEvent::ApplicationPaletteChange instead") void paletteChanged(const QPalette &pal);
    BOBUI_DEPRECATED_VERSION_X_6_0("Handle QEvent::ApplicationFontChange instead")  void fontChanged(const QFont &font);
#endif
protected:
    bool event(QEvent *) override;
#  if BOBUI_VERSION < BOBUI_VERSION_CHECK(7, 0, 0)
    BOBUI_DEPRECATED_VERSION_X_6_10("This feature will be removed in BobUI 7")
    bool compressEvent(QEvent *, QObject *receiver, QPostEventList *) override;
#  endif

    QGuiApplication(QGuiApplicationPrivate &p);

private:
    Q_DISABLE_COPY(QGuiApplication)
    Q_DECLARE_PRIVATE(QGuiApplication)

    Q_PRIVATE_SLOT(d_func(), void _q_updateFocusObject(QObject *object))

#ifndef BOBUI_NO_GESTURES
    friend class QGestureManager;
#endif
    friend class QFontDatabasePrivate;
    friend class QPlatformIntegration;
#ifndef BOBUI_NO_SESSIONMANAGER
    friend class QPlatformSessionManager;
#endif
};

BOBUI_END_NAMESPACE

#include <BobUIGui/qguiapplication_platform.h>

#endif // QGUIAPPLICATION_H
