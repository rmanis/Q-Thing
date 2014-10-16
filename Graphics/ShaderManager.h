/*
 * ShaderManager.h
 *
 *  Created on: Oct 15, 2014
 *      Author: crm
 */

#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <QtContainerFwd>
#include "Shader.h"

class ShaderManager {
    static ShaderManager *manager;
    static unsigned MAX_NUM_SHADERS;

    unsigned numShaders;
    Shader **shaders;
    QHash<QString, Shader*> *shaderByName;

    ShaderManager();
public:
    ~ShaderManager();
    static ShaderManager *getInstance();

    Shader *getShader(const char *shaderName);
};

#endif /* SHADERMANAGER_H_ */
