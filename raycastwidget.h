#ifndef RAYCASTWIDGET_H
#define RAYCASTWIDGET_H

#include <QWidget>
#include <QBasicTimer>
#include <QTimer>

class RaycastWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RaycastWidget(QWidget *parent = 0);

    void render();
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *);
    void resizeEvent(QResizeEvent*);

signals:

public slots:

private:
    void putPixel(int x, int y, QRgb &color);

private:
    QImage buffer;
    QSize bufferSize;

    // For checking FPS
    QTimer watch;

    // For setting update speed
    QBasicTimer ticker;
};

#endif // RUICASTWIDGET_H
