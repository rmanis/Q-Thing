
#include <iostream>
#include <QFile>
#include <QDebug>
#include "CrmGlWindow.h"
#include "globalicious.h"

typedef void (*PglGenVertexArrays) (GLsizei n,  GLuint *arrays);
typedef void (*PglBindVertexArray) (GLuint array);

/*
 * These are supposed to already be a thing!
 */
typedef void (*PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (*PFNGLGETSHADERLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

/**
 * TODO: bring back the shader class
 * TODO: create a gameobject class that encapsulates this stuff
 * TODO: create world class that iterates over game objects
 */
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
        vBufferId(0),
        programId(0) {
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

    initializeCoreVBO();

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    initializeShaders();
}

/*
 * Without this, the core profile won't work.
 */
void CrmGlWindow::initializeCoreVBO() {
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

    glDrawArrays(GL_LINE_LOOP, 0, 3);
}

bool checkStatus(GLuint objId,
        PFNGLGETSHADERIVPROC propGet,
        PFNGLGETSHADERLOGPROC getInfoLog,
        GLenum statusType) {
    bool status = true;
    int success = 0;

    propGet(objId, statusType, &success);

    if (success == GL_FALSE) {
        GLint logSize = 0;
        propGet(objId, GL_INFO_LOG_LENGTH, &logSize);
        char *log = new char[logSize + 1];
        getInfoLog(objId, logSize, &logSize, log);
        qWarning() << "Error : " << log;
        delete[] log;
        status = false;
    }

    return status;

}

bool checkCompileStatus(GLuint shaderId) {
    bool status = checkStatus(shaderId, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
    if (!status) {
        qWarning() << "Compile failed";
        glDeleteShader(shaderId);
    }

    return status;
}

bool checkLinkStatus(GLuint programId) {
    bool status = checkStatus(programId, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
    if (!status) {
        qWarning() << "Link failed";
        glDeleteProgram(programId);
    }
    return status;
}

bool compileShader(GLuint shaderId) {
    glCompileShader(shaderId);
    return checkCompileStatus(shaderId);
}

bool linkProgram(GLuint programId) {
    glLinkProgram(programId);
    return checkLinkStatus(programId);
}

void CrmGlWindow::initializeShaders() {
    bool shouldCompile = true;
    bool shouldLink = true;
    char *vAdapter[1];
    char *fAdapter[1];
    QFile vertFile(":/Resources/vertexShader.vs");
    QFile fragFile(":/Resources/fragmentShader.fs");
    QByteArray vBytes, fBytes;

    if (!vertFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open vertex shader";
        shouldCompile = false;
    } else {
        vBytes = vertFile.readAll();
        vAdapter[0] = vBytes.data();
    }

    if (!fragFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open fragment shader";
        shouldCompile = false;
    } else {
        fBytes = fragFile.readAll();
        fAdapter[0] = fBytes.data();
    }

    if (shouldCompile) {
        programId = glCreateProgram();
        GLuint vShaderId = glCreateShader(GL_VERTEX_SHADER);
        GLuint fShaderId = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vShaderId, 1, vAdapter, 0);
        glShaderSource(fShaderId, 1, fAdapter, 0);
        shouldLink = compileShader(vShaderId) & compileShader(fShaderId);

        if (shouldLink) {
            glAttachShader(programId, vShaderId);
            glAttachShader(programId, fShaderId);

            if (linkProgram(programId)) {
                glUseProgram(programId);
            }
        } else {
            glDeleteProgram(programId);
        }
    }
}
