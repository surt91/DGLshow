#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "rungekuttasolver.h"
#include "lorenzattraktor.h"
#include "gravitation.h"

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QRectF>
#include <QColor>
#include <QPainterPath>
#include <QPen>

class Trajectory : public QWidget
{
    Q_OBJECT
public:
    explicit Trajectory(QWidget *parent = 0);
    ~Trajectory();
    RungeKuttaSolver *rk4;
    static const int X = 800;
    static const int Y = 600;
    static const double scale = 10;

private:
    int t;
    static const int traceLength = 1000;
    int N;
    qreal **buffer;


protected:
     void paintEvent(QPaintEvent *);

signals:

public slots:
     void timestep();

};

#endif // TRAJECTORY_H
