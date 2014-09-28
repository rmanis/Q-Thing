/*
 * Shader.h
 *
 *  Created on: Sep 28, 2014
 *      Author: crm
 */

#ifndef SHADER_H_
#define SHADER_H_

#include <OpenGL/gl.h>

class Shader {
    GLuint programId;
    GLuint vShaderId;
    GLuint fShaderId;
public:
    Shader();
    Shader(const char *vertexFilename, const char *fragmentFilename);
    ~Shader();
    void use(void);
};

#endif /* SHADER_H_ */
