#include "trajectory.h"

Trajectory::Trajectory(QWidget *parent, dglType, type) :
    QWidget(parent)
{
    t = 0;
    resize(X, Y);

    switch(type)
    {
        case Lorenz:
            N = 1;
            traceLength = 1000;
            double z0[] = {1,1,20};
            rk4 = new RungeKuttaSolver(z0, N*3, 0.005, rk_lorenz_func, NULL, 0);
            break;
        case Body3:
            N=3;
            traceLength = 10000;
            double z0[] = { 0,0,  0,-1,  0,2,
                           -0.5,-0.2,  1.3,0.3, -1,0.2};
            double m[] = {1, 2, 3};
            rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
            break;
        case Body4:
            N=4;
            traceLength = 10000;
            double z0[] = { 0,0,  0,-1,  0,2, 3,7,
                           -0.5,-0.2,  1.3,0.3, -1,0.2, 0.4,-0.8};
            double m[] = {1, 2, 3, 1};
            rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
            break;
        case DoublePendulum:
            N = 2;
            traceLength = 1000;
            double z0[] = { 0,0,  1,-6, // r
                           -1,0,  0,0}; // v
            double m[] = {1, 1};
            rk4 = new RungeKuttaSolver(z0, N*4, 0.005, double_pendulum, m, N);
            break;
    }

    buffer = new qreal*[2*N];
    for(int i=0;i<2*N;i++)
        buffer[i] = new qreal[traceLength];

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timestep()));
    timer->start(16);
}

Trajectory::~Trajectory()
{
    delete rk4;
    for(int i=0;i<2*N;i++)
        delete[] buffer[i];
    delete[] buffer;
}

void Trajectory::paintEvent(QPaintEvent *)
{
    QColor col[] = {QColor("red"), QColor("blue"), QColor("green"),
                    QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                    QColor("darkGreen"), QColor("yellow")};

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    painter.eraseRect(0, 0, X, Y);

    qreal w = 1*scale; qreal h = 1*scale;

    for(int i=0;i<N;i++)
    {
        qreal x = rk4->z[i*2];
        qreal y = rk4->z[i*2+1];

        x = x*scale+X/2;
        y = y*scale+Y/8;

        qreal periodic_center_x = x-w/2 - X * floor((x-w/2)/X);
        qreal periodic_center_y = y-h/2 - Y * floor((y-h/2)/Y);
        QRectF rect(periodic_center_x, periodic_center_y, w, h);
        painter.fillRect(rect, col[i]);;

        // Spur
        buffer[2*i][t%traceLength] = x - X * floor(x/X);
        buffer[2*i+1][t%traceLength] = y - Y * floor(y/Y);

        qreal x_point = buffer[2*i][t%traceLength];
        qreal y_point = buffer[2*i+1][t%traceLength];

        QPainterPath path;
        path.moveTo(x_point, y_point);
        for(int j=traceLength; j>0; j--)
            if((t+j)%traceLength < t)
            {
                qreal x_point_tmp = buffer[2*i][(t+j)%traceLength] - X * floor(buffer[2*i][(t+j)%traceLength]/X);
                qreal y_point_tmp = buffer[2*i+1][(t+j)%traceLength] - Y * floor(buffer[2*i+1][(t+j)%traceLength]/Y);

                if(abs(x_point - x_point_tmp) > X/2 || abs(y_point - y_point_tmp) > Y/2)
                    path.moveTo(x_point_tmp, y_point_tmp);
                else
                    path.lineTo(x_point_tmp, y_point_tmp);

                x_point = x_point_tmp;
                y_point = y_point_tmp;
            }

        painter.setPen(QPen(col[i], 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter.drawPath(path);
    }
}

void Trajectory::timestep()
{
    t++;
    rk4->step(1000);
    update();
}
