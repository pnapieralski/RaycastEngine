#include <QPaintEvent>
#include <QTimerEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QGraphicsItem>
#include <cmath>
#include "Camera.h"

#include "view.h"

#define PI 3.1415926535897323f

#define WORLD_SIZE 5
int worldMap[WORLD_SIZE][WORLD_SIZE] = {
  {1,1,1,1,1},
  {1,0,0,0,1},
  {1,0,0,0,1},
  {1,0,0,0,1},
  {1,1,1,1,1},
};

Scene::Scene(Camera *pCam, unsigned height, unsigned numSlices, QWidget *parent) : QGraphicsScene(parent) {
    mNumSlices = numSlices;
    setCamera(pCam);

    mSlices = new QGraphicsRectItem*[mNumSlices];
    for( size_t i = 0; i < mNumSlices; i++ ) {
        mSlices[i] = this->addRect(i*DEFAULT_SLICE_WIDTH, 0, DEFAULT_SLICE_WIDTH, DEFAULT_WINDOW_HEIGHT, QPen(Qt::NoPen));
    }

    // update every XX ms, 33 ms delay = 30 frames/sec
    mTicker.start(33, this);
}

Scene::~Scene() {
    if( mSlices ) delete mSlices;
}

void Scene::setCamera(Camera *pCam) {
    mpCamera = pCam;
}

void Scene::timerEvent(QTimerEvent *) {
    for( size_t i = 0; i < mNumSlices; i++ ) {
        //mSlices[i]->setPen(QPen(QColor(rand()%255, 0, 0, 15)));
        mSlices[i]->setBrush(QBrush(QColor(rand()%255, 0, 0, 150)));
    }
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
