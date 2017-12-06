#ifndef NEWTONINPUTBOX_H
#define NEWTONINPUTBOX_H

#include <QObject>
#include <QWidget>
#include <QDoubleSpinBox>

class NewtonInputBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    NewtonInputBox(QWidget *parent = Q_NULLPTR);
    ~NewtonGraphicsView();

signals:

protected:

};

#endif // NEWTONINPUTBOX_H
