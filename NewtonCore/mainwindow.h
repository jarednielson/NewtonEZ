#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
    void onGraphicsViewReleased(int x, int y);
private:
    Ui::MainWindow *ui;
    QActionGroup *tools;
};

#endif // MAINWINDOW_H
