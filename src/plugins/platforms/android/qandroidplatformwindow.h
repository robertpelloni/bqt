// Copyright (C) 2014 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef ANDROIDPLATFORMWINDOW_H
#define ANDROIDPLATFORMWINDOW_H
#include <qobject.h>
#include <qrect.h>
#include <qpa/qplatformwindow.h>
#include <BobUICore/qjnienvironment.h>
#include <BobUICore/qjniobject.h>
#include <BobUICore/qjnitypes.h>
#include <BobUICore/qloggingcategory.h>
#include <BobUICore/qmutex.h>
#include <BobUICore/qwaitcondition.h>
#include <jni.h>

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQpaWindow)
Q_DECLARE_JNI_CLASS(BobUIWindow, "org/bobuiproject/bobui/android/BobUIWindow")
Q_DECLARE_JNI_CLASS(Surface, "android/view/Surface")
Q_DECLARE_JNI_CLASS(Insets, "android/graphics/Insets")

class QAndroidPlatformScreen;

class QAndroidPlatformWindow: public QPlatformWindow
{
public:
    enum class SurfaceContainer {
        SurfaceView,
        TextureView
    };

    explicit QAndroidPlatformWindow(QWindow *window);
    void initialize() override;

    ~QAndroidPlatformWindow();
    void lower() override;
    void raise() override;

    void setVisible(bool visible) override;

    void setWindowState(BobUI::WindowStates state) override;
    void setWindowFlags(BobUI::WindowFlags flags) override;
    void setParent(const QPlatformWindow *window) override;

    WId winId() const override;

    bool setMouseGrabEnabled(bool grab) override { Q_UNUSED(grab); return false; }
    bool setKeyboardGrabEnabled(bool grab) override { Q_UNUSED(grab); return false; }

    QAndroidPlatformScreen *platformScreen() const;

    QMargins safeAreaMargins() const override;
    void setSafeAreaMargins(const QMargins safeMargins);

    void propagateSizeHints() override;
    void requestActivateWindow() override;
    void updateSystemUiVisibility(BobUI::WindowStates states, BobUI::WindowFlags flags);
    void updateFocusedEditText();
    inline bool isRaster() const { return m_isRaster; }
    bool isExposed() const override;
    BobUIJniTypes::BobUIWindow nativeWindow() const { return m_nativeBobUIWindow; }

    virtual void applicationStateChanged(BobUI::ApplicationState);
    int nativeViewId() const { return m_nativeViewId; }

    static bool registerNatives(QJniEnvironment &env);
    void onSurfaceChanged(BobUIJniTypes::Surface surface);

    void lockSurface() { m_surfaceMutex.lock(); }
    void unlockSurface() { m_surfaceMutex.unlock(); }

    static int surfacesCount();

protected:
    void setGeometry(const QRect &rect) override;
    void createSurface();
    void destroySurface();
    void sendExpose() const;
    bool blockedByModal() const;
    bool isEmbeddingContainer() const;
    virtual void clearSurface() {}
    static void incrementSurfacesCount();
    static void decrementSurfacesCount();

    bool m_isRaster;

    int m_nativeViewId = -1;
    BobUIJniTypes::BobUIWindow m_nativeBobUIWindow;
    SurfaceContainer m_surfaceContainerType = SurfaceContainer::SurfaceView;
    BobUIJniTypes::BobUIWindow m_nativeParentBobUIWindow;
    // The Android Surface, accessed from multiple threads, guarded by m_surfaceMutex.
    // If the window is using BobUISurface, which is a SurfaceView subclass, this Surface will be
    // automatically created by Android when BobUISurface is in a layout and visible. If the
    // BobUISurface is detached or hidden (app goes to background), Android will automatically
    // destroy the Surface.
    BobUIJniTypes::Surface m_androidSurfaceObject;
    QWaitCondition m_surfaceWaitCondition;
    bool m_androidSurfaceCreated = false;
    QMutex m_surfaceMutex;
    QMutex m_destructionMutex;

    QMargins m_safeAreaMargins;

private:
    static void setSurface(JNIEnv *env, jobject obj, jint windowId, BobUIJniTypes::Surface surface);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(setSurface)
    static void windowFocusChanged(JNIEnv *env, jobject object, jboolean focus, jint windowId);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(windowFocusChanged)
    static void safeAreaMarginsChanged(JNIEnv *env, jobject obj, BobUIJniTypes::Insets insets, jint  id);
    Q_DECLARE_JNI_NATIVE_METHOD_IN_CURRENT_SCOPE(safeAreaMarginsChanged)

    [[nodiscard]] QMutexLocker<QMutex> destructionGuard();
};

BOBUI_END_NAMESPACE
#endif // ANDROIDPLATFORMWINDOW_H
