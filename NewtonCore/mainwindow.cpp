#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QStyleFactory>
#include <QFileDialog>
#include <QDir>
#include <QAction>
#include <QVector>
#include <QPicture>

///
/// \brief MainWindow::MainWindow
/// Main Constructor. It sets up UI, prepares stuff, connects slots and signals.
///
/// \param model
/// \param parent
///
MainWindow::MainWindow(NewtonModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    model(model)
{
    ui->setupUi(this);
    ui->formulaWidget->hide();

    ui->graphicsView->setScene(model->getGraphicsScene());
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::white));
    //ui->graphicsView->setSceneRect(0.0f, 0.0f, 200, 200);
    ui->graphicsView->setTransform(QTransform::fromScale(1, - 1));
    isSimPlaying = false;

    updateTime(0);

    tools = new QActionGroup(this);
    tools->addAction(ui->actionFormulaSheet);
    tools->addAction(ui->actionPreviousScene);
    tools->addAction(ui->actionNextScene);
    tools->setExclusive(true);

    connect(ui->graphicsView, static_cast<void (NewtonGraphicsView::*) (int, int)>(&NewtonGraphicsView::mouseRelease), this, &MainWindow::onGraphicsViewReleased);
    // TODO: test
    connect(model, &NewtonModel::briefTextChanged, this, &MainWindow::setProblemText);
    connect(model, &NewtonModel::inputWidgetsChanged, this, &MainWindow::getInputsForProblem);
    connect(model, &NewtonModel::displayWidgetsChanged, this, &MainWindow::getDisplayForProblem);
    connect(ui->checkAnswerButton, &QPushButton::clicked, this, &MainWindow::prepareEnabledInputs);
    connect(ui->actionOpen_Problem, &QAction::triggered, this, &MainWindow::createOpenFileDialog);
    connect(this, &MainWindow::sendFilePath, model, &NewtonModel::loadFile);

    connect(ui->actionFormulaSheet, &QAction::triggered, this, &MainWindow::openFormulaSheet);
    // TODO: connect to model signals to actually change scene index
    connect(ui->actionPreviousScene, &QAction::triggered, this, &MainWindow::clearInputBoxes);
    connect(ui->actionNextScene, &QAction::triggered, this, &MainWindow::clearInputBoxes);
    connect(ui->actionNextScene, &QAction::triggered, model, &NewtonModel::nextScene);
    connect(ui->actionPreviousScene, &QAction::triggered, model, &NewtonModel::prevScene);
    connect(ui->actionLoad_Default_Problem, &QAction::triggered, model, &NewtonModel::loadDefaultScene);

    connect(this, &MainWindow::sendEnabledInputs, model, &NewtonModel::validateAnswer);
    connect(model, &NewtonModel::answerValidated, this, &MainWindow::cleanUpAfterSimulation);

    // hook up play / pause button to model to show simulation
    connect(ui->PlayStopButton, &QPushButton::clicked, this, &MainWindow::prepToPlay);
    connect(this, &MainWindow::sendPlaySimRequest, model, &NewtonModel::startSimulation);
    connect(this, &MainWindow::sendStopSimRequest, model, &NewtonModel::endSimulation);
    connect(model, &NewtonModel::simulationEnd, this, &MainWindow::simulationEnded);
}

///
/// \brief MainWindow::~MainWindow
/// Destructor
///
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
void MainWindow::addInputBox(QString labelText){
    // can change min and max with setMaximum and setMinimun
    enabledLabels.push_back(new QLabel());
    enabledLabels.back()->setText(labelText);
    enabledInputs.push_back(new QDoubleSpinBox());
    enabledInputs.back()->setValue(0.0);
    enabledInputs.back()->setEnabled(true);
    enabledInputs.back()->setDecimals(4);
    layouts.push_back(new QHBoxLayout());
    layouts.back()->addWidget(enabledLabels.back());
    layouts.back()->addWidget(enabledInputs.back());
    ui->inputContainer->addLayout(layouts.back());
    ui->inputContainer->activate();
}

void MainWindow::addDisplayBox(QString labelText, double value){
    disabledLabels.push_back(new QLabel());
    disabledLabels.back()->setText(labelText);
    disabledInputs.push_back(new QDoubleSpinBox());
    disabledInputs.back()->setValue(value);
    disabledInputs.back()->setEnabled(false);
    disabledInputs.back()->setDecimals(4);
    layouts.push_back(new QHBoxLayout());
    layouts.back()->addWidget(disabledLabels.back());
    layouts.back()->addWidget(disabledInputs.back());
    ui->inputContainer->addLayout(layouts.back());
    ui->inputContainer->activate();
}

///
/// \brief MainWindow::clearInputConAndVector
/// Clears the inputContainer and the vectors holding the input
///
void MainWindow::clearInputBoxes(){
    for(int i = 0; i < disabledLabels.size(); i++){
        delete disabledLabels[i];
        delete disabledInputs[i];
    }

    for(int i = 0; i < enabledLabels.size(); i++){
        delete enabledLabels[i];
        delete enabledInputs[i];
    }
    for(unsigned int i = 0; i < layouts.size(); i++){
        delete layouts[i];
    }

    enabledLabels.clear();
    enabledInputs.clear();
    disabledLabels.clear();
    disabledInputs.clear();
    layouts.clear();
}

/// SLOT
/// \brief MainWindow::replaceAndSetText
/// This a slot that connect to a signal in the model that will update the instructional
/// text.
///
/// \param text - new instructional text
///
void MainWindow::setProblemText(QString text){
    ui->textEdit->setText(text);
}

/// SLOT
/// \brief MainWindow::updateTime
/// This slot connects with a signal in the model which will pass in it the current seconds.
/// We then update the label simClock with the new seconds.
///
/// \param seconds - current seconds
///
void MainWindow::updateTime(int seconds){
    ui->simClock->setText(QString::number(seconds));
}

// We need to refactor this to two functions... because there are going to be two signals
// one signal for enabled and the other non enabled although we could use the same method
// to hook up for both....
///
/// \brief MainWindow::getInputsForProblem
/// \param widgetLabels
/// \param values
/// \param enabled
///
void MainWindow::getInputsForProblem(QStringList widgetLabels){
    for(int i = 0; i < widgetLabels.length(); i++){
        addInputBox(widgetLabels[i]);
    }
}

///
/// \brief MainWindow::getDisplayForProblem
/// \param widgetLabels
/// \param values
///
void MainWindow::getDisplayForProblem(QStringList widgetLabels, QList<float> values ){
    clearInputBoxes();
    for(int i = 0; i < widgetLabels.length(); i++){
        addDisplayBox(widgetLabels[i], values[i]);
    }
}

/// SLOT
/// \brief MainWindow::prepareEnabledInputs
/// This slot is connected to the startEndButton click signal. It will prepare the data
/// and then send it to the model via a signal.
///
void MainWindow::prepareEnabledInputs(){
    QVector<float> enabledFloats;
    for(unsigned int i = 0; i < enabledInputs.size(); i++){
        enabledFloats.push_back(enabledInputs[i]->value());
    }
    // disable button...
    ui->checkAnswerButton->setEnabled(false);

    // emit signal that will take this vector to the model
    emit sendEnabledInputs(enabledFloats);
}

/// SLOT
/// \brief MainWindow::cleanUpAfterSimulation
/// This slot will connect with a signal in the model. We can clean up
/// after a simulation here. Currently we are just re-enabling the startEndButton.
///
void MainWindow::cleanUpAfterSimulation(QVector<bool> answers){
    ui->checkAnswerButton->setEnabled(true);
    for(int i = 0; i < answers.length() || i < enabledInputs.size(); i++){
        if(answers[i]){
            enabledInputs[i]->setStyleSheet(QString("background-color : #b3ffb3"));
        }else {
            enabledInputs[i]->setStyleSheet(QString("background-color : #ff8080"));
        }
    }
}

/// SLOT
/// \brief MainWindow::createOpenFileDialog
/// This slot hooks up to open menu button. When run it will call the getOpenFileName
/// method which will allow the user to store a file path. It then emits a sendFilPath
/// signal passing in the file path sending it to the model.
///
void MainWindow::createOpenFileDialog(){
    QString filename =  QFileDialog::getOpenFileName( this,
                                                      "Open Problem",
                                                      QDir::homePath(),
                                                      "All files (*.*)");

    emit sendFilePath(filename);
}

void MainWindow::openFormulaSheet(){
    //TODO: get formula data from model
    //TODO: add formulas to widget in a nice way
    ui->formulaWidget->show();
}

///
/// \brief MainWindow::prepToPlay
///
void MainWindow::prepToPlay(){
    if(isSimPlaying){
        ui->PlayStopButton->setText("Play");
        isSimPlaying = false;
        animationEnableDisable();
        emit sendStopSimRequest();
    } else {
        ui->PlayStopButton->setText("Stop");
        isSimPlaying = true;
        animationEnableDisable();
        emit sendPlaySimRequest();
    }
}

void MainWindow::simulationEnded(){
    ui->PlayStopButton->setText("Play");
    isSimPlaying = false;
    animationEnableDisable();
}

void MainWindow::on_actionLoad_Default_Problem_triggered()
{

}

void MainWindow::animationEnableDisable(){
    ui->actionNextScene->setEnabled(!isSimPlaying);
    ui->actionPreviousScene->setEnabled(!isSimPlaying);
    if(isSimPlaying){
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    } else {
        ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
}
