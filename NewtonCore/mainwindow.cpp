#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QStyleFactory>
#include <QDebug>

MainWindow::MainWindow(NewtonModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(model)
{
    ui->setupUi(this);

    // (nathan): Not a huge fan of how this is setting universal os styles, looking for a better way
//    ui->menuBar->setStyleSheet("background-color:\"#353535\";color:\"white\"");
//    ui->menuFile->setStyleSheet("background-color:\"#353535\";color:\"white\"");
//    this->setStyle(QStyleFactory::create("Fusion"));
//    QPalette darkPalette;
//    darkPalette.setColor(QPalette::Window,QColor(53,53,53));
//    darkPalette.setColor(QPalette::WindowText,Qt::white);
//    darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
//    darkPalette.setColor(QPalette::Base,QColor(42,42,42));
//    darkPalette.setColor(QPalette::AlternateBase,QColor(66,66,66));
//    darkPalette.setColor(QPalette::ToolTipBase,Qt::white);
//    darkPalette.setColor(QPalette::ToolTipText,Qt::white);
//    darkPalette.setColor(QPalette::Text,Qt::white);
//    darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
//    darkPalette.setColor(QPalette::Dark,QColor(35,35,35));
//    darkPalette.setColor(QPalette::Shadow,QColor(20,20,20));
//    darkPalette.setColor(QPalette::Button,QColor(53,53,53));
//    darkPalette.setColor(QPalette::ButtonText,Qt::white);
//    darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
//    darkPalette.setColor(QPalette::BrightText,Qt::red);
//    darkPalette.setColor(QPalette::Link,QColor(42,130,218));
//    darkPalette.setColor(QPalette::Highlight,QColor(42,130,218));
//    darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
//    darkPalette.setColor(QPalette::HighlightedText,Qt::white);
//    darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));
//    this->setPalette(darkPalette);

    ui->graphicsView->setScene(model->getGraphicsScene());
    replaceAndSetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n\nUt enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\nDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\nExcepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

    addInputPair("1");
    addInputPair("2", 2.0, false);
    addInputPair("3", 3.0, false);
    addInputPair("4", 4.0, false);
    addInputPair("5", 5.0, false);
    addInputPair("6", 6.0, false);
    addInputPair("7", 7.0, false);
    addInputPair("8", 8.0, false);
    addInputPair("9", 9.0, false);
    addInputPair("10", 10.0, true);
    // Comment out to see inputs, uncomment to clear all inputs
    //clearInputConAndVector();

    tools = new QActionGroup(this);
    tools->addAction(ui->actionDynamicBodyTool);
    tools->addAction(ui->actionForceTool);
    tools->addAction(ui->actionMoveTool);
    tools->addAction(ui->actionStaticBodyTool);
    tools->setExclusive(true);

    connect(ui->graphicsView, static_cast<void (NewtonGraphicsView::*) (int, int)>(&NewtonGraphicsView::mouseRelease), this, &MainWindow::onGraphicsViewReleased);
    // TODO: test
    connect(model, &NewtonModel::instructionTextChanged, this, &MainWindow::replaceAndSetText);
    connect(model, &NewtonModel::answerValidated, this, &MainWindow::updateAnswerLabel);
    connect(model, &NewtonModel::inputWidgetsChanged, this, &MainWindow::getInputsForProblem);

    updateTime(100);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tools;
}

void MainWindow::onGraphicsViewReleased(int x, int y){

}

///
/// \brief MainWindow::addInputPair
/// Helper method that will create a new input pair ( QLabel and QDoubleSpinBox ) placing it
/// in its own new QHBoxLayout.
///
/// \param labelText
///
void MainWindow::addInputPair(QString labelText){
    MainWindow::addInputPair(labelText, 0.0, true);
}

///
/// \brief MainWindow::addInputPair
/// Helper method that will create a new input pair ( QLabel and QDoubleSpinBox ) placing it
/// in its own new QHBoxLayout.
///
/// \param labelText
///
void MainWindow::addInputPair(QString labelText, double inputBoxValue, bool inputEnabled){
    // can change min and max with setMaximum and setMinimun
    labels.push_back(new QLabel());
    labels.back()->setText(labelText);
    inputs.push_back(new QDoubleSpinBox());
    inputs.back()->setValue(inputBoxValue);
    inputs.back()->setEnabled(inputEnabled);
    layouts.push_back(new QHBoxLayout());
    layouts.back()->addWidget(labels.back());
    layouts.back()->addWidget(inputs.back());
    ui->inputContainer->addLayout(layouts.back());
    ui->inputContainer->activate();
}

///
/// \brief MainWindow::clearInputConAndVector
/// Clears the inputContainer and the vectors holding the input
///
void MainWindow::clearInputConAndVector(){

    for(unsigned int i = 0; i < layouts.size(); i++){
        delete labels[i];
        delete inputs[i];
        delete layouts[i];
    }

    labels.clear();
    inputs.clear();
    layouts.clear();
}

void MainWindow::replaceAndSetText(QString text){
    ui->textEdit->setText(text);
}

void MainWindow::resetAnswerLabel(){
    ui->answerLabel->setText("");
}

void MainWindow::updateAnswerLabel(bool answer){
    if(answer){
        ui->answerLabel->setText("Correct");
    } else {
        ui->answerLabel->setText("Incorrect");
    }
}

void MainWindow::updateTime(int seconds){
    ui->simClock->setText(QString::number(seconds));
}

void MainWindow::getInputsForProblem(QStringList widgetLabels, QStringList values, QList<bool> enabled){
    clearInputConAndVector();
    for(int i = 0; i < widgetLabels.length(); i++){
        addInputPair(widgetLabels[i], values[i].toDouble(), enabled[i]);
    }
}
