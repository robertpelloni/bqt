// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSCREEN_P_H
#define QSCREEN_P_H

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

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include <BobUIGui/qscreen.h>
#include <qpa/qplatformscreen.h>
#include "qhighdpiscaling_p.h"

#include <BobUICore/private/qobject_p.h>

BOBUI_BEGIN_NAMESPACE

struct QScreenData
{
    QPlatformScreen *platformScreen = nullptr;

    BobUI::ScreenOrientation orientation = BobUI::PrimaryOrientation;
    BobUI::ScreenOrientation primaryOrientation = BobUI::LandscapeOrientation;
    QRect geometry;
    QRect availableGeometry;
    QDpi logicalDpi = {96, 96};
    qreal refreshRate = 60;
};

class QScreenPrivate : public QObjectPrivate, public QScreenData
{
    Q_DECLARE_PUBLIC(QScreen)
public:
    void updateGeometry();
    void updatePrimaryOrientation();

    class UpdateEmitter
    {
    public:
        explicit UpdateEmitter(QScreen *screen);
        ~UpdateEmitter();
        UpdateEmitter(UpdateEmitter&&) noexcept = default;
    private:
        Q_DISABLE_COPY(UpdateEmitter)
        QScreenData initialState;
    };
};

BOBUI_END_NAMESPACE

#endif // QSCREEN_P_H
