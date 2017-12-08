#ifndef NEWTONMODEL_H
#define NEWTONMODEL_H

#define TOL 0.001f

// This model needs to connect the view and model: listen to the view and repond to that by signal

#include "newtondatamodels.h"
#include "Box2D.h"
#include <QObject>
#include <QGraphicsScene>
#include <QImage>
#include <QScriptEngine>


class NewtonModel : public QObject{
    Q_OBJECT
private:
    // This is box2d simulation list, the list type maybe change to box2d type
    QVector<b2Body*> graphs;
    QVector<NewtonScene*> scenes;
    int currentSceneIndex;

    QGraphicsScene* graphicsScene;
    double evaluateFormulas(QJsonArray& formulas, QVector<float>& varVals);

public:
    NewtonModel(QObject *parent = 0);
    NewtonModel(QString filePath, QObject *parent = 0);
    ~NewtonModel();

public slots:
    void loadFile(QString filePath);
    void setScene(int sceneIndex);
    void nextScene();
    void prevScene();
    void startSimulation();
    void endSimulation();
    void getProblemDescriptions(QStringList& descriptions);
    void validateAnswer(float answer);
    void loadDefaultScene();

signals:
    // the button pressed will send this signal
    // The parameters passed in may need to change
    void simulationStart();
    void simulationEnd();

    void instructionTextChanged(QString newText);
    void inputWidgetsChanged(QStringList widgetLabels);
    void displayWidgetsChanged(QStringList widgetLabels, QList<float> widgetValues);

    void answerValidated(bool success);
    void lastSceneSelected();
    void firstSceneSelected();

private:
    void clearModel();
    float answer;
    void verifyScene(int sceneIndex);
};


#endif // NEWTONMODEL_H
