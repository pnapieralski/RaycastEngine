#include <QPaintEvent>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QPainter>

#include "raycastwidget.h"

RaycastWidget::RaycastWidget(QWidget *parent) :
    QWidget(parent)
{
    watch.start();

    // Tick every 25 seconds on this
    ticker.start(25, this);
}

void RaycastWidget::putPixel(int x, int y, QRgb &color) {
    // Grab pointer to start of buffer
    const uchar *ptr = buffer.bits();
    QRgb *start = (QRgb*)(ptr);
    QRgb stride = buffer.bytesPerLine() / 4;
    //QRgb *finish = start + stride * bufh;

    // Grab pointer to (row, col) pixel
    QRgb *pixel1 = start + y * stride + x;
    *pixel1 = color;
}

void RaycastWidget::render() {
    // Check if buffer size has been changed, update it
    if( buffer.size() != bufferSize ) {
        buffer = QImage(bufferSize, QImage::Format_ARGB32);
    }

    int bufw = buffer.width();
    int bufh = buffer.height();

    // Ensure we have valid buffer dimensions
    if( bufw <= 0 || bufh <= 0 ) return;

    for( int y = 0; y < bufh; y++ ) {
        for( int x = 0; x < bufw; x++ ) {
            QRgb color = qRgb(rand()%255, 100, 0);
            putPixel(x, y, color);
        }
    }

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
