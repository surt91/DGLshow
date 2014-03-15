#include "trajectory.h"

Trajectory::Trajectory(QWidget *parent) :
    QWidget(parent)
{
    t = 0;
    resize(X, Y);

//    N = 1;
//    double z0[] = {1,1,20};
//    rk4 = new RungeKuttaSolver(z0, 0.005, rk_lorenz_func, N*3);

    N=3;
    double z0[] = { 0,0,  0,-1,  0,2,
                   -0.5,-0.2,  1.3,0.3, -1,0.2};
    rk4 = new RungeKuttaSolver(z0, 0.00005, gravitation, N*4);

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
}

void Trajectory::paintEvent(QPaintEvent *)
{
//    printf("paint\n");

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
        QPainterPath path;
        path.moveTo(buffer[2*i][t%traceLength], buffer[2*i+1][t%traceLength]);
        for(int j=traceLength; j>0; j--)
            if((t+j)%traceLength < t)
                path.lineTo(buffer[2*i][(t+j)%traceLength] - X * floor(buffer[2*i][(t+j)%traceLength]/X),
                        buffer[2*i+1][(t+j)%traceLength] - Y * floor(buffer[2*i+1][(t+j)%traceLength]/Y));

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
