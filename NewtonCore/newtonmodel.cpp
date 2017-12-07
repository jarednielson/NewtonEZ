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
        QMap<QString, float> chosenRangeValues;
        for(int curRange = 0; curRange < varVals.size(); curRange++){
            QString indexKey = "{"+ QString::number(curRange) +"}";
            float lowerBound = varVals[curRange].toArray()[0].toDouble();
            float upperBound = varVals[curRange].toArray()[1].toDouble();

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
        NewtonScene* scene = new NewtonScene(grav, nullptr, tutorial,problemText,this);
        scenes.push_back(scene);

        //get objects from document and populate the scene
        QJsonArray objs = currentScene["objects"].toArray();
        for(size_t j = 0; j < objs.size(); j++){
            QString shapeType = objs[i].toObject()["type"].toString();
            float centerX;
            float centerY;

            //check for variable
            if(std::all_of(objs[i].toObject()["centerX"].toString().toStdString().begin(), objs[i].toObject()["centerX"].toString().toStdString().end(), ::isdigit)){
                centerX = objs[i].toObject()["centerX"].toDouble();
            }
            else{
                centerX = chosenRangeValues[(QString("{").append(QString().setNum(j)).append("}"))];
            }
            if(std::all_of(objs[i].toObject()["centerY"].toString().toStdString().begin(),objs[i].toObject()["centerY"].toString().toStdString().end(), ::isdigit)){
                centerY= objs[i].toObject()["centerY"].toDouble();
            }
            else{
                centerY = chosenRangeValues[(QString("{").append(QString().setNum(j)).append("}"))];
            }
            bool isDynamic = objs[i].toObject()["isDynamic"].toBool();

            //check for variable
            float mass;
            if(std::all_of(objs[i].toObject()["mass"].toString().toStdString().begin(),objs[i].toObject()["mass"].toString().toStdString().end(), ::isdigit)){
                mass = objs[i].toObject()["mass"].toDouble();
            }
            else{
                mass = chosenRangeValues[(QString("{").append(QString().setNum(j)).append("}"))];
            }

            float angle = objs[i].toObject()["angle"].toDouble();

            if(shapeType == "rect"){
                float width = objs[i].toObject()["width"].toDouble();
                float height = objs[i].toObject()["height"].toDouble();
                NewtonBody* rect = new NewtonBody(isDynamic, (float) mass, (float) centerX, (float) centerY, (float) width, (float) height, this);
                rect->setInitOrientation(objs[i].toObject()["angle"].toDouble());
                scene->addBody(rect);
            }
            else if(shapeType == "circ"){
                float radius = objs[i].toObject()["radius"].toDouble();
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
        QJsonArray labels = objs[i].toObject()["varValLabels"].toArray();
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
            graphicsScene->addEllipse(curr->getInitPos().x(),
                                      curr->getInitPos().y(),
                                      shapeVal.circle.r,
                                      shapeVal.circle.r);
        }
        else if(curr->getShapeType() == NewtonBody::Shape::Rect){
            NewtonBody::NewtonShape shapeVal = curr->getShapeValue();
            graphicsScene->addRect(curr->getInitPos().x()  - 0.5 * shapeVal.rect.width,
                                   curr->getInitPos().y() - 0.5 * shapeVal.rect.height,
                                   shapeVal.rect.width,
                                   shapeVal.rect.height);
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
    emit inputWidgetsChanged(currentScene->getEditableWidgetLabels());
    emit displayWidgetsChanged(currentScene->getDisplayWidgetLabels(),
                               currentScene->getDisplayWidgetValues());
    //notify instructionTextChanged
    emit instructionTextChanged(currentScene->getTutorialText());
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

void NewtonModel::validateAnswer(float answer){
    float diff = NewtonModel::answer - answer;
    if(diff < 0){
        diff = diff * -1;
    }

    emit answerValidated(diff < TOL);
}

void NewtonModel::clearModel(){
    graphicsScene->clear();
}

void NewtonModel::resetGraphicsView(){
    graphs.clear();
    scenes.clear();  // Remove all elements in Qvectors
    currentSceneIndex = 0;
}
