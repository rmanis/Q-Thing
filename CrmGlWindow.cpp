
#include <iostream>
#include <QDebug>
#include "CrmGlWindow.h"
#include "globalicious.h"

typedef void (*PglGenVertexArrays) (GLsizei n,  GLuint *arrays);
typedef void (*PglBindVertexArray) (GLuint array);

GLfloat verts[] = {
    +0.0f, +0.1f, +0.0f,
    1.0f, 0.0f, 0.0f,

    -0.1f, -0.1f, +0.0f,
    0.0f, 1.0f, 0.0f,

    +0.1f, -0.1f, +0.0f,
    0.0f, 0.0f, 1.0f
};

CrmGlWindow::CrmGlWindow(QGLFormat &format) :
        QGLWidget(format),
        vertexBuffer(QOpenGLBuffer::VertexBuffer) {
}

QSize CrmGlWindow::sizeHint() const
{
    return QSize(640, 480);
}

QSize CrmGlWindow::minimumSizeHint() const {
    return QSize(200, 200);
}

void CrmGlWindow::initializeGL() {
    QGLWidget::initializeGL();

    QString versionString(QLatin1String(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    qDebug() << "Driver Version String:" << versionString;
    qDebug() << "Current Context:" << format();

    // we need a VAO in core!
    GLuint VAO;
    PglGenVertexArrays glGenVertexArrays = (PglGenVertexArrays) context()->getProcAddress("glGenVertexArrays");
    PglBindVertexArray glBindVertexArray = (PglBindVertexArray) context()->getProcAddress("glBindVertexArray");
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    vertexBuffer.create();
    vertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (!vertexBuffer.bind()) {
        qWarning() << "Could not bind vertex buffer.";
        return;
    }

    vertexBuffer.allocate(verts, 3 * 6 * sizeof(GL_FLOAT));

    initializeShaders();
}

void CrmGlWindow::resizeGL(int width, int height) {
    IGNORE(width); IGNORE(height);
    viewport();
}

void CrmGlWindow::paintGL() {
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void CrmGlWindow::initializeShaders() {

    bool result = shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/Resources/vertexShader.vs");
    if (!result) {
        qWarning() << shader.log();
    }

    result = shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/Resources/fragmentShader.fs");
    if (!result) {
        qWarning() << shader.log();
    }

    result = shader.link();
    if (!result) {
        qWarning() << "Could not link shader.";
    }

    if (!shader.bind()) {
        qDebug() << "Could not bind shader program.";
    }

    /*
     * void setAttributeBuffer
     *  (const char *name,     // the variable name in the shader
     *   GLenum type,          // GL_FLOAT, etc
     *   int offset,           // bytes
     *   int tupleSize,        // 0,1,2,3,4
     *   int stride = 0);      // bytes
     *
     */
    shader.setAttributeBuffer("inPosition",
            GL_FLOAT,
            0,
            3,
            6 * sizeof(GL_FLOAT));
    shader.setAttributeBuffer("inColor",
            GL_FLOAT,
            3 * sizeof(GL_FLOAT),
            3,
            6 * sizeof(GL_FLOAT));
    shader.enableAttributeArray("inPosition");
    shader.enableAttributeArray("inColor");
}
