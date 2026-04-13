// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "qwasmeventdispatcher.h"
#include "qwasmintegration.h"

#include <BobUIGui/qpa/qwindowsysteminterface.h>

BOBUI_BEGIN_NAMESPACE

QWasmEventDispatcher::QWasmEventDispatcher(std::shared_ptr<QWasmSuspendResumeControl> suspendResume)
    :QEventDispatcherWasm(suspendResume)
{

}

// Note: All event dispatcher functionality is implemented in QEventDispatcherWasm
// in BobUICore, except for processPostedEvents() below which uses API from BobUIGui.
bool QWasmEventDispatcher::sendPostedEvents()
{
    QEventDispatcherWasm::sendPostedEvents();
    return QWindowSystemInterface::sendWindowSystemEvents(QEventLoop::AllEvents);
}

void QWasmEventDispatcher::onLoaded()
{
    // This function is called when the application is ready to paint
    // the first frame. Send the bobuilaoder onLoaded event first (via
    // the base class implementation), and then enable/call requestUpdate
    // to deliver a frame.
    QEventDispatcherWasm::onLoaded();

    // Make sure all screens have a defined size; and pick
    // up size changes due to onLoaded event handling.
    QWasmIntegration *wasmIntegration = QWasmIntegration::get();
    wasmIntegration->resizeAllScreens();

    wasmIntegration->releaseRequesetUpdateHold();
}

BOBUI_END_NAMESPACE
