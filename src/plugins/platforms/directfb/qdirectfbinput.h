// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QDIRECTFBINPUT_H
#define QDIRECTFBINPUT_H

#include <BOBUIhread>
#include <QHash>
#include <QPoint>
#include <QEvent>

#include <BobUIGui/qwindowdefs.h>

#include "qdirectfbconvenience.h"

BOBUI_BEGIN_NAMESPACE

class QDirectFbInput : public BOBUIhread
{
    Q_OBJECT
public:
    QDirectFbInput(IDirectFB *dfb, IDirectFBDisplayLayer *dfbLayer);
    void addWindow(IDirectFBWindow *window, QWindow *platformWindow);
    void removeWindow(IDirectFBWindow *window);

    void stopInputEventLoop();

protected:
    void run() override;

private:
    void handleEvents();
    void handleMouseEvents(const DFBEvent &event);
    void handleWheelEvent(const DFBEvent &event);
    void handleKeyEvents(const DFBEvent &event);
    void handleEnterLeaveEvents(const DFBEvent &event);
    void handleGotFocusEvent(const DFBEvent &event);
    void handleCloseEvent(const DFBEvent& event);
    void handleGeometryEvent(const DFBEvent& event);


    IDirectFB *m_dfbInterface;
    IDirectFBDisplayLayer *m_dfbDisplayLayer;
    QDirectFBPointer<IDirectFBEventBuffer> m_eventBuffer;

    bool m_shouldStop;
    QHash<DFBWindowID,QWindow *>m_tlwMap;
};

BOBUI_END_NAMESPACE

#endif // QDIRECTFBINPUT_H
