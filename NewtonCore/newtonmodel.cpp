#include <QGraphicsScene>
#include <QObject>
#include "newtonmodel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

    //open stream and read next unit as string
    QFile nextUnitFile(filePath);
    nextUnitFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString nextUnit = nextUnitFile.readAll();
    nextUnitFile.close();

    //deserialize json doc
    QJsonDocument doc = QJsonDocument::fromJson(nextUnit.toUtf8());
    QJsonObject unit = doc.object();
    QJsonArray scenelist = unit["scenes"].toArray();

    //for each scene grab info
    for(int i = 0; i < scenelist.size(); i++){

        //deserialize scene information
        QJsonObject currentScene = scenelist[i].toObject();
        float grav = currentScene["gravity"].toDouble();
        QString tutorial(currentScene["tutorial"].toString());
        QString problemText(currentScene["problemText"].toString());


        QJsonArray varVals = currentScene["varVals"].toArray();
        std::vector<float> chosenRangeValues;
        for(int curRange = 0; curRange < valVals.size(); curRange++){
            QJsonArray range = varVals[i];
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<> distr(range[0], range[1]); // define the range
            float calculated = distr(eng); //calculate our value
            chosenRangeValues.push_back(calculated);
        }

        //TODO: if meters pass nullptr, otherwise, pass conversion
        //NewtonConversion* units = nullptr;
        NewtonScene* scene = new NewtonScene(grav, nullptr, tutorial,problemText,this);
        scenes.push_back(scene);

        //get objects from document and populate the scene
        QJsonArray objs = currentScene["objects"].toArray();
        for(int i = 0; i < objs.size(); i++){
            QString shapeType = obj[i].toObject()["type"];

            //check for variable
            float centerX = obj[i].toObject()["centerX"].toFloat();
            float centerY = obj[j].toObject()["centerY"].toFloat();

            bool isDynamic = objs[i].toObject()["isDynamic"].toBool();

            //check for variable
            float mass = objs[i].toObject()["mass"].toDouble();


            float angle = objs[i].toObject()["angle"].toDouble();


            if(shapeType == "rect"){
                float width = objs[i].toObject()["width"].toFloat();
                float height = objs[i].toObject()["height"].toFloat();
                NewtonBody rect = new NewtonBody(isDynamic,mass,centerX,centerY,width,height,this);
                rect.setInitOrientation(objs[i].toObject()["angle"].toFloat);
                scene->addBody(rect);
            }
            else if(shapeType == "circ"){
                float radius = objs[i].toObject()["radius"].toFloat();
                NewtonBody circle = new NewtonBody(isDynamic,mass,centerX,centerY,radius,this);

                scene->addBody(circle);
            }

        }

        QJsonObject widge = currentScene["widgets"].toObject();
        QJsonArray formulas = widge["displayFormulas"].toArray();
        QJsonArray inputFields = widge["inputFieldUnits"].toArray();
        QJsonArray solvingFormulas = widge["solvingFormulas"].toArray();

        QStringList labels;
        QList<bool> labelsEnabled;

        for(int i = 0 ; i < formulas.size(); i++){
                labels.push_back(formulas[i].toString());
                labelsEnabled.push_back(false);
        }
        for(int j = 0; j < inputFields.size(); j++){
            labels.push_back(inputFields[j].toString());
            labelsEnabled.push_back(true);
        }



        //TODO: set labels and labelsEnabled datamembers

        //store units and question info tutorial


    }
}

void NewtonModel::setScene(int sceneIndex){
    if(sceneIndex < 0 ||
       sceneIndex >= (int) scenes.length() ||
       sceneIndex == currentSceneIndex){
        return;
    }

    currentSceneIndex = sceneIndex;
    NewtonScene* currentScene = scenes[currentSceneIndex];

    //TODO: clear graphicsScene
    graphicsScene->clear();
    //TODO: populate graphicsScene with the elements in the scene
    for(int i = 0; i < currentScene->getBodies().length(); i++){
        NewtonBody* curr = currentScene->getBodies()[i];
        //Add a circle
        if(curr->getShapeType() == NewtonBody::Shape::Circle){
            NewtonBody::NewtonShape shapeVal = curr->getShapeValue();
            //TODO convert to graphcis scene coordinates
            //Track the returned graphics item so we can update it
            graphicsScene->addEllipse(shapeVal.circle.cX,
                                      shapeVal.circle.cY,
                                      shapeVal.circle.r,
                                      shapeVal.circle.r);
        }
        else if(curr->getShapeType() == NewtonBody::Shape::Rect){
            NewtonBody::NewtonShape shapeVal = curr->getShapeValue();
            graphicsScene->addRect(shapeVal.rect.cX - 0.5 * shapeVal.rect.width,
                                   shapeVal.rect.cY - 0.5 * shapeVal.rect.height,
                                   shapeVal.rect.width,
                                   shapeVal.rect.height);
        }

    }
    //TODO: set Initial values for problem
    QVector<int> editableIndices;
    for(int i = 0; i < currentScene->getEditableWidgets(); i++){

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
    for(int i = 0; i < scenes.length(); i++){
        descriptions.append(scenes[i]->getTutorialText());
    }

}

void NewtonModel::validateAnswer(float answer, QVector<float>& parameters){
    //TODO: validate

    //Place holder
    emit answerValidated(true);
}

void NewtonModel::clearModel(){
    graphicsScene->clear();
}

void NewtonModel::resetGraphicsView(){
    graphs.clear();
    scenes.clear();  // Remove all elements in Qvectors
    currentSceneIndex = 0;
}
