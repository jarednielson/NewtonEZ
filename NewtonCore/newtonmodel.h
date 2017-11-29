#ifndef NEWTONMODEL_H
#define NEWTONMODEL_H

// This model needs to connect the view and model: listen to the view and repond to that by signal

#include <QObject>
#include <QGraphicsScene>
#include <QImage>

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


class NewtonView : public QObject{
    Q_OBJECT
    Q_PROPERTY(int init_xPos READ init_xPos WRITE setInit_xPos)
    Q_PROPERTY(int init_yPos READ init_yPos WRITE setInit_yPos)


private:
    // This is box2d simulation list, the list type maybe change to box2d type
    QList<QImage*> graphs;


public:
    virtual QString serialize() = 0;


public slots:
    // update the window which displays animation
    void updateSimulation(QImage* toUpdate);

    void endSimulation(QImage* end);


signals:
    // the button pressed will send this signal
    // The parameters passed in may need to change
    void simulationStarts(QImage* blank);
    void simulationPauses(QImage* pause);
    void simulationEnds(QImage* blank);
};


#endif // NEWTONMODEL_H
