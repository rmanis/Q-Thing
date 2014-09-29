#ifndef _CRM_GL_WINDOW_H_
#define _CRM_GL_WINDOW_H_

#include <QGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class CrmGlWindow : public QGLWidget {
    QOpenGLShaderProgram shader;
    QOpenGLBuffer vertexBuffer;

private:
    inline void viewport() {
        int w = width();
        int h = height();
        glViewport(0, 0, w, h);
    }
    void initializeShaders();

public:
    CrmGlWindow(QGLFormat &format);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected slots:
    void initializeGL();
    void resizeGL(int width, int height);

    void paintGL();
};

#endif
