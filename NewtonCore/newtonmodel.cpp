#include <QGraphicsScene>
#include <QObject>
#include "newtonmodel.h"
#include "QFile"
#include "QJsonObject"

NewtonModel::NewtonModel(QObject *parent) : QObject(parent)
{
    currentSceneIndex = 0;
    graphicsScene = new QGraphicsScene();
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

    //open stream and read next unit as string
    QFile nextUnitFile(filePath);
    nextUnit.open(QIODevice::ReadOnly | QIODevice::Text);
    QString nextUnit = nextUnitFile.readAll();
    nextUnitFile.close();

    QJsonDocument doc = QJsonObject::fromJson(nextUnit.toUtf8());
    QJsonObject unit = doc.object();
    QJsonArray scenelist = unit["scenes"].toArray();
    for(int i = 0; i < scenelist.size(); i++){


        QJsonObject currentScene = scenelist[i];
        float grav = currentScene["gravity"];
        QString tutorial(currentScene["tutorial"]);
        QString problemText(currentScene["problemText"]);
        scenes.push_back(new NewtonScene(grav, nullptr, tutorial,problemText,this));

        QJsonArray objs = currentScene["objects"];
        for(int i = 0; i < objs.size(); i++){
            //TODO: populate objects in currentScene
        }
        emit instructionTextChanged(problemText);

        QJsonObject widge = currentScene["widgets"];
        QJsonArray formulas = widge["formulas"];
        QJsonArray inputFields = widge["inputFieldUnits"];

        QStringList labels;
        QList<bool> labelsEnabled;

        for(int i = 0 ; i < formulas.size(); i++){
                labels.push_back(formulas[i]);
                labelsEnabled.push_back(false);
        }
        for(int j = 0; j < inputFields.size(); j++){
            labels.push_back(inputFields[j]);
            labelsEnabled.push_back(true);
        }
        emit inputWidgetsChanged(labels, labelsEnabled);



        //add answers to holder
        //emit units and question info



        //void instructionTextChanged(QString newText);
        //void inputWidgetsChanged(QStringList widgetLabels, QList<bool> enabled);

    }





}

void NewtonModel::setScene(int sceneIndex){
    if(sceneIndex < 0 ||
       sceneIndex >= (int) scenes.length() ||
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

void NewtonModel::getProblemDescriptions(QStringList& descriptions){
    for(size_t i = 0; i < scenes.length(); i++){
        descriptions.append(scenes[i]->getTutorialText());
    }

}

void NewtonModel::validateAnswer(float answer){
    //TODO: validate
    //TODO: notify validated
}


