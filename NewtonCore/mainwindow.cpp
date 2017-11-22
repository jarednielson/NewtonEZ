#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tools = new QActionGroup(this);
    tools->addAction(ui->actionDynamicBodyTool);
    tools->addAction(ui->actionForceTool);
    tools->addAction(ui->actionMoveTool);
    tools->addAction(ui->actionStaticBodyTool);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tools;
}
