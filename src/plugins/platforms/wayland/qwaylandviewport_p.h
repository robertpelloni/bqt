// Copyright (C) 2022 David Edmundson <davidedmundson@kde.org>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDVIEWPORT_P_H
#define QWAYLANDVIEWPORT_P_H

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

#include <BobUIWaylandClient/private/qwayland-viewporter.h>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <QRect>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandViewport : public BobUIWayland::wp_viewport
{
public:
    explicit QWaylandViewport(::wp_viewport *viewport);
    ~QWaylandViewport() override;

    void setSource(const QRectF &source);
    void setDestination(const QSize &destination);

};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDVIEWPORT_P_H
