#include <cstring>
#include <OpenGL/gl.h>
#include <QDirIterator>
#include <QFile>
#include <QDebug>
#include <QList>
#include "Shader.h"

const char *Shader::vertexSuffix   = ".vs";
const char *Shader::fragmentSuffix = ".fs";
const char *Shader::geometrySuffix = ".gs";

Shader::Shader() :
        programId(0),
        shaderPath("") {
}

Shader::Shader(QString shaderPath) :
        programId(0),
        shaderPath(shaderPath) {
}

Shader::~Shader() {
    if (programId) {
        glDeleteProgram(programId);
    }
}

bool Shader::load(void) {
    bool success = false;
    bool keepGoing = true;
    QList<GLuint> shaders;

    QDirIterator it(shaderPath);
    while (it.hasNext()) {
        QString fileName = it.next();
        bool tmpSuccess = true;
        GLuint shaderId = createShader(fileName, &tmpSuccess);
        if (shaderId) {
            shaders.append(shaderId);
        }
        keepGoing &= tmpSuccess;
    }
    unsigned numShaders = shaders.size();
    if (numShaders > 0) {
        success = true;
        programId = glCreateProgram();
        for (unsigned i = 0; i < numShaders; i++) {
            GLuint shaderId = shaders[i];
            glAttachShader(programId, shaderId);
            glDeleteShader(shaderId);
        }
        if (!linkProgram(programId)) {
            qWarning() << "Link error.";
            keepGoing = false;
        }
        if (!keepGoing) {
            qWarning() << "Error loading shader " << shaderPath;
            success = false;
            glDeleteProgram(programId);
            programId = 0;
        }
    }
    return success;
}

GLuint Shader::createShader(QString path, bool *success) {
    GLuint shaderId = 0;
    GLenum type;
    bool shouldCompile = true;

    if (path.endsWith(QString(vertexSuffix))) {
        type = GL_VERTEX_SHADER;
    } else if (path.endsWith(QString(fragmentSuffix))) {
        type = GL_FRAGMENT_SHADER;
    } else if (path.endsWith(QString(geometrySuffix))) {
        type = GL_GEOMETRY_SHADER;
    } else {
        return 0;
    }

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open shader " << path;
        shouldCompile = false;
        *success = false;
    } else {
        char *adapter[] = { f.readAll().data() };
        shaderId = glCreateShader(type);
        glShaderSource(shaderId, 1, adapter, 0);
        if (!compileShader(shaderId)) {
            glDeleteShader(shaderId);
            shaderId = 0;
            *success = false;
        }
    }

    return shaderId;
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
    bool status = checkStatus(shaderId, glGetShaderiv, glGetShaderInfoLog,
            GL_COMPILE_STATUS);
    if (!status) {
        qWarning() << "Compile failed";
        glDeleteShader(shaderId);
    }

    return status;
}

bool Shader::checkLinkStatus(GLuint programId) {
    bool status = checkStatus(programId, glGetProgramiv, glGetProgramInfoLog,
            GL_LINK_STATUS);
    if (!status) {
        qWarning() << "Link failed";
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
