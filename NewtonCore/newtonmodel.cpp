#include <QGraphicsScene>
#include <QObject>
#include "newtonmodel.h"

NewtonModel::NewtonModel(QObject *parent) : QObject(parent)
{
    currenSceneIndex = 0;
    graphicsScene = new graphicsScene();
}

NewtonModel::NewtonModel(QString filePath, QObject *parent) : QObject (parent)
{
    loadFile(filePath);
}

NewtonModel::~NewtonModel(){
    delete graphicsScene;
}

