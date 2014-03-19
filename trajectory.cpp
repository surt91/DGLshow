#include "trajectory.h"

Trajectory::Trajectory(QWidget *parent, dglType type) :
    QWidget(parent)
{
    srand(time(NULL));

    t = 0;
    resize(X, Y);

    timeConstant = 1;
    traceLength = 300*60;
    traceLengthLimit = 10*60;

    QString str;
    switch(type)
    {
        case Lorenz:
            str = QString("Lorenz");
            break;
        case Body3:
            str = QString("Body3");
            break;
        case Body4:
            str = QString("Body4");
            break;
        case Random10:
            str = QString("random Body 10");
            break;
        case Planets:
            str = QString("Planets");
            break;
        case DoublePendulum:
            str = QString("double Pendulum");
            break;
        default:
            str = QString("Lorenz");
    }
    setDGL(str);

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

QSize Trajectory::minimumSizeHint()
{
    return(QSize(X, Y));
}

void Trajectory::setTraceLength(int T)
{
    traceLengthLimit = T*60;
}

// sets the speed of the animation
// big timeConstant: more simulated time between frames >> fast forward
// small timeConstant: less simulated time between frames >> slowmotion
void Trajectory::setTimeConstant(int T)
{
    timeConstant = T/10.0;
}

#define RAND(max) (double) rand()/RAND_MAX * 2*max - max;

void Trajectory::setDGL(QString str)
{
    try
    {
        delete rk4;
        for(int i=0;i<N;i++)
            delete[] buffer[i];
        delete[] buffer;
    }
    catch (int e)
    {
    }

    if(str == QString("Lorenz"))
    {
        type = Lorenz;
        N = 1;
        double z0[] = {1,1,20};
        rk4 = new RungeKuttaSolver(z0, N*3, 0.005, lorenz, NULL, 0);
    }
    else if(str == QString("Body3"))
    {
        type = Body3;
        N=3;
        double z0[] = { 0,0,  0,-1,  0,2,
                       -0.5,-0.2,  1.3,0.3, -1,0.2};
        double m[] = {1, 2, 3};
        rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
    }
    else if(str == QString("Body4"))
    {
        type = Body4;
        N=4;
        double z0[] = { 0,0,  0,-1,  0,2, 3,7,
                       -0.5,-0.2,  1.3,0.3, -1,0.2, 0.1,-0.8};
        double m[] = {1, 2, 1.5, 1};
        rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
    }
    else if(str == QString("Planets"))
    {
        type = Planets;
        N=7;
        double z0[] = {0,0, 10,0,  0,6,  -15,0, 30,0, -35,35, 31,-0.3,
                       0,0, 0,15,  20,0, 0,-14, 0,10,  -4,-4, -0.5,13};
        double m[] = {3000, 1, 0.5, 1, 10, 0.7, 0.8};

        // change the momentum of the "sun", so that the total momentum is zero
        double p_x=0, p_y=0;
        for(int i=1;i<N;i++)
        {
            p_x += z0[2*i+N*2] * m[i];
            p_y += z0[2*i+1+N*2] * m[i];
        }
        printf("%f, %f\n", z0[N/2],z0[N/2+1]);
        z0[N*2] = - p_x/m[0];
        z0[N*2+1] = - p_y/m[0];
        printf("%f, %f\n", z0[N/2],z0[N/2+1]);

        rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
    }
    else if(str == QString("random Body 10"))
    {
        type = Random10;
        N = 10;
        double z0[N*4];
        double m[N];

        double r_max = 5;
        double v_max = 0.5;
        for(int i=0; i<N; i++)
        {
            z0[2*i] = RAND(r_max);
            z0[2*i+1] = RAND(r_max);
            z0[2*i+2*N] = RAND(v_max);
            z0[2*i+1+2*N] = RAND(v_max);
            m[i] = (double) rand()/RAND_MAX * 2;
        }
        rk4 = new RungeKuttaSolver(z0, N*4, 0.000005, gravitation, m, N);
    }
    else if(str == QString("double Pendulum"))
    {
        type = DoublePendulum;
        N = 2;
        double z0[] = { M_PI/4*3,     M_PI, // angle
                        0,     0};          // angular velocity
        pendulumL[0] = 10; pendulumL[1] = 8;
        double mAndL[] = {1, pendulumL[0], 0.5, pendulumL[1]};
        rk4 = new RungeKuttaSolver(z0, N*2, 0.000005, double_pendulum, mAndL, N*2);
    }

    buffer = new QPointF*[N];
    for(int i=0;i<N;i++)
        buffer[i] = new QPointF[traceLength];

    t = 0;

    update();
}

// paints the trajectory and positions
void Trajectory::paintEvent(QPaintEvent *)
{
    int numCol = 9;
    QColor col[] = {QColor("red"), QColor("blue"), QColor("green"),
                    QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                    QColor("darkGreen"), QColor("yellow"), QColor("darkBlue")};

    QPainter painter(this);
    qreal dx = X/scale, dy = Y/scale;

    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    painter.scale(scale, scale);

    painter.eraseRect(0, 0, dx, dy);

    qreal w = 1; qreal h = 1;

    if(type == DoublePendulum)
    {
        QPointF origin = make_periodic_and_translate(QPointF(0,0));
        QPointF first = make_periodic_and_translate(buffer[0][t%traceLength]);
        QPointF second = make_periodic_and_translate(buffer[1][t%traceLength]);
        QRectF rect(origin - QPointF(w, h)/2, QSize(w, h)/2);

        painter.setBrush(QBrush(QColor("Black")));
        painter.drawPie(rect, 0, 16*360);
        painter.setBrush(QBrush(Qt::NoBrush));

        painter.drawLine(origin, first);
        painter.drawLine(first, second);
    }

    for(int i=0;i<N;i++)
    {
        QPointF r = buffer[i][t%traceLength];
        QSize size(w, h);
        QPointF center = make_periodic_and_translate(r - QPointF(w, h)/2);
        QRectF rect(center, size);
        painter.fillRect(rect, col[i%numCol]);

        QPainterPath path;
        bool first = true;
        for(int j=1; j<traceLength; j++)
            if((t+j)%traceLength < t && traceLength - j < traceLengthLimit)
            {
                QPointF prev = make_periodic_and_translate(buffer[i][(t+j)%traceLength]);
                QPointF now = make_periodic_and_translate(buffer[i][(t+j+1)%traceLength]);

                if(abs(prev.x() - now.x()) > dx/2 || abs(prev.y() - now.y()) > dy/2 || first)
                {
                    first = false;
                    path.moveTo(now);
                }
                else
                    path.lineTo(now);
            }

        painter.setPen(QPen(col[i%numCol], 0.1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
        painter.drawPath(path);
    }
}

// transformation to periodic boundary conditions, also translates the origin to the center of the widget
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

// adds new entrys to the buffer, to draw the trajectories
void Trajectory::update_trajectory_buffer()
{
    if(type == DoublePendulum)
    {
        double x = pendulumL[0] * sin(rk4->z[0]);
        double y = pendulumL[0] * cos(rk4->z[0]);
        buffer[0][t%traceLength].setX(x);
        buffer[0][t%traceLength].setY(y);
        buffer[1][t%traceLength].setX(pendulumL[1] * sin(rk4->z[1])+x);
        buffer[1][t%traceLength].setY(pendulumL[1] * cos(rk4->z[1])+y);
    }
    else if(type == Lorenz)
    {
        buffer[0][t%traceLength].setX(rk4->z[0]);
        buffer[0][t%traceLength].setY(rk4->z[2]);
    }
    else
    for(int i=0;i<N;i++)
    {
        buffer[i][t%traceLength].setX(rk4->z[i*2]);
        buffer[i][t%traceLength].setY(rk4->z[i*2+1]);
    }
}

// calculates a time step, saves the result and updates the picture
void Trajectory::timestep()
{
    t++;
    rk4->steps(1.0/60.0*timeConstant);
    update_trajectory_buffer();
    update();
}
