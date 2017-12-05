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
    void replaceAndSetText(QString text);
    void updateAnswerLabel(bool answer);
    void updateTime(int seconds);

protected slots:
    void onGraphicsViewReleased(int x, int y);
private:
    Ui::MainWindow *ui;

    std::vector<QHBoxLayout*> layouts;
    std::vector<QLabel*> labels;
    std::vector<QDoubleSpinBox*> inputs;

    void addInputPair(QString labelText);
    void addInputPair(QString labelText, double inputBoxValue, bool inputEnabled);
    void clearInputConAndVector();
    void resetAnswerLabel();
    QActionGroup *tools;
    NewtonModel *model;
};

#endif // MAINWINDOW_H
