// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

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

#ifndef QOPENGLTEXTUREUPLOADER_P_H
#define QOPENGLTEXTUREUPLOADER_P_H

#include <BobUICore/qsize.h>
#include <BobUIOpenGL/bobuiopenglglobal.h>
#include <BobUIGui/private/qopenglcontext_p.h>

BOBUI_BEGIN_NAMESPACE

class QImage;

class Q_OPENGL_EXPORT QOpenGLTextureUploader
{
public:
    enum BindOption {
        NoBindOption                            = 0x0000,
        PremultipliedAlphaBindOption            = 0x0001,
        UseRedForAlphaAndLuminanceBindOption    = 0x0002,
        SRgbBindOption                          = 0x0004,
        PowerOfTwoBindOption                    = 0x0008
    };
    Q_DECLARE_FLAGS(BindOptions, BindOption)

    static qsizetype textureImage(GLenum target, const QImage &image, BindOptions options, QSize maxSize = QSize());

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QOpenGLTextureUploader::BindOptions)

BOBUI_END_NAMESPACE

#endif

