#ifndef NEWTONCOREMODEL_H
#define NEWTONCOREMODEL_H

#include <QObject>
#include <QGraphicsScene>

class NewtonCoreModel : public QObject
{
    Q_OBJECT
public:
    explicit NewtonCoreModel(QObject *parent = nullptr);

protected slots:
    void onGraphicsViewClicked(int x, int y);

private:
    QGraphicsScene *scene;
};

#endif // NEWTONCOREMODEL_H
