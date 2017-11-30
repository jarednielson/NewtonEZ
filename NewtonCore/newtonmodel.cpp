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

void NewtonModel::loadFile(QString filePath){
    //TODO Brian implement
}

void NewtonModel::setScene(int sceneIndex){
    if(sceneIndex < 0 ||
       sceneIndex >= (int) scenes.lenght() ||
       sceneIndex == currentSceneIndex){
        return;
    }

    currentSceneIndex = sceneIndex;

    //TODO: clear graphicsScene
    //TODO: populate graphicsScene with the elements in the scene
    //TODO: notify widgets changed
    //TODO: notify instructionTextChanged
    //TODO: update answervalidation
    //TODO: clear any box2d stuff
}

void NewtonModel::startSimulation(){
    //TODO: check to make sure scene isn't running
    //TODO: build box2d scene
    //TODO: create timer and callbacks
    //TODO: nofity simulationStart
    //TODO: run simulation
    //TODO: update graphics view as simulation goes
    //TODO: end simulation
    //TODO: reset graphicsScene
    //TODO: notify end simulation
}

void NewtonModel::endSimulation(){
    //TODO: check to make sure scene is running
    //TODO: kill timer and callbacks
    //TODO: restore graphisScene
    //TODO: notify end simulation
}

QStringList NewtonModel::getProblemDescriptions(){
    QStringList descriptions;

    for(auto it = scenes.begin(); it != scenes.end(); it++){
        descriptions.append(*(it).getTutorialText);
    }

    return descriptions;
}


