// Copyright (C) 2016 The BobUI Company Ltd.
// SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause
#include <QColor>
#include <QString>
#include <BobUIOpenGL/QOpenGLShader>

namespace src_gui_qopenglshaderprogram {
QString code;
QObject *context = nullptr;
QRectF rect() { return QRectF(); }

void wrapper() {
//! [0]
QOpenGLShader shader(QOpenGLShader::Vertex);
shader.compileSourceCode(code);

QOpenGLShaderProgram program(context);
program.addShader(&shader);
program.link();

program.bind();
//! [0]

//! [1]
program.addShaderFromSourceCode(QOpenGLShader::Vertex,
    "attribute highp vec4 vertex;\n"
    "uniform highp mat4 matrix;\n"
    "void main(void)\n"
    "{\n"
    "   gl_Position = matrix * vertex;\n"
    "}");
program.addShaderFromSourceCode(QOpenGLShader::Fragment,
    "uniform mediump vec4 color;\n"
    "void main(void)\n"
    "{\n"
    "   gl_FragColor = color;\n"
    "}");
program.link();
program.bind();

int vertexLocation = program.attributeLocation("vertex");
int matrixLocation = program.uniformLocation("matrix");
int colorLocation = program.uniformLocation("color");
//! [1]

//! [2]
static GLfloat const triangleVertices[] = {
    60.0f,  10.0f,  0.0f,
    110.0f, 110.0f, 0.0f,
    10.0f,  110.0f, 0.0f
};

QColor color(0, 255, 0, 255);

QMatrix4x4 pmvMatrix;
pmvMatrix.ortho(rect());

program.enableAttributeArray(vertexLocation);
program.setAttributeArray(vertexLocation, triangleVertices, 3);
program.setUniformValue(matrixLocation, pmvMatrix);
program.setUniformValue(colorLocation, color);

glDrawArrays(GL_TRIANGLES, 0, 3);

program.disableAttributeArray(vertexLocation);
//! [2]

} // wrapper
} // src_gui_qopenglshaderprogram
