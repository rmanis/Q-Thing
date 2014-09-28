
#include <iostream>
#include "Graphics/Shader.h"
#include "CrmGlWindow.h"
#include "globalicious.h"

GLfloat verts[] = {
    +0.0f, +0.1f, +0.0f,
    1.0f, 0.0f, 0.0f,

    -0.1f, -0.1f, +0.0f,
    0.0f, 1.0f, 0.0f,

    +0.1f, -0.1f, +0.0f,
    0.0f, 0.0f, 1.0f
};


QSize CrmGlWindow::sizeHint() const
{
    return QSize(640, 480);
}

QSize CrmGlWindow::minimumSizeHint() const {
    return QSize(200, 200);
}

void CrmGlWindow::initializeGL() {
    QGLWidget::initializeGL();

    glGenBuffers(1, &vertexBufferId);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (char *)(sizeof(GLfloat) * 3));

    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    initializeShaders();
}

void CrmGlWindow::resizeGL(int width, int height) {
    IGNORE(width); IGNORE(height);
}

void CrmGlWindow::paintGL() {
    viewport();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);

//    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void CrmGlWindow::initializeShaders() {

    shader = Shader("Resources/vertexShader.vs", "Resources/fragmentShader.fs");
    shader.use();
}
