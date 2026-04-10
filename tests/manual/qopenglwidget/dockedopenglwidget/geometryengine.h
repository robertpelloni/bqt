// Copyright (C) 2022 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only

#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void drawCubeGeometry(QOpenGLShaderProgram *program);

private:
    void initCubeGeometry();

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // GEOMETRYENGINE_H
