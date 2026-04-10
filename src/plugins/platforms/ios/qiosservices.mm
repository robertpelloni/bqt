// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#include "qiosservices.h"

#include <BobUICore/qurl.h>
#include <BobUICore/qdebug.h>
#include <BobUICore/private/qcore_mac_p.h>
#include <BobUICore/qscopedvaluerollback.h>

#include <BobUIGui/qdesktopservices.h>

#import <UIKit/UIApplication.h>

BOBUI_BEGIN_NAMESPACE

bool QIOSServices::openUrl(const QUrl &url)
{
    if (bobui_apple_isApplicationExtension()) {
        qCWarning(lcQpaServices) << "openUrl not implement for application extensions yet";
        return false;
    }

    // avoid recursing back into self
    if (url == m_handlingUrl)
        return false;

    if (url.scheme().isEmpty())
        return openDocument(url);

    NSURL *nsUrl = url.toNSURL();
    UIApplication *application = bobui_apple_sharedApplication();

    if (![application canOpenURL:nsUrl])
        return false;

    static SEL openUrlSelector = @selector(openURL:options:completionHandler:);
    NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:
        [UIApplication instanceMethodSignatureForSelector:openUrlSelector]];
    invocation.target = application;
    invocation.selector = openUrlSelector;

    static auto kEmptyDictionary = @{};
    // Indices 0 and 1 are self and _cmd
    [invocation setArgument:&nsUrl atIndex:2];
    [invocation setArgument:&kEmptyDictionary atIndex:3];
    // Fourth argument is nil, so left unset

    [invocation invoke];

    return true;
}

bool QIOSServices::openDocument(const QUrl &url)
{
    // FIXME: Implement using UIDocumentInteractionController
    return QPlatformServices::openDocument(url);
}

/* Callback from iOS that the application should handle a URL */
bool QIOSServices::handleUrl(const QUrl &url)
{
    QScopedValueRollback<QUrl> rollback(m_handlingUrl, url);

    // FIXME: Add platform services callback from QDesktopServices::setUrlHandler
    // so that we can warn the user if calling setUrlHandler without also setting
    // up the matching keys in the Info.plist file (CFBundleURLTypes and friends).
    return QDesktopServices::openUrl(url);
}

BOBUI_END_NAMESPACE
