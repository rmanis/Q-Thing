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

ShaderManager::ShaderManager() : numShaders(0), currentShader(0) {
    shaders = new Shader*[MAX_NUM_SHADERS];

    indexByName = new QHash<QString, unsigned>();
    nameByIndex = new QHash<unsigned, QString>();
}

ShaderManager::~ShaderManager() {
    delete indexByName;
    delete nameByIndex;
    for (unsigned i = 0; i < numShaders; i++) {
        delete shaders[i];
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

void ShaderManager::useShader(QString shaderName) {
    unsigned index = indexByName->value(shaderName, -1);
    if (index < numShaders) {
        currentShader = index;
        shaders[currentShader]->use();
    }
}

void ShaderManager::increment(int amount) {
    currentShader = (currentShader + amount) % numShaders;
    if (currentShader < numShaders) {
        shaders[currentShader]->use();
    }
}

QStringList ShaderManager::getShaderNames() const {
    return QStringList(indexByName->keys());
}

QString ShaderManager::getShaderName() const {
    return (*nameByIndex)[currentShader];
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
        if (shaderName.length() > 0) {
            qDebug() << "Loading shader " << shaderName;
            Shader* s = new Shader(shaderDir + "/vertexShader.vs",
                    shaderDir + "/fragmentShader.fs");
            s->load();
            unsigned newIndex = numShaders++;
            shaders[newIndex] = s;
            (*indexByName)[shaderName] = newIndex;
            (*nameByIndex)[newIndex] = shaderName;
        }
    }
}