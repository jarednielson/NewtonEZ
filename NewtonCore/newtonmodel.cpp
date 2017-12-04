#include <QGraphicsScene>
#include <QObject>
#include "newtonmodel.h"

NewtonModel::NewtonModel(QObject *parent) : QObject(parent)
{
    currentSceneIndex = 0;
    graphicsScene = new QGraphicsScene();
}

NewtonModel::NewtonModel(QString filePath, QObject *parent) : QObject (parent)
{
    graphicsScene = new QGraphicsScene();
    loadFile(filePath);
    currentSceneIndex = 0;
}

NewtonModel::~NewtonModel(){
    delete graphicsScene;
}

///
/// \brief NewtonModel::getGraphicsScene
/// \return
///
QGraphicsScene* NewtonModel::getGraphicsScene(){
    return graphicsScene;
}

void NewtonModel::loadFile(QString filePath){
    //TODO Brian implement
}

void NewtonModel::setScene(int sceneIndex){
    if(sceneIndex < 0 ||
       sceneIndex >= (int) scenes.length() ||
       sceneIndex == currentSceneIndex){
        return;
    }

    currentSceneIndex = sceneIndex;

    //TODO: clear graphicsScene
    graphicsScene->clear();
    //TODO: populate graphicsScene with the elements in the scene
    for(size_t i = 0; i < scenes[currentSceneIndex]->getBodies().length(); i++){
        NewtonBody* curr = scenes[currentSceneIndex]->getBodies()[i];
        graphicsScene->addPolygon(curr->getVertices());
    }
    //TODO: notify widgets changed
    //TODO: notify instructionTextChanged
    emit instructionTextChanged(scenes[currentSceneIndex]->getTutorialText());
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

void NewtonModel::getProblemDescriptions(QStringList& descriptions){
    for(size_t i = 0; i < scenes.length(); i++){
        descriptions.append(scenes[i]->getTutorialText());
    }

}

void NewtonModel::validateAnswer(float answer){
    //TODO: validate

    //Place holder
    emit answerValidated(true);
}


