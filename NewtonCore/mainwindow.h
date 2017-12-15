#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QTimer>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QActionGroup>
#include "newtonmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(NewtonModel *model, QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setProblemText(QString text);
    void updateTime(double seconds);
    void getInputsForProblem(QStringList widgetLabels);
    void getDisplayForProblem(QStringList widgetLabels, QList<float> values);
    void prepareEnabledInputs();
    void cleanUpAfterSimulation(QVector<bool> answers);
    void createOpenFileDialog();
    void prepToPlay();
    void simulationEnded();

signals:
    void sendEnabledInputs(QVector<float> enabledInputs);
    void sendFilePath(QString path);
    void sendPlaySimRequest();
    void sendStopSimRequest();

protected slots:
    void onGraphicsViewReleased(int x, int y);

private slots:
    void on_actionLoad_Default_Problem_triggered();

    void on_zoomSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    std::vector<QHBoxLayout*> layouts;
    std::vector<QLabel*> enabledLabels;
    std::vector<QDoubleSpinBox*> enabledInputs;
    std::vector<QLabel*> disabledLabels;
    std::vector<QDoubleSpinBox*> disabledInputs;

    void addInputBox(QString labelText);
    void addDisplayBox(QString labelText, double inputBoxValue);
    void clearInputBoxes();
    void openFormulaSheet();
    void animationEnableDisable();
    QActionGroup *tools;
    NewtonModel *model;
    bool isSimPlaying;
    bool isSimLoaded;
};

#endif // MAINWINDOW_H
