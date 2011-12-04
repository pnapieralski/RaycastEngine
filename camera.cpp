#include "camera.h"

Camera::Camera(QObject *parent) :
    QObject(parent), depth(350), fov(60), fovInRad(1.04719755f), pos(), rot()
{
}
