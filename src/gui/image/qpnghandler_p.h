// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPNGHANDLER_P_H
#define QPNGHANDLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the BobUI API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <BobUIGui/private/bobuiguiglobal_p.h>
#include "BobUIGui/qimageiohandler.h"

#ifndef BOBUI_NO_IMAGEFORMAT_PNG

BOBUI_BEGIN_NAMESPACE

class QPngHandlerPrivate;
class QPngHandler : public QImageIOHandler
{
public:
    QPngHandler();
    ~QPngHandler();

    bool canRead() const override;
    bool read(QImage *image) override;
    bool write(const QImage &image) override;

    QVariant option(ImageOption option) const override;
    void setOption(ImageOption option, const QVariant &value) override;
    bool supportsOption(ImageOption option) const override;

    static bool canRead(QIODevice *device);

private:
    QPngHandlerPrivate *d;
};

BOBUI_END_NAMESPACE

#endif // BOBUI_NO_IMAGEFORMAT_PNG
#endif // QPNGHANDLER_P_H
