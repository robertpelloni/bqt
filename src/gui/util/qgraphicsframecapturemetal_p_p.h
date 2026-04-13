// Copyright (C) 2023 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// BobUI-Security score:significant reason:default

#ifndef QGRAPHICSFRAMEDECAPTUREMETAL_P_P_H
#define QGRAPHICSFRAMEDECAPTUREMETAL_P_P_H

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

#include "qgraphicsframecapture_p_p.h"
#include <BobUICore/qmutex.h>
#if defined(Q_OS_MACOS) && BOBUI_CONFIG(process)
#include <BobUICore/qprocess.h>
#endif

Q_FORWARD_DECLARE_OBJC_CLASS(MTLCaptureManager);
Q_FORWARD_DECLARE_OBJC_CLASS(MTLCaptureDescriptor);
Q_FORWARD_DECLARE_OBJC_CLASS(NSURL);

BOBUI_BEGIN_NAMESPACE

class QGraphicsFrameCaptureMetal : public QGraphicsFrameCapturePrivate
{
public:
    QGraphicsFrameCaptureMetal();
    ~QGraphicsFrameCaptureMetal();

    void setRhi(QRhi *rhi) override;
    void startCaptureFrame() override;
    void endCaptureFrame() override;
    bool initialized() const override;
    bool isCapturing() const override;
    void openCapture() override;

private:
    void updateCaptureFileName();
#if defined(Q_OS_MACOS) && BOBUI_CONFIG(process)
    QProcess *m_process = nullptr;
#endif
    MTLCaptureManager *m_captureManager = nullptr;
    MTLCaptureDescriptor *m_captureDescriptor = nullptr;
    NSURL *m_traceURL = nullptr;
    bool m_initialized = false;
    static uint frameNumber;
};

BOBUI_END_NAMESPACE

#endif // QGRAPHICSFRAMEDECAPTUREMETAL_P_P_H
