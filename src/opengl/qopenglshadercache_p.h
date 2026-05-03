// Copyright (C) 2019 The BobUI Company Ltd.
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

#ifndef QOPENGLSHADERCACHE_P_H
#define QOPENGLSHADERCACHE_P_H

#include <BobUIOpenGL/bobuiopenglglobal.h>
#include <BobUICore/private/qglobal_p.h>

BOBUI_BEGIN_NAMESPACE


class QOpenGLShaderProgram;
class QOpenGLContext;

class CachedShader
{
public:
    inline CachedShader(const QByteArray &, const QByteArray &)
    {}

    inline bool isCached()
    {
        return false;
    }

    inline bool load(QOpenGLShaderProgram *, QOpenGLContext *)
    {
        return false;
    }

    inline bool store(QOpenGLShaderProgram *, QOpenGLContext *)
    {
        return false;
    }
};

BOBUI_END_NAMESPACE

#endif
