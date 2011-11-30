#ifndef RAYCASTWIDGET_H
#define RAYCASTWIDGET_H

#include <QWidget>
#include <QBasicTimer>
#include <QTimer>

//! XXX TODO Create Scene class
class Scene {};

//! Forward declarations
class Camera;

class RaycastWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RaycastWidget(Camera *cam, Scene *scene = 0, QWidget *parent = 0);

    void render();
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);
    void resizeEvent(QResizeEvent*);

signals:

public slots:

private:
    void putPixel(int x, int y, QRgb &color);
    void clearScreen(QRgb &color);

private:
    QImage buffer;
    QSize bufferSize;

    // For checking FPS
    QTimer watch;

    // For setting update speed
    QBasicTimer ticker;

    // Pointer to camera
    Camera *pCam;
};

#endif // RUICASTWIDGET_H
