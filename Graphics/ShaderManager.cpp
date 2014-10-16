/*
 * ShaderManager.cpp
 *
 *  Created on: Oct 15, 2014
 *      Author: crm
 */

#include <QDebug>
#include <QDirIterator>
#include <QFileInfo>
#include <QHash>
#include "ShaderManager.h"

ShaderManager *ShaderManager::manager = 0;
unsigned ShaderManager::MAX_NUM_SHADERS = 100;

ShaderManager::ShaderManager() : numShaders(0) {
    shaders = new Shader*[MAX_NUM_SHADERS];

    shaderByName = new QHash<QString, Shader*>();
    QDirIterator it(":/Resources/Shaders");
    unsigned i = 0;
    while (it.hasNext() && i < MAX_NUM_SHADERS) {
        QString shaderDir = it.next();
        QString shaderName = QFileInfo(shaderDir).baseName();
        qDebug() << "Loading shader " << shaderName << " from Q resources.";

        Shader *s = new Shader(shaderDir + "/vertexShader.vs",
                shaderDir + "/fragmentShader.fs");
        s->load();
        shaders[i++] = s;
        (*shaderByName)[shaderName] = s;
    }
}

ShaderManager::~ShaderManager() {
    delete shaderByName;
    for (unsigned i = 0; i < numShaders; i++) {
        shaders[i] = 0;
    }
    delete[] shaders;
}

ShaderManager *ShaderManager::getInstance() {
    if (!ShaderManager::manager) {
        ShaderManager::manager = new ShaderManager();
    }
    return ShaderManager::manager;
}

Shader *ShaderManager::getShader(const char *shaderName) {
    return shaderByName->value(QString(shaderName));
}
