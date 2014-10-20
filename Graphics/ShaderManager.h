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

#define getShaderManager() ShaderManager::getInstance()

class QDirIterator;

class ShaderManager {
    static ShaderManager *manager;
    static const unsigned MAX_NUM_SHADERS = 100;

    unsigned numShaders;
    unsigned currentShader; // index of shaders array
    Shader shaders[MAX_NUM_SHADERS];
    QHash<QString, unsigned> indexByName;
    QHash<unsigned, QString> nameByIndex;

    ShaderManager();
    void initialize();
    void loadDirectoryShaders(QDirIterator& dir);

public:
    ~ShaderManager();
    static ShaderManager *getInstance();

    void useShader(QString shaderName);
    void increment(int amount = 1);
    void reinitialize();
    QStringList getShaderNames() const;
    QString getShaderName() const;
};

#endif /* SHADERMANAGER_H_ */
