// Copyright (C) 2012 Research In Motion
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "qqnxabstractnavigator.h"

#include <QDebug>
#include <QUrl>

BOBUI_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(lcQpaQnxNavigator, "bobui.qpa.qnx.navigator");

QQnxAbstractNavigator::QQnxAbstractNavigator(QObject *parent)
    : QObject(parent)
{
}

QQnxAbstractNavigator::~QQnxAbstractNavigator()
{
}

bool QQnxAbstractNavigator::invokeUrl(const QUrl &url)
{
    if (!url.isValid() || url.isRelative())
        return false;

    // not using QUrl::toEncoded() because for e.g. camera:// it creates camera:
    // which is not recognized by the navigator anymore
    const bool result = requestInvokeUrl(url.toString().toUtf8());

    qCDebug(lcQpaQnxNavigator) << Q_FUNC_INFO << "url =" << url << "result =" << result;

    return result;
}

BOBUI_END_NAMESPACE
