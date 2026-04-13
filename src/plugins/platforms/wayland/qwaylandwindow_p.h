// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDWINDOW_H
#define QWAYLANDWINDOW_H

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

#include <BobUICore/QWaitCondition>
#include <BobUICore/QMutex>
#include <BobUICore/QReadWriteLock>

#include <BobUIGui/QIcon>
#include <BobUIGui/QEventPoint>
#include <BobUICore/QVariant>
#include <BobUICore/QLoggingCategory>
#include <BobUICore/QElapsedTimer>
#include <BobUICore/QList>
#include <BobUICore/QMap> // for QVariantMap

#include <qpa/qplatformwindow.h>
#include <qpa/qplatformwindow_p.h>

#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/qwaylanddisplay_p.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>
#include <BobUIWaylandClient/private/qwaylandshellsurface_p.h>

#include <BobUICore/qpointer.h>

struct wl_egl_window;

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

Q_DECLARE_LOGGING_CATEGORY(lcWaylandBackingstore)

class QWaylandDisplay;
class QWaylandBuffer;
class QWaylandShellSurface;
class QWaylandSubSurface;
class QWaylandAbstractDecoration;
class QWaylandInputDevice;
class QWaylandScreen;
class QWaylandShellIntegration;
class QWaylandShmBackingStore;
class QWaylandPointerEvent;
class QWaylandPointerGestureSwipeEvent;
class QWaylandPointerGesturePinchEvent;
class QWaylandSurface;
class QWaylandFractionalScale;
class QWaylandViewport;
class ColorManagementSurface;
class ImageDescription;

class Q_WAYLANDCLIENT_EXPORT QWaylandWindow : public QNativeInterface::Private::QWaylandWindow,
                                              public QPlatformWindow
{
    Q_OBJECT
public:
    enum WindowType {
        Shm,
        Egl,
        Vulkan
    };

    enum ToplevelWindowTilingState {
        WindowNoState = 0,
        WindowTiledLeft = 1,
        WindowTiledRight = 2,
        WindowTiledTop = 4,
        WindowTiledBottom = 8
    };
    Q_DECLARE_FLAGS(ToplevelWindowTilingStates, ToplevelWindowTilingState)

    QWaylandWindow(QWindow *window, QWaylandDisplay *display);
    ~QWaylandWindow() override;

    // Keep Toplevels position on the top left corner of their screen
    static inline bool fixedToplevelPositions = true;

    virtual WindowType windowType() const = 0;
    virtual void ensureSize();
    WId winId() const override;
    void setVisible(bool visible) override;
    void setParent(const QPlatformWindow *parent) override;

    QString windowTitle() const override;
    void setWindowTitle(const QString &title) override;

    inline QIcon windowIcon() const;
    void setWindowIcon(const QIcon &icon) override;

    void setGeometry(const QRect &rect) override;

    bool allowsIndependentThreadedRendering() const override;

    void resizeFromApplyConfigure(const QSize &sizeWithMargins, const QPoint &offset = {0, 0});
    void repositionFromApplyConfigure(const QPoint &position);
    void setGeometryFromApplyConfigure(const QPoint &globalPosition, const QSize &sizeWithMargins);

    void applyConfigureWhenPossible(); //rename to possible?

    void attach(QWaylandBuffer *buffer, int x, int y);
    void attachOffset(QWaylandBuffer *buffer);
    QPoint attachOffset() const;

    void damage(const QRect &rect);

    void safeCommit(QWaylandBuffer *buffer, const QRegion &damage);
    void commit(QWaylandBuffer *buffer, const QRegion &damage);

    void commit();

    bool waitForFrameSync(int timeout);

    QMargins frameMargins() const override;
    QMargins clientSideMargins() const;
    void setCustomMargins(const QMargins &margins) override;
    QSize surfaceSize() const;
    QMargins windowContentMargins() const;
    QRect windowContentGeometry() const;
    QPointF mapFromWlSurface(const QPointF &surfacePosition) const;

    QWaylandSurface *waylandSurface() const { return mSurface.data(); }
    ::wl_surface *wlSurface() const;
    ::wl_surface *surface() const override
    {
        return wlSurface();
    }
    static QWaylandWindow *fromWlSurface(::wl_surface *surface);

    QWaylandDisplay *display() const { return mDisplay; }
    QWaylandShellSurface *shellSurface() const;
    std::any _surfaceRole() const override;
    QWaylandSubSurface *subSurfaceWindow() const;
    QWaylandScreen *waylandScreen() const;

    void handleContentOrientationChange(BobUI::ScreenOrientation orientation) override;
    void updateBufferTransform();
    void setOrientationMask(BobUI::ScreenOrientations mask);

    ToplevelWindowTilingStates toplevelWindowTilingStates() const;
    void handleToplevelWindowTilingStatesChanged(ToplevelWindowTilingStates states);

    BobUI::WindowStates windowStates() const;
    void setWindowState(BobUI::WindowStates states) override;
    void setWindowFlags(BobUI::WindowFlags flags) override;
    BobUI::WindowFlags windowFlags() const;
    void handleWindowStatesChanged(BobUI::WindowStates states);

    void raise() override;
    void lower() override;

    void setMask(const QRegion &region) override;

    void setAlertState(bool enabled) override;
    bool isAlertState() const override;

    qreal scale() const;
    qreal devicePixelRatio() const override;

    void requestActivateWindow() override;
    bool isExposed() const override;
    bool isActive() const override;

    QWaylandAbstractDecoration *decoration() const;

    void handleMouse(QWaylandInputDevice *inputDevice, const QWaylandPointerEvent &e);
#ifndef BOBUI_NO_GESTURES
    void handleSwipeGesture(QWaylandInputDevice *inputDevice,
                            const QWaylandPointerGestureSwipeEvent &e);
    void handlePinchGesture(QWaylandInputDevice *inputDevice,
                            const QWaylandPointerGesturePinchEvent &e);
#endif

    bool touchDragDecoration(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,
                             QEventPoint::State state, BobUI::KeyboardModifiers mods);
    bool handleTabletEventDecoration(QWaylandInputDevice *inputDevice, const QPointF &local,
                                     const QPointF &global, BobUI::MouseButtons buttons,
                                     BobUI::KeyboardModifiers modifiers);

    virtual bool createDecoration();

#if BOBUI_CONFIG(cursor)
    void restoreMouseCursor(QWaylandInputDevice *device);
    void setStoredCursor(const QCursor &cursor);
    void resetStoredCursor();
    void applyCursor(QWaylandInputDevice *device, const QCursor &cursor);
#endif

    QWaylandWindow *transientParent() const;

    bool setMouseGrabEnabled(bool grab) override;
    static QWaylandWindow *mouseGrab() { return mMouseGrab; }

    void sendProperty(const QString &name, const QVariant &value);
    void setProperty(const QString &name, const QVariant &value);

    QVariantMap properties() const;
    QVariant property(const QString &name);
    QVariant property(const QString &name, const QVariant &defaultValue);

#ifdef BOBUI_PLATFORM_WINDOW_HAS_VIRTUAL_SET_BACKING_STORE
    void setBackingStore(QPlatformBackingStore *store) override;
#else
    void setBackingStore(QWaylandShmBackingStore *backingStore) { mBackingStore = backingStore; }
#endif
    QWaylandShmBackingStore *backingStore() const { return mBackingStore; }

    void setShellIntegration(QWaylandShellIntegration *shellIntegration);
    QWaylandShellIntegration *shellIntegration() const { return mShellIntegration; }

    bool setKeyboardGrabEnabled(bool) override { return false; }
    void propagateSizeHints() override;
    void addAttachOffset(const QPoint point);

    bool startSystemResize(BobUI::Edges edges) override;
    bool startSystemMove() override;

    void timerEvent(BOBUIimerEvent *event) override;
    void requestUpdate() override;
    void handleUpdate();
    void deliverUpdateRequest() override;

    void setXdgActivationToken(const QString &token);
    void requestXdgActivationToken(uint serial) override;

    void beginFrame();
    void endFrame();

    void closeChildPopups();

    // should be invoked whenever a property that potentially affects
    // exposure changes
    void updateExposure();

    virtual void reinit();
    void reset();
    void initializeWlSurface(bool colorSpace = true);

    void initializeColorSpace();

    bool windowEvent(QEvent *event) override;

    QSurfaceFormat format() const override;
    void setSessionRestoreId(const QString &role) override;
    QString sessionRestoreId() const;

    void setExtendedWindowType(QNativeInterface::Private::QWaylandWindow::WindowType) override;
    QNativeInterface::Private::QWaylandWindow::WindowType extendedWindowType() const;
    void setParentControlGeometry(const QRect &parentAnchor) override;
    QRect parentControlGeometry() const;

public Q_SLOTS:
    void applyConfigure();

Q_SIGNALS:
    void wlSurfaceCreated();
    void wlSurfaceDestroyed();

protected:
    virtual void doHandleFrameCallback();
    virtual QRect defaultGeometry() const;
    void setFormat(const QSurfaceFormat &format);

    // this should be called directly for buffer size changes only
    // use updateExposure for anything affecting the on/off state
    void sendExposeEvent(const QRect &rect);

    QWaylandDisplay *mDisplay = nullptr;

    // mSurface can be written by the main thread. Other threads should claim a read lock for access
    mutable QReadWriteLock mSurfaceLock;
    QScopedPointer<QWaylandSurface> mSurface;
    QScopedPointer<QWaylandFractionalScale> mFractionalScale;
    QScopedPointer<QWaylandViewport> mViewport;

    QWaylandShellIntegration *mShellIntegration = nullptr;
    QWaylandShellSurface *mShellSurface = nullptr;
    QWaylandSubSurface *mSubSurfaceWindow = nullptr;
    QList<QWaylandSubSurface *> mChildren;

    std::unique_ptr<QWaylandAbstractDecoration> mWindowDecoration;
    bool mWindowDecorationEnabled = false;
    bool mMouseEventsInContentArea = false;
    BobUI::MouseButtons mMousePressedInContentArea = BobUI::NoButton;

#ifndef BOBUI_NO_GESTURES
    enum GestureState {
        GestureNotActive,
        GestureActiveInContentArea,
        GestureActiveInDecoration
    };

    // We want gestures started in the decoration area to be completely ignored even if the mouse
    // pointer is later moved to content area. Likewise, gestures started in the content area should
    // keep sending events even if the mouse pointer is moved over the decoration (consider that
    // the events for that gesture will be sent to us even if it's moved outside the window).
    // So we track the gesture state and accept or ignore events based on that. Note that
    // concurrent gestures of different types are not allowed in the protocol, so single state is
    // enough
    GestureState mGestureState = GestureNotActive;
#endif

    std::atomic_bool mFrameCallbackTimedOut = false; // Whether the frame callback has timed out
    int mFrameCallbackCheckIntervalTimerId = -1;
    QAtomicInt mWaitingForUpdateDelivery = false;

    QElapsedTimer mFrameCallbackElapsedTimer; // Protected by mFrameSyncMutex
    struct ::wl_callback *mFrameCallback = nullptr; // Protected by mFrameSyncMutex
    QMutex mFrameSyncMutex;
    QWaitCondition mFrameSyncWait;

    bool mExposed = false;
    std::atomic_bool mExposeEventNeedsAttachedBuffer = false;

    // written from the main thread, read by the render thread
    std::atomic_bool mWaitingToApplyConfigure = false;
    // written from the render thread, read by the main thread
    std::atomic_bool mInFrameRender = false;

    int mFrameCallbackTimeout = 100;
    QVariantMap m_properties;

    QPoint mOffset;
    std::optional<qreal> mScale = std::nullopt;

    QString mWindowTitle;
    QIcon mWindowIcon;

    BobUI::WindowFlags mFlags;
    QRegion mMask;

    // Empty QRegion maps to "infinite" input region, needs a dedicated "deliberately empty" state.
    QRegion mInputRegion;
    bool mTransparentInputRegion = false;

    QRegion mOpaqueArea;
    BobUI::WindowStates mLastReportedWindowStates = BobUI::WindowNoState;
    ToplevelWindowTilingStates mLastReportedToplevelWindowTilingStates = WindowNoState;

    QWaylandShmBackingStore *mBackingStore = nullptr;

    QMargins mCustomMargins;
    QString mSessionRestoreId;

    QPointer<QWaylandWindow> mTransientParent;
    QList<QPointer<QWaylandWindow>> mChildPopups;

    BobUI::ScreenOrientation mLastReportedContentOrientation = BobUI::PrimaryOrientation;

    std::unique_ptr<ColorManagementSurface> mColorManagementSurface;
    QSurfaceFormat mSurfaceFormat;

#if BOBUI_CONFIG(cursor)
    QCursor mStoredCursor;
    bool mHasStoredCursor = false;
#endif

private:
    void setGeometry_helper(const QRect &rect);
    void synthesizeExposeOnGeometryChange();
    void initWindow();
    bool shouldCreateShellSurface() const;
    bool shouldCreateSubSurface() const;
    void resetSurfaceRole();
    void resetFrameCallback();
    QPlatformScreen *calculateScreenFromSurfaceEvents() const;
    void setOpaqueArea(const QRegion &opaqueArea);
    bool isOpaque() const;
    void updateInputRegion();
    void updateViewport();
    bool calculateExposure() const;
    void setPendingImageDescription();

    void handleMouseEventWithDecoration(QWaylandInputDevice *inputDevice, const QWaylandPointerEvent &e);
    void handleScreensChanged();
    void updateScale();
    void setScale(qreal newScale);

    QWaylandWindow *guessTransientParent() const;
    void addChildPopup(QWaylandWindow *child);
    void removeChildPopup(QWaylandWindow *child);

    bool mInResizeFromApplyConfigure = false;
    bool lastVisible = false;
    QRect mLastExposeGeometry;
    std::unique_ptr<ImageDescription> mPendingImageDescription;

    static const wl_callback_listener callbackListener;
    void handleFrameCallback(struct ::wl_callback* callback);
    const QPlatformWindow *lastParent = nullptr;

    struct {
        QRect parentControlGeometry;
        QNativeInterface::Private::QWaylandWindow::WindowType extendedWindowType = QNativeInterface::Private::QWaylandWindow::Default;
    } m_popupInfo;

    static QWaylandWindow *mMouseGrab;
    static QWaylandWindow *mTopPopup;

    friend class QWaylandSubSurface;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QWaylandWindow::ToplevelWindowTilingStates)

inline QIcon QWaylandWindow::windowIcon() const
{
    return mWindowIcon;
}

inline QPoint QWaylandWindow::attachOffset() const
{
    return mOffset;
}

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDWINDOW_H
