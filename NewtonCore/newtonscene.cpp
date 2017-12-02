#include "newtondatamodels.h"

NewtonScene::NewtonScene(QObject *parent) : QObject(parent)
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

QStringList& NewtonScene::getWidgetLabels(){
    return widgetLabels;
}

QList<QPair<float, float>>& NewtonScene::getWidgetValueRanges(){
    return widgetValueRanges;
}

QList<bool>& NewtonScene::getEditableWidgets(){
    return editableWidgets;
}

void NewtonScene::addWidget(QString label, bool editable, QPair<float, float> range){
    widgetLabels.append(label);
    editableWidgets.append(editable);
    widgetValueRanges.append(range);
}
