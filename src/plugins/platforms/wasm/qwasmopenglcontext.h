// Copyright (C) 2018 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef QWASMOPENGLCONTEXT_H
#define QWASMOPENGLCONTEXT_H

#include <BobUICore/qhash.h>

#include <qpa/qplatformopenglcontext.h>

#include <emscripten.h>
#include <emscripten/html5.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLContext;
class QPlatformScreen;
class QPlatformSurface;
class QWasmOpenGLContext : public QPlatformOpenGLContext
{
public:
    explicit QWasmOpenGLContext(QOpenGLContext *context);
    ~QWasmOpenGLContext();

    QSurfaceFormat format() const override;
    void swapBuffers(QPlatformSurface *surface) override;
    GLuint defaultFramebufferObject(QPlatformSurface *surface) const override;
    bool makeCurrent(QPlatformSurface *surface) override;
    void doneCurrent() override;
    bool isSharing() const override;
    bool isValid() const override;
    QFunctionPointer getProcAddress(const char *procName) override;

    static void destroyWebGLContext(QPlatformSurface *surface);

private:
    static bool isOpenGLVersionSupported(QSurfaceFormat format);
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE createEmscriptenContext(const std::string &canvasSelector, QSurfaceFormat format);
    static void destroyWebGLContext(EMSCRIPTEN_WEBGL_CONTEXT_HANDLE contextHandle);

    QSurfaceFormat m_actualFormat;
    QPlatformSurface *m_madeCurrentSurface = nullptr;
    QPlatformSurface *m_contextOwningSurface = nullptr;
    static QHash<QPlatformSurface *, EMSCRIPTEN_WEBGL_CONTEXT_HANDLE> s_contexts;
};

BOBUI_END_NAMESPACE

#endif // QWASMOPENGLCONTEXT_H
