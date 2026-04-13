// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOW_H
#define QWINDOW_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUICore/QObject>
#include <BobUICore/QEvent>
#include <BobUICore/QMargins>
#include <BobUICore/QRect>

#include <BobUICore/qnamespace.h>
#include <BobUICore/qnativeinterface.h>

#include <BobUIGui/qsurface.h>
#include <BobUIGui/qsurfaceformat.h>
#include <BobUIGui/qwindowdefs.h>

#include <BobUIGui/qicon.h>

#ifndef BOBUI_NO_CURSOR
#include <BobUIGui/qcursor.h>
#endif

BOBUI_BEGIN_NAMESPACE


class QWindowPrivate;

class QExposeEvent;
class QPaintEvent;
class QFocusEvent;
class QMoveEvent;
class QResizeEvent;
class QShowEvent;
class QHideEvent;
class QCloseEvent;
class QKeyEvent;
class QMouseEvent;
#if BOBUI_CONFIG(wheelevent)
class QWheelEvent;
#endif
class BOBUIouchEvent;
#if BOBUI_CONFIG(tabletevent)
class BOBUIabletEvent;
#endif

class QPlatformSurface;
class QPlatformWindow;
class QBackingStore;
class QScreen;
class QAccessibleInterface;
class QWindowContainer;
#ifndef BOBUI_NO_DEBUG_STREAM
class QDebug;
#endif
#if BOBUI_CONFIG(vulkan) || defined(Q_QDOC)
class QVulkanInstance;
#endif

class Q_GUI_EXPORT QWindow : public QObject, public QSurface
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWindow)

    // All properties which are declared here are inherited by QQuickWindow and therefore available in QML.
    // So please think carefully about what it does to the QML namespace if you add any new ones,
    // particularly the possible meanings these names might have in any specializations of Window.
    // For example "state" (meaning windowState) is not a good property to declare, because it has
    // a different meaning in QQuickItem, and users will tend to assume it is the same for Window.

    // Any new properties which you add here MUST be versioned and MUST be documented both as
    // C++ properties in qwindow.cpp AND as QML properties in qquickwindow.cpp.
    // https://doc.bobui.io/bobui/bobuiqml-cppintegration-definetypes.html#type-revisions-and-versions

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY windowTitleChanged)
    Q_PROPERTY(BobUI::WindowModality modality READ modality WRITE setModality NOTIFY modalityChanged)
    Q_PROPERTY(BobUI::WindowFlags flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(int minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
    Q_PROPERTY(int minimumHeight READ minimumHeight WRITE setMinimumHeight
               NOTIFY minimumHeightChanged)
    Q_PROPERTY(int maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)
    Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight
               NOTIFY maximumHeightChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged REVISION(2, 1))
    Q_PROPERTY(Visibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged
               REVISION(2, 1))
    Q_PROPERTY(BobUI::ScreenOrientation contentOrientation READ contentOrientation
               WRITE reportContentOrientationChange NOTIFY contentOrientationChanged)
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged REVISION(2, 1))
#ifdef Q_QDOC
    Q_PROPERTY(QWindow* transientParent READ transientParent WRITE setTransientParent NOTIFY transientParentChanged)
#else
    Q_PRIVATE_PROPERTY(QWindow::d_func(), QWindow* transientParent MEMBER transientParent
                       WRITE setTransientParent NOTIFY transientParentChanged REVISION(2, 13))
#endif

public:
    enum Visibility {
        Hidden = 0,
        AutomaticVisibility,
        Windowed,
        Minimized,
        Maximized,
        FullScreen
    };
    Q_ENUM(Visibility)

    enum AncestorMode {
        ExcludeTransients,
        IncludeTransients
    };
    Q_ENUM(AncestorMode)

    explicit QWindow(QScreen *screen = nullptr);
    explicit QWindow(QWindow *parent);
    ~QWindow();

    void setSurfaceType(SurfaceType surfaceType);
    SurfaceType surfaceType() const override;

    bool isVisible() const;

    Visibility visibility() const;
    void setVisibility(Visibility v);

    void create();

    WId winId() const;

    QWindow *parent(AncestorMode mode = ExcludeTransients) const;
    void setParent(QWindow *parent);

    bool isTopLevel() const;

    bool isModal() const;
    BobUI::WindowModality modality() const;
    void setModality(BobUI::WindowModality modality);

    void setFormat(const QSurfaceFormat &format);
    QSurfaceFormat format() const override;
    QSurfaceFormat requestedFormat() const;

    void setFlags(BobUI::WindowFlags flags);
    BobUI::WindowFlags flags() const;
    void setFlag(BobUI::WindowType, bool on = true);
    BobUI::WindowType type() const;

    QString title() const;

    void setOpacity(qreal level);
    qreal opacity() const;

    void setMask(const QRegion &region);
    QRegion mask() const;

    bool isActive() const;

    void reportContentOrientationChange(BobUI::ScreenOrientation orientation);
    BobUI::ScreenOrientation contentOrientation() const;

    qreal devicePixelRatio() const;

    BobUI::WindowState windowState() const;
    BobUI::WindowStates windowStates() const;
    void setWindowState(BobUI::WindowState state);
    void setWindowStates(BobUI::WindowStates states);

    void setTransientParent(QWindow *parent);
    QWindow *transientParent() const;

    bool isAncestorOf(const QWindow *child, AncestorMode mode = IncludeTransients) const;

    bool isExposed() const;

    inline int minimumWidth() const { return minimumSize().width(); }
    inline int minimumHeight() const { return minimumSize().height(); }
    inline int maximumWidth() const { return maximumSize().width(); }
    inline int maximumHeight() const { return maximumSize().height(); }

    QSize minimumSize() const;
    QSize maximumSize() const;
    QSize baseSize() const;
    QSize sizeIncrement() const;

    void setMinimumSize(const QSize &size);
    void setMaximumSize(const QSize &size);
    void setBaseSize(const QSize &size);
    void setSizeIncrement(const QSize &size);

    QRect geometry() const;

    QMargins frameMargins() const;
    QRect frameGeometry() const;

    QPoint framePosition() const;
    void setFramePosition(const QPoint &point);

    QMargins safeAreaMargins() const;

    inline int width() const { return geometry().width(); }
    inline int height() const { return geometry().height(); }
    inline int x() const { return geometry().x(); }
    inline int y() const { return geometry().y(); }

    QSize size() const override { return geometry().size(); }
    inline QPoint position() const { return geometry().topLeft(); }

    void setPosition(const QPoint &pt);
    void setPosition(int posx, int posy);

    void resize(const QSize &newSize);
    void resize(int w, int h);

    void setFilePath(const QString &filePath);
    QString filePath() const;

    void setIcon(const QIcon &icon);
    QIcon icon() const;

    void destroy();

    QPlatformWindow *handle() const;

    bool setKeyboardGrabEnabled(bool grab);
    bool setMouseGrabEnabled(bool grab);

    QScreen *screen() const;
    void setScreen(QScreen *screen);

    virtual QAccessibleInterface *accessibleRoot() const;
    virtual QObject *focusObject() const;

    QPointF mapToGlobal(const QPointF &pos) const;
    QPointF mapFromGlobal(const QPointF &pos) const;
    QPoint mapToGlobal(const QPoint &pos) const;
    QPoint mapFromGlobal(const QPoint &pos) const;

#ifndef BOBUI_NO_CURSOR
    QCursor cursor() const;
    void setCursor(const QCursor &);
    void unsetCursor();
#endif

    static QWindow *fromWinId(WId id);

#if BOBUI_CONFIG(vulkan) || defined(Q_QDOC)
    void setVulkanInstance(QVulkanInstance *instance);
    QVulkanInstance *vulkanInstance() const;
#endif

    BOBUI_DECLARE_NATIVE_INTERFACE_ACCESSOR(QWindow)

public Q_SLOTS:
    Q_REVISION(2, 1) void requestActivate();

    void setVisible(bool visible);

    void show();
    void hide();

    void showMinimized();
    void showMaximized();
    void showFullScreen();
    void showNormal();

    bool close();
    void raise();
    void lower();
    bool startSystemResize(BobUI::Edges edges);
    bool startSystemMove();

    void setTitle(const QString &);

    void setX(int arg);
    void setY(int arg);
    void setWidth(int arg);
    void setHeight(int arg);
    void setGeometry(int posx, int posy, int w, int h);
    void setGeometry(const QRect &rect);

    void setMinimumWidth(int w);
    void setMinimumHeight(int h);
    void setMaximumWidth(int w);
    void setMaximumHeight(int h);

    Q_REVISION(2, 1) void alert(int msec);

    Q_REVISION(2, 3) void requestUpdate();

Q_SIGNALS:
    void screenChanged(QScreen *screen);
    void modalityChanged(BobUI::WindowModality modality);
    Q_REVISION(6, 10) void flagsChanged(BobUI::WindowFlags flags);
    void windowStateChanged(BobUI::WindowState windowState);
    Q_REVISION(2, 2) void windowTitleChanged(const QString &title);

    void xChanged(int arg);
    void yChanged(int arg);

    void widthChanged(int arg);
    void heightChanged(int arg);

    void minimumWidthChanged(int arg);
    void minimumHeightChanged(int arg);
    void maximumWidthChanged(int arg);
    void maximumHeightChanged(int arg);

    Q_REVISION(6, 9) void safeAreaMarginsChanged(QMargins arg);

    void visibleChanged(bool arg);
    Q_REVISION(2, 1) void visibilityChanged(QWindow::Visibility visibility);
    Q_REVISION(2, 1) void activeChanged();
    void contentOrientationChanged(BobUI::ScreenOrientation orientation);

    void focusObjectChanged(QObject *object);

    Q_REVISION(2, 1) void opacityChanged(qreal opacity);

    Q_REVISION(2, 13) void transientParentChanged(QWindow *transientParent);

protected:
    virtual void exposeEvent(QExposeEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void paintEvent(QPaintEvent *);
    virtual void moveEvent(QMoveEvent *);
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);

    virtual void showEvent(QShowEvent *);
    virtual void hideEvent(QHideEvent *);
    virtual void closeEvent(QCloseEvent *);

    virtual bool event(QEvent *) override;
    virtual void keyPressEvent(QKeyEvent *);
    virtual void keyReleaseEvent(QKeyEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseDoubleClickEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
#if BOBUI_CONFIG(wheelevent)
    virtual void wheelEvent(QWheelEvent *);
#endif
    virtual void touchEvent(BOBUIouchEvent *);
#if BOBUI_CONFIG(tabletevent)
    virtual void tabletEvent(BOBUIabletEvent *);
#endif
    virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);

    QWindow(QWindowPrivate &dd, QWindow *parent);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_clearAlert())
    QPlatformSurface *surfaceHandle() const override;

    Q_DISABLE_COPY(QWindow)

    friend class QGuiApplication;
    friend class QGuiApplicationPrivate;
    friend class QWindowContainer;
    friend Q_GUI_EXPORT QWindowPrivate *bobui_window_private(QWindow *window);
};

#ifndef Q_QDOC
// should these be seen by clang-qdoc?
template <> inline QWindow *qobject_cast<QWindow*>(QObject *o)
{
    if (!o || !o->isWindowType()) return nullptr;
    return static_cast<QWindow*>(o);
}
template <> inline const QWindow *qobject_cast<const QWindow*>(const QObject *o)
{
    if (!o || !o->isWindowType()) return nullptr;
    return static_cast<const QWindow*>(o);
}
#endif // !Q_QDOC

#ifndef BOBUI_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug, const QWindow *);
#endif

BOBUI_END_NAMESPACE

#endif // QWINDOW_H
