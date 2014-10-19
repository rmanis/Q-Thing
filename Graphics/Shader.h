#ifndef _SHADER_H_
#define _SHADER_H_

#include <QGL>
/*
 * These are supposed to already be a thing!
 */
typedef void (*PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (*PFNGLGETSHADERLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

class QString;

class Shader {
    GLuint programId;
    QString shaderPath;
public:
    static const char *vertexSuffix;
    static const char *fragmentSuffix;
    static const char *geometrySuffix;
    Shader();
    Shader(QString shaderPath);
    ~Shader();
    bool load(void);
    void use(void);
private:
    GLuint createShader(QString path, bool *success);
    bool checkStatus(GLuint objId,
            PFNGLGETSHADERIVPROC propGet,
            PFNGLGETSHADERLOGPROC getInfoLog,
            GLenum statusType);
    bool checkCompileStatus(GLuint shaderId);
    bool checkLinkStatus(GLuint programId);
    bool compileShader(GLuint shaderId);
    bool linkProgram(GLuint programId);
};

#endif
