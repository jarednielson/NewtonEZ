#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0/*, Model *model*/);
    ~MainWindow();

protected slots:
    void onGraphicsViewReleased(int x, int y);
private:
    Ui::MainWindow *ui;

    std::vector<QHBoxLayout*> layouts;
    std::vector<QLabel*> labels;
    std::vector<QDoubleSpinBox*> inputs;

    void addInputPair(QString labelText);
    void clearInputConAndVector();
    QActionGroup *tools;
};

#endif // MAINWINDOW_H
