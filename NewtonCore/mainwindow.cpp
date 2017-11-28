#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Model *model) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(model)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
