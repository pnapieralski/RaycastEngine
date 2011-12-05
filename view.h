#ifndef RAYCASTWIDGET_H
#define RAYCASTWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QBasicTimer>
#include <QTimer>
#include <QPlainTextEdit>

#define DEFAULT_WINDOW_WIDTH 640
#define DEFAULT_WINDOW_HEIGHT 480
#define DEFAULT_SLICE_WIDTH 2           // Real numbers Ok
#define NUM_SLICES DEFAULT_WINDOW_WIDTH / DEFAULT_SLICE_WIDTH

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
                   size_t numSlices = NUM_SLICES, QWidget *parent = 0);

    //! Clean up slices array and everything else
    ~Scene();

    void setCamera(Camera *);

    /**
     * This is where the magic happens.
     * For each slice, trace a ray until it hits a wall (or anything)
     * Then, change the color/texture of the slice according to what was hit
     */
    void castRays();

    /**
     * This updates the scene. Eg; player movement, renders slices, etc..
     */
    void timerEvent(QTimerEvent *);

    /**
     * This will be called when the window is resized
     */
    void resizeEvent(QResizeEvent *);

    //! Temporary debugging purposes (manually connect to QPlainTextEdit on mainwindow Widget)
    void setLogger(QPlainTextEdit *);
    void log(const QString &);

protected:
    //! Input functions
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    bool handleKey(int key, bool pressed);

private:
    Camera *mpCamera;

    //! Vertical line, corresponding to response of 1 ray
    //! This structure stores pointer to each rectangular slice
    QGraphicsRectItem **mSlices;

    QBasicTimer mTicker;

    size_t mNumSlices;



    QPlainTextEdit *mpLog;
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
