#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "rungekuttasolver.h"
#include "dgls.h"

#include <math.h>
#include <stdlib.h>

#include <QWidget>
//#include <QtOpenGL/QGLWidget>
#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QSize>
#include <QPointF>
#include <QColor>
#include <QPainterPath>
#include <QPen>
#include <QBrush>

enum dglType {Lorenz, Body3, Body4, Random10, Planets, DoublePendulum};

class Trajectory : public QWidget
{
    Q_OBJECT
public:
    explicit Trajectory(QWidget *parent = 0, dglType type = Lorenz);
    ~Trajectory();
    RungeKuttaSolver *rk4;
    static const int X = 1280;
    static const int Y = 1024;
    static const double scale = 10;
    QSize minimumSizeHint();
    dglType type;

private:
    int t;
    double timeConstant;
    int traceLength;
    int traceLengthLimit;
    int N;
    QPointF **buffer;
    void update_trajectory_buffer();
    QPointF make_periodic_and_translate(QPointF);
    double pendulumL[2];

protected:
     void paintEvent(QPaintEvent *);

signals:

public slots:
     void timestep();
     void setTraceLength(int=10);
     void setTimeConstant(int=1);
     void setDGL(QString);

};

#endif // TRAJECTORY_H
