#include <QGraphicsScene>
#include <QObject>
#include "newtonmodel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>

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
    clearModel();
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
    clearModel();
    //open stream and read next unit as string
    QFile nextUnitFile(filePath);
    nextUnitFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray nextUnit = nextUnitFile.readAll();
    nextUnitFile.close();

    //deserialize json doc
    QJsonDocument doc = QJsonDocument::fromJson(nextUnit);
    QJsonObject unit = doc.object();
    QJsonArray scenelist = unit["Problems"].toArray();

    //for each scene grab info
    for(size_t i = 0; i < scenelist.size(); i++){

        //deserialize scene information
        QJsonObject currentScene = scenelist[i].toObject();
        float grav = currentScene["gravity"].toDouble();
        QString tutorial(currentScene["tutorial"].toString());



        QJsonArray varVals = currentScene["varVals"].toArray();
        QMap<QString, float> chosenRangeValues;
        for(int curRange = 0; curRange < varVals.size(); curRange++){
            QString indexKey = "{"+ QString::number(curRange) +"}";

            // Extract ranges for each variable
            float lowerBound = varVals[curRange].toObject()[indexKey].toArray()[0].toDouble();
            float upperBound = varVals[curRange].toObject()[indexKey].toArray()[1].toDouble();

            // Randomly select a value from the given range
            float calculated = lowerBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(upperBound-lowerBound)));

            chosenRangeValues.insert(indexKey,calculated);
        }


        //Swapping in ranged value pairs for the problem text
        QString problemText(currentScene["problemText"].toString());

        for(QString key : chosenRangeValues.keys()){
            int len;
            while( (len = problemText.indexOf(key)) != -1){
                problemText.replace(len,key.length(), QString().setNum(chosenRangeValues[key]));
            }
        }

        //TODO: if meters pass nullptr, otherwise, pass conversion
        //NewtonConversion* units = nullptr;
        NewtonScene* scene = new NewtonScene(grav, nullptr, tutorial, problemText, this);
        scenes.push_back(scene);

        //get objects from document and populate the scene
        QJsonArray objs = currentScene["objects"].toArray();
        for(size_t j = 0; j < objs.size(); j++){
            QString shapeType = objs[j].toObject()["type"].toString();
            QString indexKey = "{" + QString::number(j) + "}";

            float centerX;
            float centerY;

            //check for variable
            if(objs[j].toObject()["centerX"].toString()[0] !='{'){
                centerX = objs[j].toObject()["centerX"].toDouble();
            }
            else{
                centerX = chosenRangeValues[indexKey];
            }
            if(objs[j].toObject()["centerY"].toString()[0] !='{'){
                centerY= objs[j].toObject()["centerY"].toDouble();
            }
            else{
                centerY = chosenRangeValues[indexKey];
            }
            //TODO: Get width, height, radius
            bool isDynamic = objs[j].toObject()["isDynamic"].toBool();

            //check for variable
            float mass;
            if(objs[j].toObject()["mass"].toString()[0] !='{'){
                mass = objs[j].toObject()["mass"].toDouble();
            }
            else{
                mass = chosenRangeValues[indexKey];
            }

            float angle = objs[j].toObject()["angle"].toDouble();

            if(shapeType == "rect"){
                float width = objs[j].toObject()["width"].toDouble();
                float height = objs[j].toObject()["height"].toDouble();
                NewtonBody* rect = new NewtonBody(isDynamic, (float) mass, (float) centerX, (float) centerY, (float) width, (float) height, this);
                rect->setInitOrientation(objs[j].toObject()["angle"].toDouble());
                scene->addBody(rect);
            }
            else if(shapeType == "circ"){
                float radius = objs[j].toObject()["radius"].toDouble();
                NewtonBody* circle = new NewtonBody(isDynamic, mass, centerX, centerY, radius, this);

                scene->addBody(circle);
            }

        }
        QJsonArray funcs= currentScene["solvingFormulas"].toArray();
        QJsonArray inputWidgetUnits = currentScene["inputWidgetUnits"].toArray();

        for(size_t j=0;j<inputWidgetUnits.size(); j++){
            scene->addEditableWidget(inputWidgetUnits[j].toString(),funcs[j].toString());
        }
        QList<float> rangeVals = chosenRangeValues.values();
        QJsonArray labels = currentScene["varValLabels"].toArray();
        for(size_t j = 0; j < labels.size(); j++){
            scene->addDisplayWidget(labels[j].toString(),rangeVals[j]);
        }

        setScene(0);
    }
}

double NewtonModel::evaluateFormulas(QJsonArray& formulas, QVector<float>& varVals)
{
    QVector<QString> stringFormulas;

    // Will match with any variable in the formulas and replace them with the chosen values
    QRegularExpression regExp("\\[([a-z])\\]");
    QRegularExpressionMatchIterator matches;

    QScriptEngine eng;
    QVector<double> answers;

    // Replaces known variables with their values, adds the modified formulas to stringFormulas
    for(int i = 0; i < formulas.size(); i++)
    {
        QString formula = formulas[i].toString();
        matches = regExp.globalMatch(formula);

        // Extracts each variable in the formula and replaces it with its corresponding value
        // (e.g., a maps to index 0 of the varVals array, b maps to 1, and so on)
        while(matches.hasNext())
        {
            QRegularExpressionMatch match = matches.next();

            QChar variable = match.captured(1)[0];
            int asciiVal = variable.toLatin1();

            // 97 is the ascii value for 'a'
            double varVal = varVals[asciiVal - 97];
            formula.replace(match.captured(0), QString::number(varVal));
        }

        stringFormulas.push_back(formula);
    }

    // Begin solving the formulas
    regExp.setPattern("\\[(\\d)\\]");

    for(int i = 0; i < stringFormulas.size(); i++)
    {
        // Check for any unknowns. These values will be reliant upon a previous formula
        matches = regExp.globalMatch(stringFormulas[i]);

        while(matches.hasNext())
        {
            QRegularExpressionMatch match = matches.next();

            double val = answers[match.captured(1).toInt()];

            stringFormulas[i].replace(match.captured(0), QString::number(val));
        }

        QScriptValue answer = eng.evaluate(stringFormulas[i]);

        answers.push_back(answer.toNumber());

    }

    return answers.last();


}
void NewtonModel::setScene(int sceneIndex){
    if(sceneIndex < 0 ||
       sceneIndex >= (int) scenes.length() ||
       sceneIndex == currentSceneIndex){
        return;
    }

    currentSceneIndex = sceneIndex;
    NewtonScene* currentScene = scenes[currentSceneIndex];

    //populate graphicsScene with the elements in the scene
    graphicsScene->clear();
    for(int i = 0; i < currentScene->getBodies().length(); i++){
        NewtonBody* curr = currentScene->getBodies()[i];
        //Add a circle
        if(curr->getShapeType() == NewtonBody::Shape::Circle){
            NewtonBody::NewtonShape shapeVal = curr->getShapeValue();
            //TODO convert to graphcis scene coordinates
            //Track the returned graphics item so we can update it
            QGraphicsItem* item =graphicsScene->addEllipse(curr->getInitPos().x() - 0.5 * shapeVal.circle.r,
                                      curr->getInitPos().y() - 0.5 * shapeVal.circle.r,
                                      shapeVal.circle.r,
                                      shapeVal.circle.r);
            scBodies.push_back(item);

        }
        else if(curr->getShapeType() == NewtonBody::Shape::Rect){
            NewtonBody::NewtonShape shapeVal = curr->getShapeValue();
            QGraphicsItem* item = graphicsScene->addRect(curr->getInitPos().x()  - 0.5 * shapeVal.rect.width,
                                   curr->getInitPos().y() - 0.5 * shapeVal.rect.height,
                                   shapeVal.rect.width,
                                   shapeVal.rect.height);
            scBodies.push_back(item);
        }

    }
    //set Initial value for problem
    //Search for first editable widget
    NewtonFormula* f = currentScene->getEditableFunctions()[0];
    //Right now we are assuming EVERY display widget is used in order for hte func
    QVector<float> args;
    for(int i = 0; i < currentScene->getDisplayWidgetValues().length(); i++){
        args.push_back(currentScene->getDisplayWidgetValues()[i]);
    }

    answer = f->evaluate(args);
    //notify widgets changed
    emit displayWidgetsChanged(currentScene->getDisplayWidgetLabels(),
                               currentScene->getDisplayWidgetValues());
    emit inputWidgetsChanged(currentScene->getEditableWidgetLabels());
    //notify instructionTextChanged
    emit briefTextChanged(currentScene->getBriefDescription());
    //TODO: clear any box2d stuff
}

void NewtonModel::nextScene(){
    if(currentSceneIndex < scenes.length() - 1){
        setScene(++currentSceneIndex);
        if(currentSceneIndex == scenes.length() - 1){
            emit lastSceneSelected();
        }
    }
}

void NewtonModel::prevScene(){
    if(currentSceneIndex > 0){
        setScene(--currentSceneIndex);
        if(currentSceneIndex == 0){
            emit firstSceneSelected();
        }
    }
}

void NewtonModel::startSimulation(){
    //check to make sure scene isn't running
    if(simRunning){
        return;
    }
    simRunning = true;
    graphicsScene->clear();
    //build box2d scene
    NewtonScene* curScn = scenes[currentSceneIndex];
    b2Vec2 gravity = b2Vec2(0.0f,
                             curScn->getGravity());
    b2World world(gravity);
    //Loop through the scene and create bodies for all scene objects
    QVector<NewtonBody*> scBodies = curScn->getBodies();
    for(int i = 0; i < scBodies.length(); i++){

        b2BodyDef bdef;
        bdef.position.set(scBodies[i]->getInitPos().x(),
                          scBodies[i]->getInitPos().y());
        if(scBodies[i]->isDynamic()){
            bdef.type = b2_dynamicBody;
        }
    }
    //TODO: create timer and callbacks
    //TODO: nofity simulationStart
    emit simulationStart();
    //TODO: run simulation
    //TODO: update graphics view as simulation goes
    //TODO: end simulation
    //TODO: reset graphicsScene
    graphicsScene->clear();
    //TODO: notify end simulation
    emit simulationEnd();
}

void NewtonModel::endSimulation(){
    //TODO: check to make sure scene is running
    //TODO: kill timer and callbacks
    //TODO: restore graphisScene
    graphicsScene->clear();
    //notify end simulation
    emit simulationEnd();
}

void NewtonModel::getProblemDescriptions(QStringList& descriptions){
    for(int i = 0; i < scenes.length(); i++){
        descriptions.append(scenes[i]->getTutorialText());
    }

}

void NewtonModel::validateAnswer(QVector<float> answers){
    QVector<bool> validaters;
    //TODO: update anwswer to list of answers for multi input
    for(int i = 0; i < answers.length(); i++){
        float diff = NewtonModel::answer - answers[i];
        if(diff < 0){
            diff = diff * -1;
        }
        validaters.push_back(diff < TOL);
    }

    emit answerValidated(validaters);
}

void NewtonModel::loadDefaultScene(){
    clearModel();

    NewtonScene* scene = new NewtonScene(-9.8f,
                                         Q_NULLPTR,
                                         QString("Two objects with mass m1 = 10kg and m2 = 5 kg ") +
                                         QString("are dropped from a height of 10 m. ") +
                                         QString("If gravity is -9.8 m/sec^2 at what time will ") +
                                         QString("they hit the ground?"),
                                         QString("Two Objects Falling"),
                                         this);

    scene->addBody(new NewtonBody(true, 10, 5, 10, 5, 5, scene));
    scene->addBody(new NewtonBody(true, 5, 20, 10, 5, scene));
    scene->addDisplayWidget(QString("M1 (kg)"), 10.0f);
    scene->addDisplayWidget(QString("M2 (kg)"), 5.0f);
    scene->addEditableWidget(QString("Time (sec)"),
                             QString("(function(a, b) { return Math.sqrt(a*2/9.8);})"));

    scenes.push_back(scene);
    setScene(0);
}

void NewtonModel::clearModel(){
    graphicsScene->clear();
    for(int i = 0; i < scenes.length(); i++){
        delete scenes[i];
    }
    scenes.clear();
    currentSceneIndex = -1;
    //TODO: free b2d resources if needed
}
