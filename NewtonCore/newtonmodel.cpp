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

        QJsonObject varVals = currentScene["varVals"].toObject();
        QMap<QString, float> chosenRangeValues;
        for(QString key : varVals.keys()){
            // Extract ranges for each variable
            float lowerBound = varVals[key].toArray()[0].toDouble();
            float upperBound = varVals[key].toArray()[1].toDouble();

            // Randomly select a value from the given range
            float calculated = lowerBound + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(upperBound-lowerBound)));

            chosenRangeValues.insert(key,calculated);
        }


        //Swapping in ranged value pairs for the problem text
        QString problemText(currentScene["problemText"].toString());

        for(float val : chosenRangeValues.values()){
            problemText = problemText.arg(val);
        }

        //TODO: if meters pass nullptr, otherwise, pass conversion
        //NewtonConversion* units = nullptr;
        NewtonScene* scene = new NewtonScene(grav, nullptr, tutorial, problemText, this);
        scenes.push_back(scene);

        //get objects from document and populate the scene
        QJsonArray objs = currentScene["objects"].toArray();
        for(size_t j = 0; j < objs.size(); j++){
            QJsonObject obj = objs[j].toObject();
            QString shapeType = obj["type"].toString();
            QString indexKey;

            //get initial velocites
            QJsonArray velocities = obj["initialVelocity"].toArray();
            float velX;
            float velY;
            if(velocities[0].isDouble()){
                velX = velocities[0].toDouble();
            } else {
                indexKey = velocities[0].toString();
                velX = chosenRangeValues[indexKey];
            }
            if(velocities[1].isDouble()){
                velY = velocities[1].toDouble();
            } else {
                indexKey = velocities[1].toString();
                velY = chosenRangeValues[indexKey];
            }
            QPointF veloPoint(velX, velY);

            //get forces
            QJsonArray forces = obj["forces"].toArray();
            float fX;
            float fY;
            if(forces[0].isDouble()){
                fX = forces[0].toDouble();
            } else {
                indexKey = velocities[0].toString();
                fX = chosenRangeValues[indexKey];
            }
            if(forces[1].isDouble()){
                fY = forces[1].toDouble();
            } else {
                indexKey = forces[1].toString();
                fY = chosenRangeValues[indexKey];
            }
            QPointF forcesXY(fX, fY);

            float centerX;
            float centerY;

            //check for variable
            if(obj["centerX"].isDouble()){
                centerX = obj["centerX"].toDouble();
            }
            else{
                indexKey = obj["centerX"].toString();
                centerX = chosenRangeValues[indexKey];
            }
            if(obj["centerY"].isDouble()){
                centerY= obj["centerY"].toDouble();
            }
            else{
                indexKey = obj["centerY"].toString();
                centerY = chosenRangeValues[indexKey];
            }
            //TODO: Get width, height, radius
            bool isDynamic = obj["isDynamic"].toBool();

            //check for variable
            float mass;
            if(obj["mass"].isDouble()){
                mass = obj["mass"].toDouble();
            }
            else{
                indexKey = obj["mass"].toString();
                mass = chosenRangeValues[indexKey];
            }
            float angle;
            if(obj["angle"].isDouble()){
                angle = obj["angle"].toDouble();
            } else {
                indexKey = obj["angle"].toString();
                mass = chosenRangeValues[indexKey];
            }

            if(shapeType == "rect"){
                float width;
                float height;
                if(obj["width"].isDouble()){
                    width = obj["width"].toDouble();
                } else {
                    indexKey = obj["width"].toString();
                    width = chosenRangeValues[indexKey];
                }
                if(obj["height"].isDouble()){
                    height = obj["height"].toDouble();
                } else {
                    indexKey = obj["height"].toString();
                    height = chosenRangeValues[indexKey];
                }

                NewtonBody* rect = new NewtonBody(isDynamic, (float) mass, (float) centerX, (float) centerY, (float) width, (float) height, this);
                rect->setInitOrientation(obj["angle"].toDouble());
                rect->setInitVelocity(veloPoint);
                rect->setInitForce(forcesXY);
                scene->addBody(rect);
            }
            else if(shapeType == "circ"){
                float radius;
                if(obj["radius"].isDouble()){
                    radius = obj["radius"].toDouble();
                } else {
                    indexKey = obj["radius"].toString();
                    radius = chosenRangeValues[indexKey];
                }
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
        problemsComplete.push_back(false);
    }
    setScene(0);
}

void NewtonModel::setScene(int sceneIndex){
    if(sceneIndex < 0 ||
       sceneIndex >= (int) scenes.length() ||
       sceneIndex == currentSceneIndex){
        return;
    }
    clearScene();
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

    //Right now we are assuming EVERY display widget is used in order for the func
    QVector<float> args;
    for(int i = 0; i < currentScene->getDisplayWidgetValues().length(); i++){
        args.push_back(currentScene->getDisplayWidgetValues()[i]);
    }
    //All functions must use args in order
    //Calculate values for each function
    QList<NewtonFormula*> formulas = currentScene->getEditableFunctions();
    for(int i = 0; i < formulas.length(); i++){
        if(formulas[i]->isValid()){
            answers.push_back(formulas[i]->evaluate(args));
        } else {
            answers.push_back(0.0f);
        }

    }
    //notify widgets changed
    emit displayWidgetsChanged(currentScene->getDisplayWidgetLabels(),
                               currentScene->getDisplayWidgetValues());
    emit inputWidgetsChanged(currentScene->getEditableWidgetLabels());
    //notify instructionTextChanged
    emit briefTextChanged(currentScene->getBriefDescription());
}

void NewtonModel::nextScene(){
    if(currentSceneIndex < scenes.length() - 1){
        setScene(currentSceneIndex + 1);
        if(currentSceneIndex == scenes.length() - 1){
            emit lastSceneSelected();
        }
    }
}

void NewtonModel::prevScene(){
    if(currentSceneIndex > 0){
        setScene(currentSceneIndex - 1);
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
    if(scenes.isEmpty()){
        emit simulationEnd();
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
        QPointF initVelocity = curNt->getInitVelocity();
        body->SetLinearVelocity(b2Vec2(initVelocity.x(), initVelocity.y()));
        body->SetUserData(scBodies[i]);
    }

    simRuntime.start();
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
    if(scenes.length() == 0){
        emit answerValidated(validaters);
        return;
    }
    bool allCorrect = true;
    for(int i = 0; i < answers.length() || i < NewtonModel::answers.length(); i++){
        float diff = NewtonModel::answers[i] - answers[i];
        if(diff < 0){
            diff = diff * -1;
        }
        validaters.push_back(diff < TOL);
        allCorrect = allCorrect && diff < TOL;
    }

    if(int diff = answers.length() - NewtonModel::answers.length() > 0){
        allCorrect = false;
        for(int i = 0; i < diff; i++){
            validaters.push_back(false);
        }
    }
    problemsComplete[currentSceneIndex] = allCorrect;
    int numCorrect = 0;
    for(int i = 0; i < problemsComplete.length(); i++){
        if(problemsComplete[i]){
            numCorrect++;
        }
    }
    emit progressUpdate((numCorrect / (float)problemsComplete.length()) * 100);
    emit answerValidated(validaters);
}

void NewtonModel::loadDefaultScene(){
    clearModel();
    NewtonScene* scene = new NewtonScene(-9.8f,
                                         Q_NULLPTR,
                                         QString("Projectile Motion"),
                                         QString("An object with mass M1 = 10Kg is dropped from a height of ") +
                                         QString("H1 = 10m. A second object has mass M1 = 5Kg and height H2 = 5m ") +
                                         QString("and an initial y velocity Vy0 = 5 m/sec.\n") +
                                         QString("If gravity is -9.8 m/sec^2 at what time will ") +
                                         QString("they hit the ground?"),
                                         this);

    scene->addBody(new NewtonBody(true, 10, 5, 10, 0.5, 0.5, scene));
    scene->addBody(new NewtonBody(true, 5, 10, 5, 0.25, scene));
    scene->getBodies()[1]->setInitVelocity(QPointF(0.0f, 5.0f));
    scene->addBody(new NewtonBody(false, 100, 10, -1, 20, 2, scene));
    scene->addDisplayWidget(QString("H1 (m)"), 10.0f);
    scene->addDisplayWidget(QString("H2 (m)"), 5.0f);
    scene->addDisplayWidget(QString("Vy0 (m/sec)"), 5.0f);
    scene->addDisplayWidget(QString("M1 (Kg)"), 10.0f);
    scene->addDisplayWidget(QString("M2 (Kg)"), 5.0f);
    scene->addEditableWidget(QString("T1 (sec)"),
                             QString("(function(a, b, c, d) { return Math.sqrt(a*2/9.8);})"));
    scene->addEditableWidget(QString("T2 (sec)"),
                             QString("(function(a, c, b, d) { return (-1*b - Math.sqrt(b*b + 4 * 4.8 *  c)) / (-9.8);})"));

    scenes.push_back(scene);
    problemsComplete.push_back(false);
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
    emit(updateClock(simRuntime.elapsed()/1000.0));
}
void NewtonModel::clearModel(){
    clearScene();
    for(int i = 0; i < scenes.length(); i++){
        delete scenes[i];
    }
    problemsComplete.clear();
    scenes.clear();
    currentSceneIndex = -1;
    emit progressUpdate(0);
}

void NewtonModel::clearScene(){
    if(simRunning){
        endSimulation();
    }
    graphicsScene->clear();
    scBodies.clear();
    answers.clear();
}

float NewtonModel::convertToPixel(float meter){
    return meter * PIX_PER_METER;
}

QPointF NewtonModel::convertToPixel(QPointF meter){
    return QPointF(meter.x() * PIX_PER_METER,
                   meter.y() * PIX_PER_METER);
}
