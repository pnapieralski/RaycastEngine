#ifndef RAYCASTWIDGET_H
#define RAYCASTWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QBasicTimer>
#include <QTimer>

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480
#define DEFAULT_SLICE_WIDTH 2           // Real numbers Ok

//! Forward declarations
class Camera;

/**
 * Scene
 * This class connects connects to our View and handles all drawing
 */
class Scene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit Scene(Camera *, unsigned height = DEFAULT_WINDOW_HEIGHT,
                   size_t numSlices = DEFAULT_WINDOW_WIDTH / DEFAULT_SLICE_WIDTH, QWidget *parent = 0);
    ~Scene();

    void setCamera(Camera *);

    /**
     * This updates the scene. Eg; player movement, renders slices, etc..
     */
    void timerEvent(QTimerEvent *);

    /**
     * This will be called when the window is resized
     */
    void resizeEvent(QResizeEvent *);

private:
    Camera *mpCamera;

    //! Vertical line, corresponding to response of 1 ray
    //! This structure stores pointer to each rectangular slice
    QGraphicsRectItem **mSlices;

    QBasicTimer mTicker;

    size_t mNumSlices;
};

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View(Scene *scene, QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);
};

#endif // RUICASTWIDGET_H
