#include "camera.h"

Camera::Camera(QObject *parent) :
    QObject(parent), depth(255), fov(60), x(0), y(0), angle(0)
{
}
