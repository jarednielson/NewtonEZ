#include "newtoninputbox.h"
#include "mainwindow.h"
#include <QDoubleSpinBox>

NewtonInputBox::NewtonInputBox(int index, QWidget *parent) :
    index(index)
{
}

NewtonInputBox::~NewtonInputBox(){

}

void NewtonInputBox::interceptChange(double d){
    emit NewtonInputBox::inputBoxChanged(index, d);
}
