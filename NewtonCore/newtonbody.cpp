#include "newtondatamodels.h"
#include <QString>

NewtonBody::NewtonBody(QObject *parent) :
    NewtonSceneObject(QPointF(0.0f, 0.0f), parent),
    m_isDynamic(false),
    m_Mass(1.0f)
{
    m_shape = NewtonBody::Rect;
    shapeVal.rect.cX = 0.0f;
    shapeVal.rect.cY = 0.0f;
    shapeVal.rect.width = 1.0f;
    shapeVal.rect.height= 1.0f;
}

NewtonBody::NewtonBody(bool isDynamic, float mass, float centerX, float centerY, float radius, QObject *parent) :
    NewtonSceneObject(QPointF(centerX, centerY), parent),
    m_isDynamic(isDynamic),
    m_Mass(mass)
{
    m_shape = Shape::Circle;
    shapeVal.circle.cX = centerX;
    shapeVal.circle.cY = centerY;
    shapeVal.circle.r = radius;

}

NewtonBody::NewtonBody(bool isDynamic, float mass, float centerX, float centerY, float width, float height, QObject *parent) :
    NewtonSceneObject(QPointF(centerX, centerY), parent),
    m_isDynamic(isDynamic),
    m_Mass(mass)
{
    m_shape = Shape::Rect;
    shapeVal.rect.cX = centerX;
    shapeVal.rect.cY = centerY;
    shapeVal.rect.width = width;
    shapeVal.rect.height = height;
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

NewtonBody::Shape NewtonBody::getShapeType(){
    return m_shape;
}

NewtonBody::NewtonShape NewtonBody::getShapeValue(){
    return shapeVal;
}

void NewtonBody::setShape(float centerX, float centerY, float radius){
    m_shape = Shape::Circle;
    if(radius < 0.0f){
        radius = 0.0f;
    }

    shapeVal.circle.cX = centerX;
    shapeVal.circle.cY = centerY;
    shapeVal.circle.r = radius;
}

void NewtonBody::setShape(float centerX, float centerY, float width, float height){
    m_shape = Shape::Rect;
    if(width < 0.0f){
        width = 0.0f;
    }
    if(height < 0.0f){
        height = 0.0f;
    }

    shapeVal.rect.cX = centerX;
    shapeVal.rect.cY = centerY;
    shapeVal.rect.width = width;
    shapeVal.rect.height = height;

}

QString NewtonBody::serialize() const {
    return QString("");
}

