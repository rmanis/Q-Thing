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
#include <QStandardPaths>
#include "ShaderManager.h"

ShaderManager *ShaderManager::manager = 0;
unsigned ShaderManager::MAX_NUM_SHADERS = 100;

ShaderManager::ShaderManager() : numShaders(0) {
    shaders = new Shader*[MAX_NUM_SHADERS];

    shaderByName = new QHash<QString, Shader*>();
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
        manager->initialize();
    }
    return ShaderManager::manager;
}

Shader *ShaderManager::getShader(const char *shaderName) {
    return shaderByName->value(QString(shaderName));
}

void ShaderManager::initialize() {
    QDirIterator it(":/Resources/Shaders");
    qDebug() << "Loading shaders from bundled resources.";
    loadDirectoryShaders(it);

    QString localDir = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Shaders";
    QDirIterator locals(localDir);
    qDebug() << "Loading shaders from filesystem.";
    loadDirectoryShaders(locals);
}

void ShaderManager::loadDirectoryShaders(QDirIterator& dir) {
    while (dir.hasNext() && numShaders < MAX_NUM_SHADERS) {
        QString shaderDir = dir.next();
        QString shaderName = QFileInfo(shaderDir).baseName();
        qDebug() << "Loading shader " << shaderName;
        Shader* s = new Shader(shaderDir + "/vertexShader.vs",
                shaderDir + "/fragmentShader.fs");
        s->load();
        shaders[numShaders++] = s;
        (*shaderByName)[shaderName] = s;
    }
}
