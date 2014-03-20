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

class Trajectory
{
public:
    explicit Trajectory();
    ~Trajectory();
    RungeKuttaSolver *rk4;
    dglType type;
    void benchmark();

protected:
    int t;
    double timeConstant;
    int traceLength;
    int traceLengthLimit;
    int N;
    QPointF **buffer;
    void update_trajectory_buffer();

private:
    double pendulumL[2];

    void setLorenz();
    void setPlanets();
    void setDoublePendulum();
    void setThreeBody();
    void setFourBody();
    void setTenBody();

signals:

public slots:
     void timestep();
     void setTraceLength(int=10);
     void setTimeConstant(int=1);
     void setDGL(QString);

};

#endif // TRAJECTORY_H
