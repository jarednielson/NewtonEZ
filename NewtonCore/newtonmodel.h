#ifndef NEWTONMODEL_H
#define NEWTONMODEL_H

// This model needs to connect the view and model: listen to the view and repond to that by signal

#include "newtondatamodels.h"
#include <QObject>
#include <QGraphicsScene>
#include <QImage>



class NewtonView : public QObject{
    Q_OBJECT
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
