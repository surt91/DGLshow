#include "trajectory.h"

Trajectory::Trajectory(dglType type, QWidget *parent) :
    QWidget(parent)
{
    t = 0;
    resize(X, Y);

    switch(type)
    {
        case Lorenz:
        {
            N = 1;
            traceLength = 1000;
            double z0[] = {1,1,20};
            rk4 = new RungeKuttaSolver(z0, N*3, 0.005, lorenz, NULL, 0);
            break;
        }
        case Body3:
        {
            N=3;
            traceLength = 10000;
            double z0[] = { 0,0,  0,-1,  0,2,
                           -0.5,-0.2,  1.3,0.3, -1,0.2};
            double m[] = {1, 2, 3};
            rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
            break;
        }
        case Body4:
        {
            N=4;
            traceLength = 10000;
            double z0[] = { 0,0,  0,-1,  0,2, 3,7,
                           -0.5,-0.2,  1.3,0.3, -1,0.2, 0.4,-0.8};
            double m[] = {1, 2, 3, 1};
            rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
            break;
        }
        case DoublePendulum:
        {
            N = 2;
            traceLength = 1000;
            double z0[] = { 0,0,  1,-6, // r
                           -1,0,  0,0}; // v
            double m[] = {1, 1};
            rk4 = new RungeKuttaSolver(z0, N*4, 0.005, double_pendulum, m, N);
            break;
        }
    }

    buffer = new QPointF*[N];
    for(int i=0;i<N;i++)
        buffer[i] = new QPointF[traceLength];

    update();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timestep()));
    timer->start(16);
}

Trajectory::~Trajectory()
{
    delete rk4;
    for(int i=0;i<N;i++)
        delete[] buffer[i];
    delete[] buffer;
}

void Trajectory::paintEvent(QPaintEvent *)
{
    QColor col[] = {QColor("red"), QColor("blue"), QColor("green"),
                    QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                    QColor("darkGreen"), QColor("yellow")};

    QPainter painter(this);
    qreal dx = X/scale, dy = Y/scale;

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    painter.scale(scale, scale);

    painter.eraseRect(0, 0, dx, dy);

    qreal w = 1; qreal h = 1;

    for(int i=0;i<N;i++)
    {
        QPointF r = buffer[i][t%traceLength];
        QSize size(w, h);
        QPointF center = make_periodic_and_translate(r - QPointF(w, h)/2);
        QRectF rect(center, size);
        painter.fillRect(rect, col[i]);

        QPainterPath path;
        path.moveTo(make_periodic_and_translate(buffer[i][(t+1)%traceLength]));
        for(int j=1; j<traceLength; j++)
            if((t+j)%traceLength < t)
            {
                QPointF prev = make_periodic_and_translate(buffer[i][(t+j)%traceLength]);
                QPointF now = make_periodic_and_translate(buffer[i][(t+j+1)%traceLength]);

                if(abs(prev.x() - now.x()) > dx/2 || abs(prev.y() - now.y()) > dy/2)
                    path.moveTo(now);
                else
                    path.lineTo(now);
            }

        painter.setPen(QPen(col[i], 0.1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.drawPath(path);
    }
}

QPointF Trajectory::make_periodic_and_translate(QPointF r)
{
    qreal dx = X/scale;
    qreal dy = Y/scale;

    qreal x = r.x() + dx/2;
    x = x - dx * floor(x/dx);
    qreal y = r.y() + dy/2;
    y = y - dy * floor(y/dy);

    return QPointF(x, y);
}

void Trajectory::update_trajectory_buffer()
{
    for(int i=0;i<N;i++)
    {
        buffer[i][t%traceLength].setX(rk4->z[i*2]);
        buffer[i][t%traceLength].setY(rk4->z[i*2+1]);
    }
}

void Trajectory::timestep()
{
    t++;
    rk4->step(1.0/60.0);
    update_trajectory_buffer();
    update();
}
