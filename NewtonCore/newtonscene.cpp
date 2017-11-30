#include "newtondatamodels.h"

NewtonScene::NewtonScene(QObject *parent) : QObject(parent)
{

}

NewtonScene::NewtonScene(float gravity, NewtonConversion* units, QObject *parent) : QObject(parent),
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

QString getTutorialText(){
    return tutorial;
}

QString getBriefDescription(){
    return briefDescription;
}
