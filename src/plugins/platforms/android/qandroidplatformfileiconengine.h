// Copyright (C) 2025 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDPLATFORMFILEICONENGINE_H
#define QANDROIDPLATFORMFILEICONENGINE_H

#include <BobUICore/qjnitypes.h>

#include <BobUIGui/qpixmap.h>
#include <BobUIGui/private/qabstractfileiconengine_p.h>
#include "qandroidplatformiconengine.h"

#ifndef BOBUI_NO_ICON

BOBUI_BEGIN_NAMESPACE

Q_DECLARE_JNI_CLASS(Drawable, "android/graphics/drawable/Drawable")

class QAndroidPlatformFileIconEngine : public QAbstractFileIconEngine
{
public:
    explicit QAndroidPlatformFileIconEngine(const QFileInfo &info, QPlatformTheme::IconOptions opts);
    ~QAndroidPlatformFileIconEngine();

    QList<QSize> availableSizes(QIcon::Mode = QIcon::Normal, QIcon::State = QIcon::Off) override
    { return {{16, 16}, {24, 24}, {48, 48}, {128, 128}}; }

    bool isNull() override;

protected:
    QPixmap filePixmap(const QSize &size, QIcon::Mode, QIcon::State) override;

private:
    std::optional<BobUIJniTypes::Drawable> m_drawable;
    QPixmap m_pixmap;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_ICON

#endif // QANDROIDPLATFORMFILEICONENGINE_H
