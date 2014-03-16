#include "trajectory.h"

Trajectory::Trajectory(QWidget *parent, dglType type) :
    QWidget(parent)
{
    srand(time(NULL));

    t = 0;
    resize(X, Y);

    traceLength = 300*60;
    traceLengthLimit = 10*60;

    QString *str;
    switch(type)
    {
        case Lorenz:
            str = new QString("Lorenz");
            break;
        case Body3:
            str = new QString("Body3");
            break;
        case Body4:
            str = new QString("Body4");
            break;
        case DoublePendulum:
            str = new QString("double Pendulum");
            break;
        default:
            str = new QString("Lorenz");
    }
    setDGL(*str);

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

void Trajectory::setTraceLength(int T)
{
    traceLengthLimit = T*60;
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
        N = 1;
        double z0[] = {1,1,20};
        rk4 = new RungeKuttaSolver(z0, N*3, 0.005, lorenz, NULL, 0);
    }
    else if(str == QString("Body3"))
    {
        N=3;
        double z0[] = { 0,0,  0,-1,  0,2,
                       -0.5,-0.2,  1.3,0.3, -1,0.2};
        double m[] = {1, 2, 3};
        rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
    }
    else if(str == QString("Body4"))
    {
        N=4;
        double z0[] = { 0,0,  0,-1,  0,2, 3,7,
                       -0.5,-0.2,  1.3,0.3, -1,0.2, 0.1,-0.8};
        double m[] = {1, 2, 1.5, 1};
        rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
    }
    else if(str == QString("random Body 10"))
    {
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
//            printf("%f\n", z0[2*i]);
        }
        rk4 = new RungeKuttaSolver(z0, N*4, 0.000005, gravitation, m, N);
    }
    else if(str == QString("double Pendulum"))
    {
        N = 2;
        double z0[] = { 0,0,  1,-6, // r
                       -1,0,  0,0}; // v
        double m[] = {1, 1};
        rk4 = new RungeKuttaSolver(z0, N*4, 0.005, double_pendulum, m, N);
    }

    buffer = new QPointF*[N];
    for(int i=0;i<N;i++)
        buffer[i] = new QPointF[traceLength];

    t = 0;

    update();
}

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
    rk4->steps(1.0/60.0);
    update_trajectory_buffer();
    update();
}
