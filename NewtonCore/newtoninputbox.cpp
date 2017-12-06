#include "newtoninputbox.h"
#include <QDoubleSpinBox>

NewtonInputBox::NewtonInputBox(int index, QWidget *parent = Q_NULLPTR) :
    index(index)
{

    connect(this, QDoubleSpinBox::valueChanged, this, NewtonInputBox::interceptChange);
}

void NewtonInputBox::interceptChange(double d){
    emit NewtonInputBox::inputBoxChanged(index, d);
}
