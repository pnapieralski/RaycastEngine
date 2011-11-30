#include "camera.h"

Camera::Camera(QObject *parent) :
    QObject(parent), depth(350), pos(), rot()
{
}
