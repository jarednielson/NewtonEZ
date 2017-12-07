#include "newtondatamodels.h"
#include "newtonformula.h"

NewtonScene::NewtonScene(QObject *parent) : QObject(parent),
    m_Gravity(-9.8),
    tutorial(""),
    briefDescription("")
{

}

NewtonScene::NewtonScene(float gravity, NewtonConversion* units, QString tutorial, QString brief, QObject *parent) : QObject(parent),
    m_Gravity(gravity),
    units(units),
    tutorial(tutorial),
    briefDescription(brief)
{

}

float NewtonScene::getGravity() const{
    return m_Gravity;
}

void NewtonScene::setGravity(float gravity){
    m_Gravity = gravity;
    emit gravityChanged(m_Gravity);
}

NewtonConversion* NewtonScene::getUnits() const{
    return units;
}

void NewtonScene::addBody(NewtonBody* body){
    bodies.push_back(body);
}

const QVector<NewtonBody*>& NewtonScene::getBodies() const {
    return bodies;
}

QString NewtonScene::getTutorialText(){
    return tutorial;
}

QString NewtonScene::getBriefDescription(){
    return briefDescription;
}

QStringList& NewtonScene::getDisplayWidgetLabels(){
    return displayWidgetLabels;
}

QList<float>& NewtonScene::getDisplayWidgetValues(){
    return displayWidgetValues;
}

QStringList& NewtonScene::getEditableWidgetLabels(){
    return editableWidgetLabels;
}

QList<NewtonFormula*>& NewtonScene::getEditableFunctions(){
    return functions;
}

void NewtonScene::addDisplayWidget(QString label, float value)
{
    displayWidgetLabels.push_back(label);
    displayWidgetValues.push_back(value);
}
void NewtonScene::addEditableWidget(QString label, QString formula){
    editableWidgetLabels.push_back(label);
    NewtonFormula* f = new NewtonFormula(formula);
    if(f->isValid()){
        functions.push_back(f);
    } else {
        functions.push_back(Q_NULLPTR);
    }
}
