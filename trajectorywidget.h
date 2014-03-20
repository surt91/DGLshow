#ifndef TRAJECTORYWIDGET_H
#define TRAJECTORYWIDGET_H

#include <QWidget>
#include <trajectory.h>

class TrajectoryWidget : public QWidget, Trajectory
{
    Q_OBJECT
public:
    explicit TrajectoryWidget(QWidget *parent = 0);
    static const int X = 1280;
    static const int Y = 1024;
    static const double scale = 10;
    QSize minimumSizeHint();

protected:
    void paintEvent(QPaintEvent *);

private:
    QPointF make_periodic_and_translate(QPointF);

signals:

public slots:
    void timestep();
    void setDGL(QString);
    void setTraceLength(int=10);
    void setTimeConstant(int=1);
};

#endif // TRAJECTORYWIDGET_H
