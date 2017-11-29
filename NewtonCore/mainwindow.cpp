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
    labels.push_back(new QLabel());
    labels[0]->setText("here");
    layouts.push_back(new QHBoxLayout());
    layouts[0]->addWidget(labels[0]);
    ui->inputContainer->addLayout(layouts[0]);
}

MainWindow::~MainWindow()
{
    delete ui;
}
