#include <QPaintEvent>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QGraphicsItem>
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <cstdlib>
#include <cmath>
#include "Camera.h"

#include "view.h"

#define PI 3.1415926535897323

#define GRID_HEIGHT 64
#define GRID_WIDTH 64

// Size of each cube (in map)
#define CUBE_SIZE 64

#define WORLD_SIZE 6
int worldMap[WORLD_SIZE][WORLD_SIZE] = {
  {1,1,1,1,1,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,1,1,1,1,1}
};

Scene::Scene(Camera *pCam, unsigned height, unsigned numSlices, QWidget *parent)
        : QGraphicsScene(parent)
{
    mNumSlices = numSlices;
    setCamera(pCam);

    mSlices = new QGraphicsRectItem*[mNumSlices];
    for( size_t i = 0; i < mNumSlices; i++ ) {
        mSlices[i] = this->addRect(i*DEFAULT_SLICE_WIDTH, 0,
                                   DEFAULT_SLICE_WIDTH, DEFAULT_WINDOW_HEIGHT,
                                   QPen(Qt::NoPen), QBrush(Qt::red));
    }

    // update every XX ms, 33 ms delay = 30 frames/sec
    mTicker.start(33, this);
}

Scene::~Scene() {
    if( mSlices ) delete mSlices;
}

void Scene::setLogger(QPlainTextEdit *p) {
    mpLog = p;
}

void Scene::log(const QString &t) {
    if(!mpLog) return;
    mpLog->setPlainText(t);
}

void Scene::setCamera(Camera *pCam) {
    mpCamera = pCam;
}

void Scene::castRays() {
    mpCamera->x = 96;
    mpCamera->y = 224;
    //mpCamera->angle = 0;

    // Get sin/cos of y rotation
    qreal angBetweenRays = mpCamera->fov / DEFAULT_WINDOW_WIDTH;

    // Look XX degrees left and right of camera
    qreal ang = mpCamera->angle - mpCamera->fov / 2;
    for( unsigned ray = 0; ray < mNumSlices; ray++, ang += angBetweenRays ) {
        if( ang > 360 ) ang -= 360;
        if( ang < 0 ) ang += 360;
        qreal angRadians = ang * PI / 180;
        qreal Px = mpCamera->x;
        qreal Py = mpCamera->y;

        qreal dx, dy;

        // Grid value in map
        size_t gridX, gridY;
        size_t Xa, Ya, Cx, Cy;
        qreal distance1, distance2;

        // Check whether ray from (maPx, maPy) at angle (ang) intersects something non-zero

        // Find first horizontal intersection point (see http://www.permadi.com/tutorial/raycast/rayc7.html)
        size_t Ax, Ay;
        if( ang >= 0 && ang < 180 ) {
            // Ray is facing up
            Ay = floor((float)Py/(float)GRID_HEIGHT) * GRID_HEIGHT - 1;
            dy = -GRID_HEIGHT;
        } else {
            // Ray is facing down
            Ay = floor((float)Py/(float)GRID_HEIGHT) * GRID_HEIGHT + GRID_HEIGHT;
            dy = GRID_HEIGHT;
        }

        Ax = Px + ((Py-Ay)/tanf(angRadians));
        dx = GRID_HEIGHT/tanf(angRadians);

        // Find the intersection (if Ax, Ay is not it)
        gridX = Ax / GRID_HEIGHT;
        gridY = Ay / GRID_HEIGHT;

        while( gridX < WORLD_SIZE && gridY < WORLD_SIZE && gridX >= 0 && gridY >= 0 && worldMap[gridY][gridX] == 0 ) {
            Ax += dx;
            Ay += dy;
            gridX = Ax / GRID_HEIGHT;
            gridY = Ay / GRID_HEIGHT;
        }

        distance1 = abs(Px - Ax)/cos(angRadians);

        // Find vertical intersections
        size_t Bx, By;
        if( ang <= 90 || ang >= 270 ) {
            // Ray is facing right
            Bx = floor((float)Px / (float)GRID_WIDTH) * GRID_WIDTH + 64;
            dx = GRID_WIDTH;
        } else {
            // Ray is facing left
            Bx = floor((float)Px / (float)GRID_WIDTH) * GRID_WIDTH - 1;
            dx = -GRID_WIDTH;
        }

        By = Py + ((Px - Bx)*tanf(angRadians));
        dy = GRID_WIDTH*tanf(angRadians);

        gridX = Bx / GRID_WIDTH;
        gridY = By / GRID_WIDTH;
        while( gridX < WORLD_SIZE && gridY < WORLD_SIZE && gridX >= 0 && gridY >= 0 && worldMap[gridY][gridX] == 0 ) {
            Bx += dx;
            By += dy;
            gridX = Bx / GRID_WIDTH;
            gridY = By / GRID_WIDTH;
        }

        distance2 = abs(Px-Bx)/cos(angRadians);

        // Is horizontal or vertical intersection closer? (store in distance1)
        if( distance2 < distance1 ) {
            distance1 = distance2;
        }

        // Handle distortion (http://www.permadi.com/tutorial/raycast/rayc8.html)
        qreal beta = mpCamera->angle - ang;
        distance1 *= cosf(beta * PI / 180);

        // Find slice height (project onto project plane)
        qreal sliceHeight = (CUBE_SIZE / distance1) * mpCamera->depth;
        if( sliceHeight > 200 ) sliceHeight = 200;
        if( sliceHeight < 0 ) sliceHeight = 0;
        qreal yVal = sliceHeight - sliceHeight/2.0;

        mSlices[ray]->setRect(ray*DEFAULT_SLICE_WIDTH, yVal,
                            DEFAULT_SLICE_WIDTH, sliceHeight);

        log(QString("%1 %2").arg(ang).arg(gridY) );
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if( focusItem() ) {
        QGraphicsScene::mouseMoveEvent(event);
        return;
    }

    if( event->buttons() & Qt::LeftButton ) {
        QPointF delta(event->scenePos() - event->lastScenePos());
        mpCamera->angle += delta.x();
        if( mpCamera->angle > 360 ) mpCamera->angle -= 360;
        if( mpCamera->angle < 0 ) mpCamera->angle += 360;
    }
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if (handleKey(event->key(), true)) {
        event->accept();
        return;
    }

    QGraphicsScene::keyPressEvent(event);
}

void Scene::keyReleaseEvent(QKeyEvent *event)
{
    if (handleKey(event->key(), false)) {
        event->accept();
        return;
    }

    QGraphicsScene::keyReleaseEvent(event);
}

bool Scene::handleKey(int key, bool pressed) {
    if( focusItem() ) return false;

    switch(key) {
        case Qt::Key_Left:
        case Qt::Key_Q:
            mpCamera->angle += 1;
            break;
    }

    return false;
}

void Scene::timerEvent(QTimerEvent *) {
    castRays();

    //for( size_t i = 0; i < mNumSlices; i++ ) {
    //    mSlices[i]->setBrush(QBrush(QColor(rand()%255, 0, 0, 150)));
    //}
}

View::View(Scene *scene, QWidget *parent) :
    QGraphicsView(parent)
{
    setScene(scene);
    //scene->setSceneRect(0, 0, );
}

void View::resizeEvent(QResizeEvent *event) {
    QSize dim = event->size();
    QTransform s;
    s.scale(dim.width()/640.0, dim.height()/480.0);
    setTransform(s);
}
