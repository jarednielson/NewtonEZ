#include "mainwindow.h"
#include <QApplication>
#include "Box2D.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    b2Vec2 gravity(0.0f, -10.0f);

    // Construct a world object, which will hold and simulate the rigid bodies.
    b2World world(gravity);

    return a.exec();
}
