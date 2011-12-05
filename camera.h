#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include "Vector3.h"

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = 0);
    qreal x, y, angle, fov;
    int depth;

    // received from widget
    int width;
    int height;

    // width / 2
    int offsetX;

    // height / 2
    int offsetY;

signals:
    // TODO

public slots:
    // TODO
};

#endif // CAMERA_H
