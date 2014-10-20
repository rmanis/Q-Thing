
#include <iostream>
#include <QFile>
#include <QDebug>
#include <QKeyEvent>
#include "CrmGlWindow.h"
#include "Graphics/ShaderManager.h"
#include "globalicious.h"

typedef void (*PglGenVertexArrays) (GLsizei n,  GLuint *arrays);
typedef void (*PglBindVertexArray) (GLuint array);

/**
 * TODO: create a gameobject class that encapsulates this stuff
 * TODO: create world class that iterates over game objects
 */
GLfloat verts[] = {
    -0.3f, +0.3f, +0.0f,
    1.0f, 0.0f, 0.0f,

    -0.1f, +0.3f, +0.0f,
    0.0f, 1.0f, 0.0f,

    +0.1f, +0.3f, +0.0f,
    0.0f, 0.0f, 1.0f,

    +0.3f, +0.3f, +0.0f,
    1.0f, 0.3f, 0.4f,


    -0.2f, +0.1f, +0.0f,
    0.2f, 0.3f, 0.4f,

    0.0f, +0.1f, +0.0f,
    0.7f, 0.1f, 0.4f,

    0.2f, +0.1f, +0.0f,
    1.0f, 0.5f, 0.0f,


    -0.1f, -0.1f, +0.0f,
    0.1f, 0.5f, 0.9f,

    +0.1f, -0.1f, +0.0f,
    0.5f, 0.5f, 1.0f,


    +0.0f, -0.3f, +0.0f,
    0.0f, 1.0f, 0.75f,
};

GLushort indices[] = {
    0, 4, 1,
    1, 5, 2,
    2, 6, 3,
    4, 7, 5,
    5, 8, 6,
    7, 9, 8,

    1, 4, 5,
    2, 5, 6,
    5, 7, 8,
};

CrmGlWindow::CrmGlWindow(QGLFormat &format) :
        QGLWidget(format),
        vBufferId(0),
        iBufferId(0) {
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

    initializeCoreVAO();

    glGenBuffers(1, &vBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            6 * sizeof(GL_FLOAT),
            (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
            6 * sizeof(GL_FLOAT),
            (void *) (3 * sizeof(GL_FLOAT)));

    glGenBuffers(1, &iBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
            indices, GL_STATIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    initializeShaders();
}

/*
 * Without this, the core profile won't work.
 */
void CrmGlWindow::initializeCoreVAO() {
    // we need a VAO in core!
    GLuint VAO;
    PglGenVertexArrays glGenVertexArrays = (PglGenVertexArrays) context()->getProcAddress("glGenVertexArrays");
    PglBindVertexArray glBindVertexArray = (PglBindVertexArray) context()->getProcAddress("glBindVertexArray");
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void CrmGlWindow::resizeGL(int width, int height) {
    IGNORE(width); IGNORE(height);
    viewport();
}

void CrmGlWindow::paintGL() {
//    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(short), GL_UNSIGNED_SHORT, (void *) 0);
}

void CrmGlWindow::keyPressEvent(QKeyEvent *e) {
    IGNORE(e);
}
void CrmGlWindow::keyReleaseEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Tab) {

        getShaderManager()->increment();
        qDebug() << "Using shader " << getShaderManager()->getShaderName();
        update();
    } else if (e->key() == Qt::Key_Space) {
        getShaderManager()->reinitialize();
        getShaderManager()->useShader("ColorPassthrough");
        update();
    }
}


void CrmGlWindow::initializeShaders() {

    getShaderManager()->useShader("ColorPassthrough");
}
