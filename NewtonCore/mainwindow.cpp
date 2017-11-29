#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>

MainWindow::MainWindow(QWidget *parent/*, Model *model*/) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)/*,
    model(model)*/
{
    ui->setupUi(this);
    ui->textEdit->setText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

    addInputPair("1");
    addInputPair("2");
    addInputPair("3");
    addInputPair("4");
    addInputPair("5");
    addInputPair("6");
    addInputPair("7");
    clearInputConAndVector();

}

MainWindow::~MainWindow()
{
    delete ui;
}

///
/// \brief MainWindow::addInputPair
/// Helper method that will create a new input pair ( QLabel and QDoubleSpinBox ) placing it
/// in its own new QHBoxLayout.
///
/// \param labelText
///
void MainWindow::addInputPair(QString labelText){
    labels.push_back(new QLabel());
    labels.back()->setText(labelText);
    inputs.push_back(new QDoubleSpinBox());
    layouts.push_back(new QHBoxLayout());
    layouts.back()->addWidget(labels.back());
    layouts.back()->addWidget(inputs.back());
    ui->inputContainer->addLayout(layouts.back());
}

///
/// \brief MainWindow::clearInputConAndVector
/// Clears the inputContainer and the vectors holding the input
///
void MainWindow::clearInputConAndVector(){
    QLayoutItem *child = ui->inputContainer->takeAt(0);
    while (child != 0) {
      delete child;
      child = ui->inputContainer->takeAt(0);
    }
    // needs work...
    labels.clear();
    inputs.clear();
    layouts.clear();
}
