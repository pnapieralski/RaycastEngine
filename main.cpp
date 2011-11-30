#include <QtGui/QApplication>
#include "mainwindow.h"
#include "raycastwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RaycastWidget raycast;

    raycast.setWindowTitle("Raycasting demo");
    raycast.resize(640, 480);
    raycast.show();

    return a.exec();
}
