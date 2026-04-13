// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDSHMWINDOW_H
#define QWAYLANDSHMWINDOW_H

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

#include <BobUIWaylandClient/private/qwaylandwindow_p.h>
#include <BobUIGui/QRegion>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class Q_WAYLANDCLIENT_EXPORT QWaylandShmWindow : public QWaylandWindow
{
public:
    QWaylandShmWindow(QWindow *window, QWaylandDisplay *display);
    ~QWaylandShmWindow() override;

    WindowType windowType() const override;

    bool createDecoration() override;
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDSHMWINDOW_H
