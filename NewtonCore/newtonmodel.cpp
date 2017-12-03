#include <QGraphicsScene>
#include <QObject>
#include "newtonmodel.h"
#include "QFile"
#include "QJsonObject"
#include "QJsonArray"
#include "QJsonDocument"


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

        //TODO: if meters pass nullptr, otherwise, pass conversion
        //NewtonConversion* units = nullptr;
        NewtonScene* scene = new NewtonScene(grav, nullptr, tutorial,problemText,this);
        scenes.push_back(scene);

        //get objects from document and populate the scene
        QJsonArray objs = currentScene["objects"].toArray();
        for(int i = 0; i < objs.size(); i++){
            //TODO: test
            QVector<QPointF>* vertPoints = new QVector<QPointF>;
            QJsonArray verts = objs[i].toObject()["verts"].toArray();
            for(int j =0; j < verts.size(); j++){
                QString ptstr = verts[j].toString();
                QStringList xy = ptstr.split(',');
                float y = xy.back().toFloat();
                xy.pop_back();
                float x =  xy.back().toFloat();
                xy.pop_back();
                QPointF point(x,y);
                vertPoints->push_back(point);
            }
            bool isDynamic = objs[i].toObject()["isDynamic"].toBool();
            float mass = objs[i].toObject()["mass"].toDouble();
            float angle = objs[i].toObject()["angle"].toDouble();

            //TODO: add angle
            scene->addBody(new NewtonBody(isDynamic,mass,*vertPoints,this));
        }

        QJsonObject widge = currentScene["widgets"].toObject();
        QJsonArray formulas = widge["formulas"].toArray();
        QJsonArray inputFields = widge["inputFieldUnits"].toArray();

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

// TODO: Test using formulas from json doc. Currently I'm not sure if the x1, x2, ..., xn variable naming scheme will work with
// the formula parser. I really hope it does, otherwise we need to come up with a different variable naming scheme that will
// allow us to iterate through the variables, inserting their respective values. If it doesn't work, I'll probably just use
// a, b, c, ..., z and iterate over their ASCII values. I can't imagine us ever using any more than 26 variables.
double NewtonModel::evaluateFormula(QString formula, QVector<double> varVals)
{
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double>     expression_t;
    typedef exprtk::parser<double>             parser_t;

    std::string expressionStr = formula.toStdString();

    // Register formula variables with the symbol_table. Variables will be of the form x1, x2, x3, ..., xn
    symbol_table_t symbolTable;
    for(int i = 0; i < varVals.size(); i++)
    {
        symbolTable.add_variable("x"+std::to_string(i+1),varVals[i]);
    }

    // Instantiate expression and register symbol_table
    expression_t expression;
    expression.register_symbol_table(symbolTable);

    // Instantiate parser and compile the expression
    parser_t parser;
    parser.compile(expressionStr, expression);

    double result = expression.value();

    return result;
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


