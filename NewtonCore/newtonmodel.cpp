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
    for(size_t i = 0; i < scenelist.size(); i++){

        //deserialize scene information
        QJsonObject currentScene = scenelist[i].toObject();
        float grav = currentScene["gravity"].toDouble();
        QString tutorial(currentScene["tutorial"].toString());



        QJsonArray varVals = currentScene["varVals"].toArray();
        std::map<std::string, float> chosenRangeValues;
        for(int curRange = 0; curRange < valVals.size(); curRange++){
            std::string currentKey = "{"+curRange+"}";
            QJsonArray range = varVals[i].toObject()[currentKey];


            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<> distr(range[0], range[1]); // define the range
            float calculated = distr(eng); //calculate our value
            chosenRangeValues.insert(currentKey,calculated);
        }


        //Swapping in ranged value pairs for the problem text
        QString problemText(currentScene["problemText"].toString());

        for(std::pair<std::string, float> pair : chosenRangeValues){
            size_t len;
            while( len = problemText.indexOf(pair.first)!=0){
                problemText.replace(len,pair.first.length(), QString(pair.second));
            }
        }




        //TODO: if meters pass nullptr, otherwise, pass conversion
        //NewtonConversion* units = nullptr;
        NewtonScene* scene = new NewtonScene(grav, nullptr, tutorial,problemText,this);
        scenes.push_back(scene);

        //get objects from document and populate the scene
        QJsonArray objs = currentScene["objects"].toArray();
        for(size_t j = 0; j < objs.size(); j++){
            QString shapeType = obj[i].toObject()["type"];
            float centerX;
            float centerY;

            //check for variable
            if(std::all_of(obj[i].toObject()["centerX"].toString().begin(),obj[i].toObject()["centerX"].toString().end(), ::isdigit)){
                centerX = obj[i].toObject()["centerX"].toFloat();
            }
            else{
                centerX = chosenRangeValues.at("{"+j+"}");
            }
            if(std::all_of(obj[i].toObject()["centerY"].toString().begin(),obj[i].toObject()["centerY"].toString().end(), ::isdigit)){
                centerY= obj[i].toObject()["centerY"].toFloat();
            }
            else{
                centerY = chosenRangeValues.at("{"+j+"}");
            }

            bool isDynamic = objs[i].toObject()["isDynamic"].toBool();

            //check for variable
            float mass = objs[i].toObject()["mass"].toDouble();


            float angle = objs[i].toObject()["angle"].toDouble();


            if(shapeType == "rect"){
                float width = objs[i].toObject()["width"].toFloat();
                float height = objs[i].toObject()["height"].toFloat();
                NewtonBody rect = new NewtonBody(isDynamic,mass,centerX,centerY,width,height,this);
                rect.setInitOrientation(objs[i].toObject()["angle"].toFloat());
                scene->addBody(rect);
            }
            else if(shapeType == "circ"){
                float radius = objs[i].toObject()["radius"].toFloat();
                NewtonBody circle = new NewtonBody(isDynamic,mass,centerX,centerY,radius,this);

                scene->addBody(circle);
            }

        }

        QJsonArray referenceFormulas = currentScene["displayWidgets"].toArray();
        QJsonArray funcs= currentScene["solvingFormulas"].toArray();
        QJsonArray inputWidgetUnits = currentScene["inputWidgetUnits"].toArray();

        //TODO: RESOLVE!!!!
        for(size_t j = 0; j < referenceFormulas.size(); j++ ){
            //scene->addWidget("",false,);  //Are we taking care of this with a reference sheet?
        }

        for(size_t j=0;j<inputWidgetUnits.size(); j++){
            scene->addWidget(inputWidgetUnits[j], true,chosenRangeValues.at("{"+j+"}"),funcs[j]);

        }

        setScene(0);
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
