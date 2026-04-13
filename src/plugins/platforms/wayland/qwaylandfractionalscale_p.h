// Copyright (C) 2022 David Edmundson <davidedmundson@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDFRACTIONALSCALE_P_H
#define QWAYLANDFRACTIONALSCALE_P_H

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

#include <BobUIWaylandClient/private/qwayland-fractional-scale-v1.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <QObject>

#include <optional>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandFractionalScale : public QObject, public BobUIWayland::wp_fractional_scale_v1
{
    Q_OBJECT
public:
    explicit QWaylandFractionalScale(struct ::wp_fractional_scale_v1 *object);
    ~QWaylandFractionalScale();

    std::optional<qreal> preferredScale() const { return mPreferredScale; }

Q_SIGNALS:
    void preferredScaleChanged();

protected:
    void wp_fractional_scale_v1_preferred_scale(uint scale) override;

private:
    std::optional<qreal> mPreferredScale;
};

}

BOBUI_END_NAMESPACE

#endif
