#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Model *model);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Model *model;
};

#endif // MAINWINDOW_H
