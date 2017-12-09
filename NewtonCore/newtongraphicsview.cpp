#include "newtongraphicsview.h"
#include <QGraphicsView>
#include <QPoint>
#include <QMouseEvent>

NewtonGraphicsView::NewtonGraphicsView(QWidget *parent) : QGraphicsView(parent)
{

}
NewtonGraphicsView::NewtonGraphicsView(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{

}
NewtonGraphicsView::~NewtonGraphicsView(){

}
void NewtonGraphicsView::mouseDoubleClickEvent(QMouseEvent *event){
    emit mouseDoubleClick(event->x(), event->y());
    emit mouseDoubleClick(QPoint(event->x(), event->y()));
    QGraphicsView::mouseDoubleClickEvent(event);
}
void NewtonGraphicsView::mouseMoveEvent(QMouseEvent *event){
    emit mouseMove(event->x(), event->y());
    emit mouseMove(QPoint(event->x(), event->y()));
    QGraphicsView::mouseMoveEvent(event);
}
void NewtonGraphicsView::mousePressEvent(QMouseEvent *event){
    emit mousePress(event->x(), event->y());
    emit mousePress(QPoint(event->x(), event->y()));
    QGraphicsView::mousePressEvent(event);
}
void NewtonGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    emit mouseRelease(event->x(), event->y());
    emit mouseRelease(QPoint(event->x(), event->y()));
    QGraphicsView::mouseReleaseEvent(event);
}
