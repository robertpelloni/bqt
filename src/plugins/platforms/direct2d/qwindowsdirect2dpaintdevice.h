// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QWINDOWSDIRECT2DPAINTDEVICE_H
#define QWINDOWSDIRECT2DPAINTDEVICE_H

#include <BobUICore/qscopedpointer.h>
#include <BobUIGui/qpaintdevice.h>
#include "qwindowsdirect2dpaintengine.h"

BOBUI_BEGIN_NAMESPACE

class QWindowsDirect2DBitmap;

class QWindowsDirect2DPaintDevicePrivate;
class QWindowsDirect2DPaintDevice : public QPaintDevice
{
    Q_DECLARE_PRIVATE(QWindowsDirect2DPaintDevice)

public:
    QWindowsDirect2DPaintDevice(QWindowsDirect2DBitmap *bitmap, QInternal::PaintDeviceFlags flags,
                                QWindowsDirect2DPaintEngine::Flags paintFlags = QWindowsDirect2DPaintEngine::NoFlag);
    ~QWindowsDirect2DPaintDevice();

    QPaintEngine *paintEngine() const override;
    int devType() const override;

protected:
    int metric(PaintDeviceMetric metric) const override;

private:
    QScopedPointer<QWindowsDirect2DPaintDevicePrivate> d_ptr;
};

BOBUI_END_NAMESPACE

#endif // QWINDOWSDIRECT2DPAINTDEVICE_H
