#ifndef _CRM_GL_WINDOW_H_
#define _CRM_GL_WINDOW_H_

#include <QGLWidget>

class Shader;

class CrmGlWindow : public QGLWidget {
    GLuint vBufferId;
    GLuint iBufferId;

private:
    inline void viewport() {
        int w = width();
        int h = height();
        glViewport(0, 0, w, h);
    }
    void initializeCoreVAO();
    void initializeShaders();

public:
    CrmGlWindow(QGLFormat &format);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

protected slots:
    void initializeGL();
    void resizeGL(int width, int height);

    void paintGL();

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
};

#endif
