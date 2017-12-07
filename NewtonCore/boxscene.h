#ifndef BOX2DSCENE_H
#define BOX2DSCENE_H

#define pixelToMeter 30
#include <QObject>
#include "Box2D.h"

class BoxScene : public QObject{
    Q_OBJECT
private:
    b2World* world;
    b2Body* walls;
    float32 wallLineOffset;

    void RunScene();
    void buildWorld();
    void buildWalls();
    void buildRect();
    void buildCirc();
    void buildDynamicRect();
    void buildDynamicCirc();

};

#endif // BOX2DSCENE_H
