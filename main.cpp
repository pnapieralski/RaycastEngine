#include <QtGui/QApplication>
#include "Camera.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    Camera cam;
    Scene scene(&cam);
    View raycast(&scene);

    raycast.setWindowTitle("Raycasting demo");
    raycast.resize(640, 480);
    raycast.show();

    return a.exec();
}
