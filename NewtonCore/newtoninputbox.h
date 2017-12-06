#ifndef NEWTONINPUTBOX_H
#define NEWTONINPUTBOX_H

#include <QObject>
#include <QWidget>
#include <QDoubleSpinBox>

class NewtonInputBox : public QDoubleSpinBox
{
    Q_OBJECT
public:
    NewtonInputBox(int index, QWidget *parent = Q_NULLPTR);
    ~NewtonGraphicsView();
public slots:
    void interceptChange(double d);

private:
    int index;

signals:
    void inputBoxChanged(int index, double d);

protected:

};

#endif // NEWTONINPUTBOX_H
