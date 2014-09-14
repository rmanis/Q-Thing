
#include "CrmGlWindow.h"

void CrmGlWindow::initializeGL() {
    glViewport(0, 0, width(), height());
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void CrmGlWindow::paintGL() {
    glViewport(0, 0, width(), height());
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
