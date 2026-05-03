// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMEVENTDISPATCHER_H
#define QWASMEVENTDISPATCHER_H

#include <BobUICore/private/qeventdispatcher_wasm_p.h>

#include <memory>

BOBUI_BEGIN_NAMESPACE

class QWasmSuspendResumeControl;

class QWasmEventDispatcher : public QEventDispatcherWasm
{
public:
    QWasmEventDispatcher(std::shared_ptr<QWasmSuspendResumeControl> suspendResume);

protected:
    bool sendPostedEvents() override;
    void onLoaded() override;
};

BOBUI_END_NAMESPACE

#endif
