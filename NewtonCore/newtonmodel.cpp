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
    graphicsScene->clear();
    emit simulationStart();
    //TODO: build box2d scene
    //TODO: create timer and callbacks
    //TODO: nofity simulationStart
    emit simulationStart();
    //TODO: run simulation
    //TODO: update graphics view as simulation goes
    //TODO: end simulation
    clearModel();
    //TODO: reset graphicsScene
    graphicsScene->clear();
    //TODO: notify end simulation
    emit simulationEnd();
}

void NewtonModel::endSimulation(){
    //TODO: check to make sure scene is running
    emit simulationStart();  // Besides this, actual running also be notified?
    //TODO: kill timer and callbacks
    //TODO: restore graphisScene
    graphicsScene->clear();
    //TODO: notify end simulation
    emit simulationEnd();
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

void NewtonModel::clearModel(){
    graphicsScene->clear();
    //TODO: need to clean other attibutes as well
}

void NewtonModel::resetGraphicsView(){
    graphs.clear();
    scenes.clear();  // Remove all elements in Qvectors
    currentSceneIndex = 0;
}

//TODO:
//loop and check the passed in shapes are dynamic or static and report.
// Actual box2d goes here
// Check the types of shapes passed in (rectangle or the circle)
// IsDdynamic in the  newtonObject.cpp
// Set up the world and shape of the box2d

void NewtonModel::verifyScene(int sceneIndex){
    if (sceneIndex == currentSceneIndex)
    {
        for(size_t i = 0; i < scenes[currentSceneIndex]->getBodies().length(); i++){

        }

    }
}



