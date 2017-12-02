#include "newtondatamodels.h"
#include <QObject>
#include <QPointF>

NewtonSceneObject::NewtonSceneObject(QObject *parent) : NewtonObject(parent){

}

NewtonSceneObject::NewtonSceneObject(QPointF initPos, QObject *parent) : NewtonObject(parent),
    m_initPos(initPos),
    m_initOrientation(0)
{

}

QPointF NewtonSceneObject::getInitPos() const{
    return m_initPos;
}

void NewtonSceneObject::setInitPos(QPointF pos){
    m_initPos = pos;
    emit initPosChanged(m_initPos);
}


float NewtonSceneObject::getInitOrientation(){
    return m_initOrientation;
}

void NewtonSceneObject::setInitOrientation(float theta){
    m_initOrientation = theta;
}
