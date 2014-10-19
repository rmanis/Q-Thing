#include <cstring>
#include <OpenGL/gl.h>
#include <QFile>
#include <QDebug>
#include "Shader.h"

Shader::Shader() :
        programId(0),
        vertexFilename(""),
        fragmentFilename("") {
}

Shader::Shader(QString vertexFilename, QString fragmentFilename) :
        programId(0),
        vertexFilename(vertexFilename),
        fragmentFilename(fragmentFilename) {
}

Shader::~Shader() {
    if (programId) {
        glDeleteProgram(programId);
    }
}

void Shader::load(void) {
    bool shouldCompile = true;
    bool shouldLink = true;
    const char *vAdapter[1];
    const char *fAdapter[1];
    QFile vertFile(vertexFilename);
    QFile fragFile(fragmentFilename);
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

            if (!linkProgram(programId)) {
                qWarning() << "Link error.";
            }
        } else {
            glDeleteProgram(programId);
        }
        glDeleteShader(vShaderId);
        glDeleteShader(fShaderId);
    }
}

void Shader::use(void) {
    if (!programId) {
        load();
    }
    glUseProgram(programId);
}

bool Shader::checkStatus(GLuint objId,
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

bool Shader::checkCompileStatus(GLuint shaderId) {
    bool status = checkStatus(shaderId, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
    if (!status) {
        qWarning() << "Compile failed";
        glDeleteShader(shaderId);
    }

    return status;
}

bool Shader::checkLinkStatus(GLuint programId) {
    bool status = checkStatus(programId, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
    if (!status) {
        qWarning() << "Link failed";
        glDeleteProgram(programId);
    }
    return status;
}

bool Shader::compileShader(GLuint shaderId) {
    glCompileShader(shaderId);
    return checkCompileStatus(shaderId);
}

bool Shader::linkProgram(GLuint programId) {
    glLinkProgram(programId);
    return checkLinkStatus(programId);
}
