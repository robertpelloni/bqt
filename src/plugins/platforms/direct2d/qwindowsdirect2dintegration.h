// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSDIRECT2DINTEGRATION_H
#define QWINDOWSDIRECT2DINTEGRATION_H

#include "qwindowsintegration.h"

#include <BobUICore/qscopedpointer.h>

BOBUI_BEGIN_NAMESPACE

class QWindowsDirect2DContext;
class QWindowsDirect2DIntegrationPrivate;

class QWindowsDirect2DIntegration : public QWindowsIntegration
{
public:
    static QWindowsDirect2DIntegration *create(const QStringList &paramList);

    virtual ~QWindowsDirect2DIntegration();

    static QWindowsDirect2DIntegration *instance();

    QPlatformNativeInterface *nativeInterface() const override;
    QPlatformPixmap *createPlatformPixmap(QPlatformPixmap::PixelType type) const override;
    QPlatformBackingStore *createPlatformBackingStore(QWindow *window) const override;

    QWindowsDirect2DContext *direct2DContext() const;

protected:
    QWindowsWindow *createPlatformWindowHelper(QWindow *window, const QWindowsWindowData &) const override;

private:
    explicit QWindowsDirect2DIntegration(const QStringList &paramList);
    bool init();

    QScopedPointer<QWindowsDirect2DIntegrationPrivate> d;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSDIRECT2DINTEGRATION_H
