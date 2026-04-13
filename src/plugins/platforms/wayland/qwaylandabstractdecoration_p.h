// Copyright (C) 2017 Robin Burchell <robin.burchell@viroteck.net>
// Copyright (C) 2019 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDABSTRACTDECORATION_H
#define QWAYLANDABSTRACTDECORATION_H

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

#include <BobUICore/QMargins>
#include <BobUICore/QPointF>
#include <BobUIGui/QGuiApplication>
#include <BobUIGui/QCursor>
#include <BobUIGui/QColor>
#include <BobUIGui/QStaticText>
#include <BobUIGui/QImage>
#include <BobUIGui/QEventPoint>
#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

#include <BobUICore/QDebug>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE

class QWindow;
class QPaintDevice;
class QPainter;
class QEvent;

namespace BobUIWaylandClient {

class QWaylandScreen;
class QWaylandWindow;
class QWaylandInputDevice;
class QWaylandAbstractDecorationPrivate;

class Q_WAYLANDCLIENT_EXPORT QWaylandAbstractDecoration : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWaylandAbstractDecoration)
public:
    enum MarginsType {
        Full,
        ShadowsExcluded,
        ShadowsOnly
    };

    QWaylandAbstractDecoration();
    ~QWaylandAbstractDecoration() override;

    void setWaylandWindow(QWaylandWindow *window);
    QWaylandWindow *waylandWindow() const;

    void update();
    bool isDirty() const;

    virtual QMargins margins(MarginsType marginsType = Full) const = 0;

    QWindow *window() const;
    const QImage &contentImage();

    virtual bool handleMouse(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global,BobUI::MouseButtons b,BobUI::KeyboardModifiers mods) = 0;
    virtual bool handleTouch(QWaylandInputDevice *inputDevice, const QPointF &local, const QPointF &global, QEventPoint::State state, BobUI::KeyboardModifiers mods) = 0;

protected:
    virtual void paint(QPaintDevice *device) = 0;

    void setMouseButtons(BobUI::MouseButtons mb);

    void startResize(QWaylandInputDevice *inputDevice, BobUI::Edges edges, BobUI::MouseButtons buttons);
    void startMove(QWaylandInputDevice *inputDevice, BobUI::MouseButtons buttons);
    void showWindowMenu(QWaylandInputDevice *inputDevice);

    bool isLeftClicked(BobUI::MouseButtons newMouseButtonState);
    bool isRightClicked(BobUI::MouseButtons newMouseButtonState);
    bool isLeftReleased(BobUI::MouseButtons newMouseButtonState);
};

}

BOBUI_END_NAMESPACE

#endif // QWAYLANDABSTRACTDECORATION_H
