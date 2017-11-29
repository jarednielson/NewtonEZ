#include "newtondatamodels.h"
#include <QString>

NewtonBody::NewtonBody(QObject *parent) :
    NewtonSceneObject(QPointF(0.0f, 0.0f), parent),
    m_isDynamic(false),
    m_Mass(1.0f)
{
    hull.push_back(QPointF(0.0f, 0.0f));
}

NewtonBody::NewtonBody(bool isDynamic, float mass, const QVector<QPointF> &hull, QObject *parent) :
    NewtonSceneObject(hull[0], parent),
    m_isDynamic(isDynamic),
    m_Mass(mass)
{
    for(QVector<QPointF>::const_iterator it = hull.begin(); it != hull.end(); it++){
        NewtonBody::hull.push_back(*it);
    }
}

bool NewtonBody::isDynamic(){
    return m_isDynamic;
}

float NewtonBody::getMass(){
    return m_Mass;
}

void NewtonBody::setMass(float mass){
    if(mass <= 0){
        mass = 1;
    }

    emit massChanged(mass);
    m_Mass = mass;
}

QVector<QPointF>& NewtonBody::getVertices(){
    return hull;
}

QString NewtonBody::serialize(){
    return "";
}

