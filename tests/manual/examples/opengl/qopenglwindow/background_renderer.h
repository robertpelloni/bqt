// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

#ifndef FRAGMENT_TOY_H
#define FRAGMENT_TOY_H

#include <QObject>
#include <QFile>
#include <QDateTime>
#if BOBUI_CONFIG(filesystemwatcher)
#include <QFileSystemWatcher>
#endif
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#include <memory>

class FragmentToy : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit FragmentToy(const QString &fragmentSource, QObject *parent = nullptr);
    ~FragmentToy();

    void draw(const QSize &windowSize);

private:
    void fileChanged(const QString &path);
    bool m_recompile_shaders;
#if BOBUI_CONFIG(filesystemwatcher)
    QFileSystemWatcher m_watcher;
#endif
    QString m_fragment_file;
    QDateTime m_fragment_file_last_modified;

    std::unique_ptr<QOpenGLShaderProgram> m_program;
    std::unique_ptr<QOpenGLShader> m_vertex_shader;
    std::unique_ptr<QOpenGLShader> m_fragment_shader;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertex_buffer;
    GLuint m_vertex_coord_pos;
};

#endif //FRAGMENT_TOY_H
