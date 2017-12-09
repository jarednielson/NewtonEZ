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
#include <QtMath>
#include <QTimer>
#include <QDebug>

NewtonModel::NewtonModel(QObject *parent) : QObject(parent)
{
    currentSceneIndex = 0;
    graphicsScene = new QGraphicsScene();
    simTimer = new QTimer(this);
    connect(simTimer, SIGNAL(timeout()), this, SLOT(updateBodies()));
    simSingleShot = new QTimer(this);
    connect(simSingleShot, SIGNAL(timeout()), this, SLOT(endSimulation()));
}

NewtonModel::NewtonModel(QString filePath, QObject *parent) : QObject (parent)
{
    graphicsScene = new QGraphicsScene();
    loadFile(filePath);
    currentSceneIndex = 0;
    simTimer = new QTimer(this);
    connect(simTimer, SIGNAL(timeout()), this, SLOT(updateBodies()));
    simSingleShot = new QTimer(this);
    connect(simSingleShot, SIGNAL(timeout), this, SLOT(endSimulation()));
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

            //get initial velocites
            QJsonArray velocities = objs[j].toObject()["initialVelocity"].toArray();
            QPointF veloPoint(velocities[0], velocities[1]);

            //get forces
            QJsonArray forces = objs[j].toObject()["forces"].toArray();
            QPointF forcesXY(forces[0], forces[1]);

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
                rect->setInitVelocity(veloPoint);
                rect->setInitForce(forcesXY);
                scene->addBody(rect);
            }
            else if(shapeType == "circ"){
                float radius = objs[j].toObject()["radius"].toDouble();
                NewtonBody* circle = new NewtonBody(isDynamic, mass, centerX, centerY, radius, this);
                   circle->setInitVelocity(veloPoint);
                   circle->setInitForce(forcesXY);
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
            QGraphicsItem* item =graphicsScene->addEllipse(
                                      convertToPixel(-1.0 * shapeVal.circle.r),
                                      convertToPixel(-1.0 * shapeVal.circle.r),
                                      convertToPixel(shapeVal.circle.r * 2.0f),
                                      convertToPixel(shapeVal.circle.r * 2.0f));
            item->setPos(convertToPixel(curr->getInitPos().x()),
                         convertToPixel(curr->getInitPos().y()));
            scBodies.push_back(item);

        }
        else if(curr->getShapeType() == NewtonBody::Shape::Rect){
            NewtonBody::NewtonShape shapeVal = curr->getShapeValue();
            QGraphicsItem* item = graphicsScene->addRect(
                                   convertToPixel(-0.5 * shapeVal.rect.width),
                                   convertToPixel(-0.5 * shapeVal.rect.height),
                                   convertToPixel(shapeVal.rect.width),
                                   convertToPixel(shapeVal.rect.height));
            item->setPos(convertToPixel(curr->getInitPos().x()),
                         convertToPixel(curr->getInitPos().y()));
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
    //graphicsScene->clear();
    //build box2d scene
    NewtonScene* curScn = scenes[currentSceneIndex];
    b2Vec2 gravity = b2Vec2(0.0f,
                             curScn->getGravity());
    world = new b2World(gravity);
    //Loop through the scene and create bodies for all scene objects
    QVector<NewtonBody*> scNBodies = curScn->getBodies();
    for(int i = 0; i < scNBodies.length(); i++){
        NewtonBody* curNt = scNBodies[i];
        b2BodyDef bdef;
        bdef.position.Set(curNt->getInitPos().x(),
                          curNt->getInitPos().y());
        if(scNBodies[i]->isDynamic()){
            bdef.type = b2_dynamicBody;
        }
        b2Body* body = world->CreateBody(&bdef);
        if(curNt->getShapeType() == NewtonBody::Shape::Circle){
            b2CircleShape bodyShape;

            bodyShape.m_p = b2Vec2(0.0f, 0.0f);
            bodyShape.m_radius = curNt->getShapeValue().circle.r;
            float density;
            if(curNt->isDynamic()){
                density = curNt->getMass() / (M_PI *
                                              curNt->getShapeValue().circle.r *
                                              curNt->getShapeValue().circle.r);
            } else {
                density = 0.0f;
            }
            body->CreateFixture(&bodyShape, density);
        } else if(curNt->getShapeType() == NewtonBody::Shape::Rect){
            b2PolygonShape bodyShape;

            bodyShape.SetAsBox(curNt->getShapeValue().rect.width / 2,
                               curNt->getShapeValue().rect.height / 2);
            float density;
            if(curNt->isDynamic()){
                density = curNt->getMass() /
                          curNt->getShapeValue().rect.width *
                          curNt->getShapeValue().rect.height;

            } else {
                density = 0.0f;
            }
            body->CreateFixture(&bodyShape, density);
        }
        body->SetUserData(scBodies[i]);
    }

    simTimer->start(TIME_STEP * 1000);
    //nofity simulationStart
    emit simulationStart();
    //end simulation
    simSingleShot->setSingleShot(true);
    simSingleShot->start(TIME_OUT);
}

void NewtonModel::endSimulation(){
    //check to make sure scene is running
    if(!simRunning){
        return;
    }
    simRunning = false;
    //kill timer
    simSingleShot->stop();
    simTimer->stop();
    delete world;

    //Reset graphics view stuff
    QVector<NewtonBody*> ntBodies = scenes[currentSceneIndex]->getBodies();
    for(int i = 0; i < scBodies.length(); i++){
        scBodies[i]->setPos(convertToPixel(ntBodies[i]->getInitPos().x()),
                            convertToPixel(ntBodies[i]->getInitPos().y()));
    }
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

    scene->addBody(new NewtonBody(true, 10, 5, 10, 0.5, 0.5, scene));
    scene->addBody(new NewtonBody(true, 5, 10, 10, 0.25, scene));
    scene->addBody(new NewtonBody(false, 100, 10, 1, 20, 2, scene));
    scene->addDisplayWidget(QString("M1 (kg)"), 10.0f);
    scene->addDisplayWidget(QString("M2 (kg)"), 5.0f);
    scene->addEditableWidget(QString("Time (sec)"),
                             QString("(function(a, b) { return Math.sqrt(a*2/9.8);})"));

    scenes.push_back(scene);
    setScene(0);
}

void NewtonModel::updateBodies(){
    world->Step(TIME_STEP, VEL_ITER, POS_ITER);
    int bodyCount = world->GetBodyCount();
    b2Body* body = world->GetBodyList();
    for(int i = 0; i < bodyCount; i++){
        b2Vec2 pos = body->GetPosition();
        QGraphicsItem* item =
                static_cast<QGraphicsItem*>(body->GetUserData());
        item->setPos(convertToPixel(QPointF(pos.x, pos.y)));
        body = body->GetNext();
        //item->setRotation(bodies[i].GetAngle() * M_PI / 180.0f);
    }
}
void NewtonModel::clearModel(){
    if(simRunning){
        endSimulation();
    }
    graphicsScene->clear();
    scBodies.clear();
    for(int i = 0; i < scenes.length(); i++){
        delete scenes[i];
    }
    scenes.clear();
    currentSceneIndex = -1;
    //TODO: free b2d resources if needed
}

float NewtonModel::convertToPixel(float meter){
    return meter * PIX_PER_METER;
}

QPointF NewtonModel::convertToPixel(QPointF meter){
    return QPointF(meter.x() * PIX_PER_METER,
                   meter.y() * PIX_PER_METER);
}
