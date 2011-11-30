#include <QPaintEvent>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QPainter>
#include <cmath>
#include "Camera.h"

#include "RaycastWidget.h"

RaycastWidget::RaycastWidget(Camera *cam, Scene *scene, QWidget *parent) :
    QWidget(parent), pCam(cam)
{
    watch.start();

    // Render every xx ms
    ticker.start(22, this);
}

void RaycastWidget::putPixel(int x, int y, QRgb &color) {
    if( x > buffer.width() || y > buffer.height() || x < 0 || y < 0 ) return;

    // Grab pointer to start of buffer
    const uchar *ptr = buffer.bits();
    QRgb *start = (QRgb*)(ptr);
    QRgb stride = buffer.bytesPerLine() / 4;
    //QRgb *finish = start + stride * bufh;

    // Grab pointer to (row, col) pixel
    QRgb *pixel1 = start + y * stride + x;
    *pixel1 = color;
}

void RaycastWidget::clearScreen(QRgb &color) {
    buffer.fill(0);
}

void RaycastWidget::render() {
    // Check if buffer size has been changed, update it
    if( buffer.size() != bufferSize ) {
        buffer = QImage(bufferSize, QImage::Format_ARGB32);
        pCam->width = buffer.width();
        pCam->height = buffer.height();
        pCam->offsetX = pCam->width / 2;
        pCam->offsetY = pCam->height / 2;
    }

    int bufw = buffer.width();
    int bufh = buffer.height();

    // Ensure we have valid buffer dimensions
    if( bufw <= 0 || bufh <= 0 ) return;

    float xs[] = {100,  -100,  -100,   100,  100, -100, -100,  100};
    float ys[] = {100,   100,  -100,  -100,  100,  100, -100, -100};
    float zs[] = {1000,  1000,  1000,  1000, 500,  500,  500,  500};

    // Clear screen
    QRgb color = qRgb(50, 100, 0);
    //putPixel(x, y, color);
    clearScreen(color);

    for( int y = 0; y < bufh-8; y++ ) {
        for( int x = 0; x < bufw-8; x++ ) {
            // Draw wall vertices projected onto 'window'
            color = qRgb(255, 100, 0);
            for( int i = 0; i < 8; i++ ) {
                for( int offx = 0; offx < 4; offx++ ) {
                    for( int offy = 0; offy < 4; offy++ ) {
                        float dz = zs[i] - pCam->pos.z;
                        float dx = xs[i] - pCam->pos.x;
                        float dy = ys[i] - pCam->pos.y;

                        float d1x = cosf(pCam->rot.y)*dx + sinf(pCam->rot.y)*dz;
                        float d1y = dy;
                        float d1z = cosf(pCam->rot.y)*dz - sinf(pCam->rot.y)*dx;

                        if( d1z > 0 ) {

                            float scale = pCam->depth / d1z;
                            float posX = (d1x)*scale + pCam->offsetX;
                            float posY = (d1y)*scale + pCam->offsetY;

                            putPixel( posX+offx, posY+offy, color );
                        }
                    }
                }
            }
        }
    }
    //pCam->pos.z-=5;
    pCam->rot.y+=0.1;

    // send paint event
    update(QRect(QPoint(0,0), bufferSize));


}

void RaycastWidget::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Source);

    // Draw buffer to widget
    p.drawImage(event->rect(), buffer, event->rect());

    p.end();
}

/**
 * This event is called X times per second
 * Update/render everything
 */
void RaycastWidget::timerEvent(QTimerEvent *event) {
    render();
}

void RaycastWidget::resizeEvent(QResizeEvent *event) {
    bufferSize = size();
    update();
}
