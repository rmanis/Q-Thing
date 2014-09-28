/*
 * Shader.cpp
 *
 *  Created on: Sep 28, 2014
 *      Author: crm
 */

#include <cstdio>
#include <cstring>
#include "Shader.h"
#include "globalicious.h"

char *slurp(const char *filename) {

    unsigned int length = strlen(directoryName) + 2 + strlen(filename);
    char *fullPath = new char[length];
    sprintf(fullPath, "%s/%s", directoryName, filename);

    char *buf = NULL;
    FILE* f = fopen(fullPath, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);

        buf = new char[size + 1];

        rewind(f);
        fread(buf, sizeof(char), size, f);
        buf[size] = '\0';
    }

    delete[] fullPath;
    return buf;
}

Shader::Shader() {
    programId = vShaderId = fShaderId = 0;
}

Shader::Shader(const char *vertexFilename, const char *fragmentFilename) {
    char *vertexCode = slurp(vertexFilename);
    char *fragmentCode = slurp(fragmentFilename);
    char *vAdapter[] = { vertexCode };
    char *fAdapter[] = { fragmentCode };

    programId = glCreateProgram();
    vShaderId = glCreateShader(GL_VERTEX_SHADER);
    fShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vShaderId, 1, vAdapter, 0);
    glShaderSource(fShaderId, 1, fAdapter, 0);

    glCompileShader(vShaderId);
    glCompileShader(fShaderId);

    glAttachShader(programId, vShaderId);
    glAttachShader(programId, fShaderId);
    glLinkProgram(programId);
}

Shader::~Shader() {
    // TODO Auto-generated destructor stub
}

void Shader::use() {
    glUseProgram(programId);
}
