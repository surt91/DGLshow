#include "trajectorywidget.h"

TrajectoryWidget::TrajectoryWidget(QWidget *parent) :
    QGLWidget(parent), Trajectory()
{
    t = 0;
    strokeWidth = 0.15;
    resize(X, Y);
    this->setFormat(QGLFormat(QGL::SampleBuffers));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timestep()));
    timer->start(16);
}

QSize TrajectoryWidget::minimumSizeHint()
{
    return(QSize(X, Y));
}

// paints the trajectory and positions
void TrajectoryWidget::paintEvent(QPaintEvent *)
{
    int numCol = 9;
    QColor col[] = {QColor("red"), QColor("blue"), QColor("green"),
                    QColor("darkRed"), QColor("darkCyan"), QColor("darkMagenta"),
                    QColor("darkGreen"), QColor("yellow"), QColor("darkBlue")};

    QPainter painter(this);
    qreal dx = X/scale, dy = Y/scale;


    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

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

        painter.setPen(QPen(QColor("Black"), strokeWidth, Qt::SolidLine));
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

        painter.setPen(QPen(col[i%numCol], strokeWidth, Qt::SolidLine));
        painter.drawPath(path);
    }
}

// calculates a time step, saves the result and updates the picture
void TrajectoryWidget::timestep()
{
    Trajectory::timestep();
    update();
}

void TrajectoryWidget::setDGL(QString str)
{
    Trajectory::setDGL(str);
    update();
}

void TrajectoryWidget::setTraceLength(int T)
{
    Trajectory::setTraceLength(T);
}

// sets the speed of the animation
// big timeConstant: more simulated time between frames >> fast forward
// small timeConstant: less simulated time between frames >> slowmotion
void TrajectoryWidget::setTimeConstant(int T)
{
    Trajectory::setTimeConstant(T);
}

// transformation to periodic boundary conditions, also translates the origin to the center of the widget
QPointF TrajectoryWidget::make_periodic_and_translate(QPointF r)
{
    qreal dx = X/scale;
    qreal dy = Y/scale;

    qreal x = r.x() + dx/2;
    x = x - dx * floor(x/dx);
    qreal y = r.y() + dy/2;
    y = y - dy * floor(y/dy);

    return QPointF(x, y);
}
