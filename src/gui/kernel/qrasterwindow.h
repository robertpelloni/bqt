// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QRASTERWINDOW_H
#define QRASTERWINDOW_H

#include <BobUIGui/bobuiguiglobal.h>
#include <BobUIGui/QPaintDeviceWindow>

BOBUI_BEGIN_NAMESPACE

class QRasterWindowPrivate;

class Q_GUI_EXPORT QRasterWindow : public QPaintDeviceWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QRasterWindow)

public:
    explicit QRasterWindow(QWindow *parent = nullptr);
    ~QRasterWindow();

protected:
    int metric(PaintDeviceMetric metric) const override;
    QPaintDevice *redirected(QPoint *) const override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Q_DISABLE_COPY(QRasterWindow)
};

BOBUI_END_NAMESPACE

#endif
