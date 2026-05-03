// Copyright (C) 2016 The BobUI Company Ltd.
// Copyright (C) 2024 Jie Liu <liujie01@kylinos.cn>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDDISPLAY_H
#define QWAYLANDDISPLAY_H

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

#include <BobUICore/QList>
#include <BobUICore/QObject>
#include <BobUICore/QPointer>
#include <BobUICore/QRect>
#include <BobUICore/QMutex>
#include <BobUICore/QReadWriteLock>

#include <BobUICore/QWaitCondition>
#include <BobUICore/QLoggingCategory>

#include <BobUIWaylandClient/private/qwayland-wayland.h>
#include <BobUIWaylandClient/private/bobuiwaylandclientglobal_p.h>
#include <BobUIWaylandClient/private/qwaylandshm_p.h>

#include <qpa/qplatforminputcontextfactory_p.h>

#if BOBUI_CONFIG(xkbcommon)
#include <BobUIGui/private/qxkbcommon_p.h>
#endif

struct wl_cursor_image;
struct wp_viewport;

BOBUI_BEGIN_NAMESPACE

#define WAYLAND_IM_KEY "wayland"

class QAbstractEventDispatcher;
class QSocketNotifier;
class QPlatformScreen;
class QPlatformPlaceholderScreen;

namespace BobUIWayland {
    class zwp_text_input_manager_v1;
    class zwp_text_input_manager_v2;
    class zwp_text_input_manager_v3;
    class bobui_text_input_method_manager_v1;
    class wp_cursor_shape_manager_v1;
    class wp_fractional_scale_manager_v1;
    class wp_viewporter;
    class xx_session_manager_v1;
    class xdg_system_bell_v1;
    class xdg_toplevel_drag_manager_v1;
    class wp_pointer_warp_v1;
}

namespace BobUIWaylandClient {

BOBUI_DECLARE_EXPORTED_BOBUI_LOGGING_CATEGORY(lcQpaWayland, Q_WAYLANDCLIENT_EXPORT);

class QWaylandAppMenuManager;
class QWaylandInputDevice;
class QWaylandBuffer;
class QWaylandScreen;
class QWaylandXdgOutputManagerV1;
class QWaylandClientBufferIntegration;
class QWaylandWindowManagerIntegration;
class QWaylandDataDeviceManager;
#if BOBUI_CONFIG(clipboard)
class QWaylandDataControlManagerV1;
#endif
#if BOBUI_CONFIG(wayland_client_primary_selection)
class QWaylandPrimarySelectionDeviceManagerV1;
#endif
#if BOBUI_CONFIG(tabletevent)
class QWaylandTabletManagerV2;
#endif
class QWaylandPointerGestures;
class QWaylandWindow;
class QWaylandIntegration;
class QWaylandHardwareIntegration;
class QWaylandSessionManager;
class QWaylandSurface;
class QWaylandShellIntegration;
class QWaylandCursor;
class QWaylandCursorTheme;
class EventThread;
class ColorManager;

typedef void (*RegistryListener)(void *data,
                                 struct wl_registry *registry,
                                 uint32_t id,
                                 const QString &interface,
                                 uint32_t version);

class Q_WAYLANDCLIENT_EXPORT QWaylandDisplay : public QObject, public BobUIWayland::wl_registry {
    Q_OBJECT

public:
    QWaylandDisplay(QWaylandIntegration *waylandIntegration);
    ~QWaylandDisplay(void) override;

    bool initialize();

#if BOBUI_CONFIG(xkbcommon)
    struct xkb_context *xkbContext() const { return mXkbContext.get(); }
#endif

    QList<QWaylandScreen *> screens() const { return mScreens; }
    QPlatformPlaceholderScreen *placeholderScreen() const { return mPlaceholderScreen; }
    void ensureScreen();

    QWaylandScreen *screenForOutput(struct wl_output *output) const;
    void handleScreenInitialized(QWaylandScreen *screen);

    struct wl_surface *createSurface(void *handle);
    struct ::wl_region *createRegion(const QRegion &qregion);
    struct ::wl_subsurface *createSubSurface(QWaylandWindow *window, QWaylandWindow *parent);
    struct ::wp_viewport *createViewport(QWaylandWindow *window);

    QWaylandShellIntegration *shellIntegration() const;
    QWaylandClientBufferIntegration *clientBufferIntegration() const;
    QWaylandWindowManagerIntegration *windowManagerIntegration() const;

#if BOBUI_CONFIG(cursor)
    QWaylandCursor *waylandCursor();
    QWaylandCursorTheme *loadCursorTheme(const QString &name, int pixelSize);
#endif
    struct wl_display *wl_display() const
    {
        return mDisplay;
    }
    struct ::wl_registry *wl_registry() { return object(); }

    BobUIWayland::wl_compositor *compositor()
    {
        return mGlobals.compositor.get();
    }

    QList<QWaylandInputDevice *> inputDevices() const { return mInputDevices; }
    QWaylandInputDevice *defaultInputDevice() const;
    QWaylandInputDevice *currentInputDevice() const { return defaultInputDevice(); }
#if BOBUI_CONFIG(wayland_datadevice)
    QWaylandDataDeviceManager *dndSelectionHandler() const
    {
        return mGlobals.dndSelectionHandler.get();
    }
#endif
#if BOBUI_CONFIG(clipboard)
    QWaylandDataControlManagerV1 *dataControlManager() const
    {
        return mGlobals.dataControlManager.get();
    }
#endif
#if BOBUI_CONFIG(wayland_client_primary_selection)
    QWaylandPrimarySelectionDeviceManagerV1 *primarySelectionManager() const
    {
        return mGlobals.primarySelectionManager.get();
    }
#endif
#if BOBUI_CONFIG(tabletevent)
    QWaylandTabletManagerV2 *tabletManager() const
    {
        return mGlobals.tabletManager.get();
    }
#endif
    QWaylandPointerGestures *pointerGestures() const
    {
        return mGlobals.pointerGestures.get();
    }
    BobUIWayland::bobui_text_input_method_manager_v1 *textInputMethodManager() const
    {
        return mGlobals.textInputMethodManager.get();
    }
    BobUIWayland::zwp_text_input_manager_v1 *textInputManagerv1() const
    {
        return mGlobals.textInputManagerv1.get();
    }
    BobUIWayland::zwp_text_input_manager_v2 *textInputManagerv2() const
    {
        return mGlobals.textInputManagerv2.get();
    }
    BobUIWayland::zwp_text_input_manager_v3 *textInputManagerv3() const
    {
        return mGlobals.textInputManagerv3.get();
    }
    QWaylandHardwareIntegration *hardwareIntegration() const
    {
        return mGlobals.hardwareIntegration.get();
    }
    QWaylandXdgOutputManagerV1 *xdgOutputManager() const
    {
        return mGlobals.xdgOutputManager.get();
    }
    BobUIWayland::wp_fractional_scale_manager_v1 *fractionalScaleManager() const
    {
        return mGlobals.fractionalScaleManager.get();
    }
    BobUIWayland::wp_viewporter *viewporter() const
    {
        return mGlobals.viewporter.get();
    }
    BobUIWayland::wp_cursor_shape_manager_v1 *cursorShapeManager() const
    {
        return mGlobals.cursorShapeManager.get();
    }
    BobUIWayland::xdg_toplevel_drag_manager_v1 *xdgToplevelDragManager() const
    {
        return mGlobals.xdgToplevelDragManager.get();
    }
    BobUIWayland::xx_session_manager_v1 *xxSessionManager() const
    {
        return mGlobals.xxSessionManager.get();
    }
    BobUIWayland::xdg_system_bell_v1 *systemBell() const
    {
        return mGlobals.systemBell.get();
    }
    QWaylandAppMenuManager *appMenuManager() const
    {
        return mGlobals.appMenuManager.get();
    }
    ColorManager *colorManager() const
    {
        return mGlobals.colorManager.get();
    }
    BobUIWayland::wp_pointer_warp_v1 *pointerWarp() const
    {
        return mGlobals.pointerWarp.get();
    }

    struct RegistryGlobal {
        uint32_t id;
        QString interface;
        uint32_t version;
        struct ::wl_registry *registry = nullptr;
        RegistryGlobal(uint32_t id_, const QString &interface_, uint32_t version_, struct ::wl_registry *registry_)
            : id(id_), interface(interface_), version(version_), registry(registry_) { }
    };
    QList<RegistryGlobal> globals() const
    {
        return mRegistryGlobals;
    }
    bool hasRegistryGlobal(QStringView interfaceName) const;

    /* wl_registry_add_listener does not add but rather sets a listener, so this function is used
     * to enable many listeners at once. */
    void addRegistryListener(RegistryListener listener, void *data);
    void removeListener(RegistryListener listener, void *data);

    QWaylandShm *shm() const
    {
        return mGlobals.shm.get();
    }

    void forceRoundTrip();

    bool supportsWindowDecoration() const;

    uint32_t lastInputSerial() const { return mLastInputSerial; }
    QWaylandInputDevice *lastInputDevice() const { return mLastInputDevice; }
    QWaylandWindow *lastInputWindow() const;
    void setLastInputDevice(QWaylandInputDevice *device, uint32_t serial, QWaylandWindow *window);

    bool isWindowActivated(const QWaylandWindow *window);
    void handleWindowActivated(QWaylandWindow *window);
    void handleWindowDeactivated(QWaylandWindow *window);
    void handleKeyboardFocusChanged(QWaylandInputDevice *inputDevice);
    void handleWindowDestroyed(QWaylandWindow *window);

    wl_event_queue *frameEventQueue() { return m_frameEventQueue; };

    bool isKeyboardAvailable() const;
    bool isWaylandInputContextRequested() const;

    void initEventThread();

public Q_SLOTS:
    void flushRequests();

Q_SIGNALS:
    void connected();
    void globalAdded(const RegistryGlobal &global);
    void globalRemoved(const RegistryGlobal &global);

private:
    void checkWaylandError();
    void reconnect();
    void setupConnection();
    void handleWaylandSync();
    void requestWaylandSync();

    void checkTextInputProtocol();

    struct Listener {
        Listener() = default;
        Listener(RegistryListener incomingListener,
                 void* incomingData)
        : listener(incomingListener), data(incomingData)
        {}
        RegistryListener listener = nullptr;
        void *data = nullptr;
    };
    struct wl_display *mDisplay = nullptr;
    std::unique_ptr<EventThread> m_eventThread;
    wl_event_queue *m_frameEventQueue = nullptr;
    QScopedPointer<EventThread> m_frameEventQueueThread;
    QList<QWaylandScreen *> mWaitingScreens;
    QList<QWaylandScreen *> mScreens;
    QPlatformPlaceholderScreen *mPlaceholderScreen = nullptr;
    QList<QWaylandInputDevice *> mInputDevices;
    QList<Listener> mRegistryListeners;
    QWaylandIntegration *mWaylandIntegration = nullptr;
#if BOBUI_CONFIG(cursor)
    struct WaylandCursorTheme {
        QString name;
        int pixelSize;
        std::unique_ptr<QWaylandCursorTheme> theme;
    };
    std::vector<WaylandCursorTheme> mCursorThemes;

    struct FindExistingCursorThemeResult {
        std::vector<WaylandCursorTheme>::const_iterator position;
        bool found;

        QWaylandCursorTheme *theme() const noexcept
        { return found ? position->theme.get() : nullptr; }
    };
    FindExistingCursorThemeResult findExistingCursorTheme(const QString &name,
                                                          int pixelSize) const noexcept;
    QScopedPointer<QWaylandCursor> mCursor;
#endif

    struct GlobalHolder
    {
        std::unique_ptr<BobUIWayland::wl_compositor> compositor;
        std::unique_ptr<QWaylandShm> shm;
#if BOBUI_CONFIG(wayland_datadevice)
        std::unique_ptr<QWaylandDataDeviceManager> dndSelectionHandler;
#endif
        std::unique_ptr<BobUIWayland::wl_subcompositor> subCompositor;
#if BOBUI_CONFIG(tabletevent)
        std::unique_ptr<QWaylandTabletManagerV2> tabletManager;
#endif
        std::unique_ptr<QWaylandPointerGestures> pointerGestures;
#if BOBUI_CONFIG(clipboard)
        std::unique_ptr<QWaylandDataControlManagerV1> dataControlManager;
#endif
#if BOBUI_CONFIG(wayland_client_primary_selection)
        std::unique_ptr<QWaylandPrimarySelectionDeviceManagerV1> primarySelectionManager;
#endif
        std::unique_ptr<BobUIWayland::bobui_text_input_method_manager_v1> textInputMethodManager;
        std::unique_ptr<BobUIWayland::zwp_text_input_manager_v1> textInputManagerv1;
        std::unique_ptr<BobUIWayland::zwp_text_input_manager_v2> textInputManagerv2;
        std::unique_ptr<BobUIWayland::zwp_text_input_manager_v3> textInputManagerv3;
        std::unique_ptr<QWaylandHardwareIntegration> hardwareIntegration;
        std::unique_ptr<QWaylandXdgOutputManagerV1> xdgOutputManager;
        std::unique_ptr<BobUIWayland::wp_viewporter> viewporter;
        std::unique_ptr<BobUIWayland::wp_fractional_scale_manager_v1> fractionalScaleManager;
        std::unique_ptr<BobUIWayland::wp_cursor_shape_manager_v1> cursorShapeManager;
        std::unique_ptr<BobUIWayland::xx_session_manager_v1> xxSessionManager;
        std::unique_ptr<BobUIWayland::xdg_system_bell_v1> systemBell;
        std::unique_ptr<BobUIWayland::xdg_toplevel_drag_manager_v1> xdgToplevelDragManager;
        std::unique_ptr<QWaylandWindowManagerIntegration> windowManagerIntegration;
        std::unique_ptr<QWaylandAppMenuManager> appMenuManager;
        std::unique_ptr<ColorManager> colorManager;
        std::unique_ptr<BobUIWayland::wp_pointer_warp_v1> pointerWarp;
    } mGlobals;

    int mFd = -1;
    int mWritableNotificationFd = -1;
    QList<RegistryGlobal> mRegistryGlobals;
    uint32_t mLastInputSerial = 0;
    QWaylandInputDevice *mLastInputDevice = nullptr;
    QPointer<QWaylandWindow> mLastInputWindow;
    QPointer<QWaylandWindow> mLastKeyboardFocus;
    QList<QWaylandWindow *> mActiveWindows;
    struct wl_callback *mSyncCallback = nullptr;
    static const wl_callback_listener syncCallbackListener;
    bool mWaylandTryReconnect = false;
    bool mPreferWlrDataControl = false;

    bool mWaylandInputContextRequested = [] () {
        const auto requested = QPlatformInputContextFactory::requested();
        return requested.isEmpty() || requested.contains(QLatin1String(WAYLAND_IM_KEY));
    }();
    QStringList mTextInputManagerList;
    int mTextInputManagerIndex = INT_MAX;

    void registry_global(uint32_t id, const QString &interface, uint32_t version) override;
    void registry_global_remove(uint32_t id) override;

#if BOBUI_CONFIG(xkbcommon)
    QXkbCommon::ScopedXKBContext mXkbContext;
#endif

    friend class QWaylandIntegration;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDDISPLAY_H
