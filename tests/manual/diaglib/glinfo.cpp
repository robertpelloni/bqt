// Copyright (C) 2021 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#include "glinfo.h"

#include <QOpenGLFunctions>
#include <BobUIOpenGLWidgets/QOpenGLWidget>
#include <BobUIOpenGL/QOpenGLWindow>
#include <BobUIGui/QOpenGLContext>
#include <BobUIGui/QOpenGLFunctions>
#include <BobUIGui/QWindow>
#include <BobUICore/QDebug>
#include <BobUICore/QString>
#include <BobUICore/BOBUIimer>

namespace BobUIDiag {

static QString getGlString(const QOpenGLContext *ctx, GLenum name)
{
    if (const GLubyte *p = ctx->functions()->glGetString(name))
        return QString::fromLatin1(reinterpret_cast<const char *>(p));
    return QString();
}

static QString glInfo(const QOpenGLContext *ctx)
{
    return getGlString(ctx, GL_VENDOR)
        + QLatin1Char('\n')
        + getGlString(ctx, GL_RENDERER);
}

QString glInfo(const QObject *o)
{
    if (o->isWindowType()) {
        if (const QOpenGLWindow *oglw = qobject_cast<const QOpenGLWindow *>(o))
            return glInfo(oglw->context());
        return QString();
    }

    if (o->isWidgetType()) {
        if (const QOpenGLWidget *g = qobject_cast<const QOpenGLWidget *>(o))
            return glInfo(g->context());
    }
    return QString();
}

} // namespace BobUIDiag
