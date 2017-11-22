#ifndef NEWTONGRAPHICSVIEW_H
#define NEWTONGRAPHICSVIEW_H

#include <QObject>
#include <QGraphicsView>
#include <QWidget>

class NewtonGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    NewtonGraphicsView(QWidget *parent = Q_NULLPTR);
    NewtonGraphicsView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);
    ~NewtonGraphicsView();

signals:
    void mouseDoubleClick(int x, int y);
    void mouseDoubleClick(QPoint pos);
    void mouseMove(int x, int y);
    void mouseMove(QPoint pos);
    void mousePress(int x, int y);
    void mousePress(QPoint pos);
    void mouseRelease(int x, int y);
    void mouseRelease(QPoint pos);


protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // NEWTONGRAPHICSVIEW_H
