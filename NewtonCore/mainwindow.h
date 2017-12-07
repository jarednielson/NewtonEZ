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
    void updateTime(int seconds);
    void getInputsForProblem(QStringList widgetLabels, QStringList values, QList<bool> enabled);
    void prepareEnabledInputs();
    void cleanUpAfterSimulation();
    void createOpenFileDialog();

signals:
    void sendEnabledInputs(std::vector<QString> enabledInputs);
    void sendFilePath(QString path);

protected slots:
    void onGraphicsViewReleased(int x, int y);

private:
    Ui::MainWindow *ui;

    std::vector<QHBoxLayout*> layouts;
    std::vector<QLabel*> labels;
    std::vector<QDoubleSpinBox*> inputs;

    void addInputBox(QString labelText);
    void addInputBox(QString labelText, double inputBoxValue, bool inputEnabled);
    void clearInputBoxes();
    QActionGroup *tools;
    NewtonModel *model;
};

#endif // MAINWINDOW_H
