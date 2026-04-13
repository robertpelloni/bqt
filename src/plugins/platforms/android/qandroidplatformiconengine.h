// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QANDROIDPLATFORMICONENGINE_H
#define QANDROIDPLATFORMICONENGINE_H

#include <BobUIGui/private/qfonticonengine_p.h>

#ifndef BOBUI_NO_ICON

BOBUI_BEGIN_NAMESPACE

class QAndroidPlatformIconEngine : public QFontIconEngine
{
public:
    QAndroidPlatformIconEngine(const QString &iconName);
    ~QAndroidPlatformIconEngine();

    QString key() const override;
    QIconEngine *clone() const override;

protected:
    QString string() const override;

private:
    const QString m_glyphs;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_ICON

#endif // QANDROIDPLATFORMICONENGINE_H
