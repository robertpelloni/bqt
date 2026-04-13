// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QSHAPEDPIXMAPDNDWINDOW_H
#define QSHAPEDPIXMAPDNDWINDOW_H

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
#include <BobUIGui/QRasterWindow>
#include <BobUIGui/QPixmap>

BOBUI_REQUIRE_CONFIG(draganddrop);

BOBUI_BEGIN_NAMESPACE

class QShapedPixmapWindow : public QRasterWindow
{
    Q_OBJECT
public:
    explicit QShapedPixmapWindow(QScreen *screen = nullptr);
    ~QShapedPixmapWindow();

    void setUseCompositing(bool on) { m_useCompositing = on; }
    void setPixmap(const QPixmap &pixmap);
    void setHotspot(const QPoint &hotspot);

    void updateGeometry(const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    QPixmap m_pixmap;
    QPoint m_hotSpot;
    bool m_useCompositing;
};

BOBUI_END_NAMESPACE

#endif // QSHAPEDPIXMAPDNDWINDOW_H
