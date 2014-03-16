#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "rungekuttasolver.h"
#include "dgls.h"

#include <math.h>

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QSize>
#include <QPointF>
#include <QColor>
#include <QPainterPath>
#include <QPen>
#include <QBrush>

enum dglType {Lorenz, Body3, Body4, DoublePendulum};

class Trajectory : public QWidget
{
    Q_OBJECT
public:
    explicit Trajectory(dglType type, QWidget *parent = 0);
    ~Trajectory();
    RungeKuttaSolver *rk4;
    static const int X = 800;
    static const int Y = 600;
    static const double scale = 10;

private:
    int t;
    int traceLength;
    int N;
    QPointF **buffer;
    void update_trajectory_buffer();
    QPointF make_periodic_and_translate(QPointF);

protected:
     void paintEvent(QPaintEvent *);

signals:

public slots:
     void timestep();

};

#endif // TRAJECTORY_H
