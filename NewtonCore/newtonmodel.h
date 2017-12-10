#ifndef NEWTONMODEL_H
#define NEWTONMODEL_H

#define TOL 0.001f
#define TIME_STEP 0.01666666666f
#define TIME_OUT 10000
#define VEL_ITER 8
#define POS_ITER 3
#define PIX_PER_METER 50

// This model needs to connect the view and model: listen to the view and repond to that by signal

#include "newtondatamodels.h"
#include "Box2D.h"
#include <QObject>
#include <QGraphicsScene>
#include <QImage>
#include <QScriptEngine>
#include <QGraphicsItem>
#include <QTimer>
#include <QTime>


class NewtonModel : public QObject{
    Q_OBJECT
    // This is box2d simulation list, the list type maybe change to box2d type
    QVector<b2Body*> graphs;
    b2World* world;
    QVector<NewtonScene*> scenes;
    QVector<QGraphicsItem*> scBodies;
    QTimer* simTimer;
    QTimer* simSingleShot;
    QTime simRuntime;
    QVector<float> answers;

    int currentSceneIndex;
    bool simRunning = false;

    QGraphicsScene* graphicsScene;

public:
    NewtonModel(QObject *parent = 0);
    NewtonModel(QString filePath, QObject *parent = 0);
    ~NewtonModel();
    QGraphicsScene* getGraphicsScene();

public slots:
    void loadFile(QString filePath);
    void setScene(int sceneIndex);
    void nextScene();
    void prevScene();
    void startSimulation();
    void endSimulation();
    void getProblemDescriptions(QStringList& descriptions);
    void validateAnswer(QVector<float> answers);
    void loadDefaultScene();
    void updateBodies();

signals:
    // the button pressed will send this signal
    // The parameters passed in may need to change
    void simulationStart();
    void simulationEnd();

    void updateClock(double seconds);

    void briefTextChanged(QString newText);
    void inputWidgetsChanged(QStringList widgetLabels);
    void displayWidgetsChanged(QStringList widgetLabels, QList<float> widgetValues);

    void answerValidated(QVector<bool> success);
    void lastSceneSelected();
    void firstSceneSelected();

private:
    void clearModel();
    void clearScene();
    void verifyScene(int sceneIndex);
    float convertToPixel(float meter);
    QPointF convertToPixel(QPointF meter);
};


#endif // NEWTONMODEL_H
