#ifndef _CRM_GL_WINDOW_H_
#define _CRM_GL_WINDOW_H_

#include <QGLWidget>

class CrmGlWindow : public QGLWidget {
protected:
    void initializeGL();
    void paintGL();
};

#endif
