#ifndef _SHADER_H_
#define _SHADER_H_

/*
 * These are supposed to already be a thing!
 */
typedef void (*PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (*PFNGLGETSHADERLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

class Shader {
    GLuint programId;
    char *vertexFilename;
    char *fragmentFilename;
public:
    Shader();
    Shader(const char *vertexFilename, const char *fragmentFilename);
    ~Shader();
    void load(void);
    void use(void);
private:
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
