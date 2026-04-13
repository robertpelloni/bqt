// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWAYLANDDND_H
#define QWAYLANDDND_H

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

#include <qpa/qplatformdrag.h>
#include <BobUIGui/private/qsimpledrag_p.h>

#include <BobUIGui/QDrag>
#include <BobUICore/QMimeData>

#include <BobUIWaylandClient/bobuiwaylandclientglobal.h>

BOBUI_BEGIN_NAMESPACE

namespace BobUIWaylandClient {

class QWaylandDisplay;
#if BOBUI_CONFIG(draganddrop)
class Q_WAYLANDCLIENT_EXPORT QWaylandDrag : public QBasicDrag
{
public:
    QWaylandDrag(QWaylandDisplay *display);
    ~QWaylandDrag() override;

    void setResponse(bool accepted);
    void setResponse(const QPlatformDropBobUIResponse &response);
    void setDropResponse(const QPlatformDropBobUIResponse &response);
    void finishDrag();

protected:
    void startDrag() override;
    void cancel() override;
    void move(const QPoint &globalPos, BobUI::MouseButtons b, BobUI::KeyboardModifiers mods) override;
    void drop(const QPoint &globalPos, BobUI::MouseButtons b, BobUI::KeyboardModifiers mods) override;
    void endDrag() override;

    bool ownsDragObject() const override;

private:
    QWaylandDisplay *m_display = nullptr;
};
#endif
}

BOBUI_END_NAMESPACE

#endif // QWAYLANDDND_H
