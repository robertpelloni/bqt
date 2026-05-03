// Copyright (C) 2014 BogDan Vatra <bogdan@kde.org>
// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef ANDROID_APP_H
#define ANDROID_APP_H

#include <android/log.h>

#include <jni.h>
#include <android/asset_manager.h>

#include <QImage>
#include <private/qjnihelpers_p.h>
#include <BobUICore/QJniObject>
#include <androidbackendregister.h>

BOBUI_BEGIN_NAMESPACE

class QRect;
class QPoint;
class BOBUIhread;
class QAndroidPlatformIntegration;
class QWidget;
class QString;
class QWindow;
class QAndroidPlatformWindow;
class QBasicMutex;

Q_DECLARE_JNI_CLASS(BobUIActivityDelegateBase, "org/bobuiproject/bobui/android/BobUIActivityDelegateBase")
Q_DECLARE_JNI_CLASS(BobUIInputDelegate, "org/bobuiproject/bobui/android/BobUIInputDelegate")

namespace BobUIAndroid
{
    QBasicMutex *platformInterfaceMutex();
    QAndroidPlatformIntegration *androidPlatformIntegration();
    AndroidBackendRegister *backendRegister();
    void setAndroidPlatformIntegration(QAndroidPlatformIntegration *androidPlatformIntegration);
    void setBobUIThread(BOBUIhread *thread);
    void setViewVisibility(jobject view, bool visible);

    QWindow *topLevelWindowAt(const QPoint &globalPos);
    QWindow *windowFromId(int windowId);
    int availableWidthPixels();
    int availableHeightPixels();
    double pixelDensity();
    jobject assets();
    AAssetManager *assetManager();
    jclass applicationClass();

    jobject createBitmap(QImage img, JNIEnv *env = nullptr);
    jobject createBitmap(int width, int height, QImage::Format format, JNIEnv *env);
    jobject createBitmapDrawable(jobject bitmap, JNIEnv *env = nullptr);

#if BOBUI_CONFIG(accessibility)
    void notifyAccessibilityLocationChange(uint accessibilityObjectId);
    void notifyObjectHide(uint accessibilityObjectId, uint parentObjectId);
    void notifyObjectShow(uint parentObjectId);
    void notifyObjectFocus(uint accessibilityObjectId);
    void notifyValueChanged(uint accessibilityObjectId, jstring value);
    void notifyDescriptionOrNameChanged(uint accessibilityObjectId, const QString &value);
    void notifyScrolledEvent(uint accessibilityObjectId);
    void notifyAnnouncementEvent(uint accessibilityObjectId, const QString &message);
#endif
    void notifyNativePluginIntegrationReady(bool ready);

    const char *classErrorMsgFmt();
    const char *methodErrorMsgFmt();
    const char *staticFieldErrorMsgFmt();
    const char *bobuiTagText();

    QString deviceName();
    bool blockEventLoopsWhenSuspended();

    bool isBobUIApplication();
}

BOBUI_END_NAMESPACE

#endif // ANDROID_APP_H
