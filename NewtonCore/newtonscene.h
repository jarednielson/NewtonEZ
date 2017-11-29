#ifndef NEWTONSCENE_H
#define NEWTONSCENE_H

#include <QObject>
#include <QGraphicsScene>

///
/// \brief The NewtonObject class
/// Base class for all objects allowed in the scene
/// This extends QObject so we can get properties
class NewtonObject : public QObject{
    Q_OBJECT
    Q_PROPERTY(int init_xPos READ init_xPos WRITE setInit_xPos)
    Q_PROPERTY(int init_yPos READ init_yPos WRITE setInit_yPos)

public:
    virtual QString serialize() = 0;
};

///
/// \brief The NewtonBody class
/// Base class for all objects that are simulated in the scene
class NewtonBody : public NewtonObject {
    Q_OBJECT
    Q_PROPERTY(type name READ name WRITE setName)
};

///
/// \brief The NewtonWidget class
/// Essentially a wrapper object for widgets that can be added to the
/// scene. Text, fields etc.
class NewtonWidget : public NewtonObject {

};



class NewtonScene : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(float pixPerMeter READ pixPerMeter WRITE setPixPerMeter NOTIFY pixPerMeterChanged)

public:
    explicit NewtonScene(QObject *parent = nullptr);

private:
    QGraphicsScene *scene;
};

#endif // NEWTONSCENE_H
