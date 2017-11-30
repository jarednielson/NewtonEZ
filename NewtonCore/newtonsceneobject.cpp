#include "newtondatamodels.h"
#include <QObject>
#include <QPointF>

NewtonSceneObject::NewtonSceneObject(QObject *parent) : NewtonObject(parent){

}

NewtonSceneObject::NewtonSceneObject(QPointF initPos, QObject *parent) : NewtonObject(parent),
    m_initPos(initPos){

}

QPointF NewtonSceneObject::getInitPos() const{
    return m_initPos;
}

void NewtonSceneObject::setInitPos(QPointF pos){
    m_initPos = pos;
    emit initPosChanged(m_initPos);
}
