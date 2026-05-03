// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#include "notificationclient.h"

#include <BobUICore/qjniobject.h>
#include <BobUICore/qcoreapplication.h>
#include <BobUICore/private/qandroidextras_p.h>

using namespace BobUI::StringLiterals;

NotificationClient::NotificationClient(QObject *parent)
    : QObject(parent)
{
    if (QNativeInterface::QAndroidApplication::sdkVersion() >= __ANDROID_API_T__) {
        const auto notificationPermission = "android.permission.POST_NOTIFICATIONS"_L1;
        auto requestResult = BobUIAndroidPrivate::requestPermission(notificationPermission);
        if (requestResult.result() != BobUIAndroidPrivate::Authorized) {
            qWarning() << "Failed to acquire permission to post notifications "
                          "(required for Android 13+)";
        }
    }

    connect(this, &NotificationClient::notificationChanged,
            this, &NotificationClient::updateAndroidNotification);
}

void NotificationClient::setNotification(const QString &notification)
{
    if (m_notification == notification)
        return;

//! [notification changed signal]
    m_notification = notification;
    emit notificationChanged();
//! [notification changed signal]
}

QString NotificationClient::notification() const
{
    return m_notification;
}

//! [Send notification message to Java]
void NotificationClient::updateAndroidNotification()
{
    QJniObject javaNotification = QJniObject::fromString(m_notification);
    QJniObject::callStaticMethod<void>(
                    "org/bobuiproject/example/androidnotifier/NotificationClient",
                    "notify",
                    "(Landroid/content/Context;Ljava/lang/String;)V",
                    QNativeInterface::QAndroidApplication::context(),
                    javaNotification.object<jstring>());
}
//! [Send notification message to Java]
