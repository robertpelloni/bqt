// Copyright (C) 2011 - 2012 Research In Motion
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QQNXSERVICES_H
#define QQNXSERVICES_H

#include <qpa/qplatformservices.h>

BOBUI_BEGIN_NAMESPACE

class QQnxAbstractNavigator;

class QQnxServices : public QPlatformServices
{
public:
    explicit QQnxServices(QQnxAbstractNavigator *navigator);
    ~QQnxServices();

    bool openUrl(const QUrl &url) override;
    bool openDocument(const QUrl &url) override;

private:
    bool navigatorInvoke(const QUrl &url);

private:
    QQnxAbstractNavigator *m_navigator;
};

BOBUI_END_NAMESPACE

#endif // QQNXSERVICES_H
