// Copyright (C) 2014 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Sean Harmer <sean.harmer@kdab.com>
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QOPENGLVERTEXARRAYOBJECT_H
#define QOPENGLVERTEXARRAYOBJECT_H

#include <BobUIOpenGL/bobuiopenglglobal.h>

#ifndef BOBUI_NO_OPENGL

#include <BobUICore/QObject>
#include <BobUIGui/qopengl.h>

BOBUI_BEGIN_NAMESPACE

class QOpenGLVertexArrayObjectPrivate;

class Q_OPENGL_EXPORT QOpenGLVertexArrayObject : public QObject
{
    Q_OBJECT

public:
    explicit QOpenGLVertexArrayObject(QObject* parent = nullptr);
    ~QOpenGLVertexArrayObject();

    bool create();
    void destroy();
    bool isCreated() const;
    GLuint objectId() const;
    void bind();
    void release();

    class Binder
    {
    public:
        inline Binder(QOpenGLVertexArrayObject *v)
            : vao(v)
        {
            Q_ASSERT(v);
            if (vao->isCreated() || vao->create())
                vao->bind();
        }

        inline ~Binder()
        {
            release();
        }

        inline void release()
        {
            vao->release();
        }

        inline void rebind()
        {
            vao->bind();
        }

    private:
        Q_DISABLE_COPY(Binder)
        QOpenGLVertexArrayObject *vao;
    };

private:
    Q_DISABLE_COPY(QOpenGLVertexArrayObject)
    Q_DECLARE_PRIVATE(QOpenGLVertexArrayObject)
    Q_PRIVATE_SLOT(d_func(), void _q_contextAboutToBeDestroyed())
    QOpenGLVertexArrayObject(QOpenGLVertexArrayObjectPrivate &dd);
};

BOBUI_END_NAMESPACE

#endif

#endif // QOPENGLVERTEXARRAYOBJECT_H
