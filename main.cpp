#include <QtGui/QApplication>
#include "Camera.h"
#include "RaycastWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Camera cam;
    RaycastWidget raycast(&cam);

    raycast.setWindowTitle("Raycasting demo");
    raycast.resize(320, 240);
    raycast.show();

    return a.exec();
}
