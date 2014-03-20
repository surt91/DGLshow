#include "trajectory.h"

Trajectory::Trajectory()
{
    t = 0;
    timeConstant = 1;
    traceLength = 300*60;
    traceLengthLimit = 10*60;
    setDGL(QString("Lorenz"));
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

// sets the speed of the animation
// big timeConstant: more simulated time between frames >> fast forward
// small timeConstant: less simulated time between frames >> slowmotion
void Trajectory::setTimeConstant(int T)
{
    timeConstant = T/10.0;
}

void Trajectory::setDGL(QString str)
{
    if(t>0)
    {
        delete rk4;
        for(int i=0;i<N;i++)
            delete[] buffer[i];
        delete[] buffer;
    }

    if(str == QString("Lorenz"))
        setLorenz();
    else if(str == QString("Body3"))
        setThreeBody();
    else if(str == QString("Body4"))
        setFourBody();
    else if(str == QString("Planets"))
        setPlanets();
    else if(str == QString("random Body 10"))
        setTenBody();
    else if(str == QString("double Pendulum"))
        setDoublePendulum();

    buffer = new QPointF*[N];
    for(int i=0;i<N;i++)
        buffer[i] = new QPointF[traceLength];

    t = 0;
}

void Trajectory::setLorenz()
{
    type = Lorenz;
    N = 1;
    double z0[] = {1,1,20};
    rk4 = new RungeKuttaSolver(z0, N*3, 0.005, lorenz, NULL, 0);
}

void Trajectory::setPlanets()
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
    z0[N*2] = - p_x/m[0];
    z0[N*2+1] = - p_y/m[0];

    rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
}

void Trajectory::setDoublePendulum()
{
    type = DoublePendulum;
    N = 2;
    double z0[] = { M_PI/4*3,     M_PI, // angle
                    0,     0};          // angular velocity
    pendulumL[0] = 10; pendulumL[1] = 8;
    double mAndL[] = {1, pendulumL[0], 0.5, pendulumL[1]};
    rk4 = new RungeKuttaSolver(z0, N*2, 0.000005, double_pendulum, mAndL, N*2);
}

void Trajectory::setThreeBody()
{
    type = Body3;
    N=3;
    double z0[] = { 0,0,  0,-1,  0,2,
                   -0.5,-0.2,  1.3,0.3, -1,0.2};
    double m[] = {1, 2, 3};
    rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
}

void Trajectory::setFourBody()
{
    type = Body4;
    N=4;
    double z0[] = { 0,0,  0,-1,  0,2, 3,7,
                   -0.5,-0.2,  1.3,0.3, -1,0.2, 0.1,-0.8};
    double m[] = {1, 2, 1.5, 1};
    rk4 = new RungeKuttaSolver(z0, N*4, 0.00005, gravitation, m, N);
}

#define RAND(max) (double) rand()/RAND_MAX * 2*max - max;
void Trajectory::setTenBody()
{
    srand(time(NULL));
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
}

// calculates a time step, saves the result and updates the picture
void Trajectory::benchmark()
{
    double T = 60*60; // 1h
    setDGL(QString("double Pendulum"));

    int begin = clock();
    rk4->steps(T);
    int end = clock();
    printf("1h Doppelpendel: %.2fs\n", (double)(end-begin)/CLOCKS_PER_SEC);


    setDGL(QString("Lorenz"));

    begin = clock();
    rk4->steps(T);
    end = clock();
    printf("1h Lorenz: %.2fs\n", (double)(end-begin)/CLOCKS_PER_SEC);


    setDGL(QString("Planets"));

    begin = clock();
    rk4->steps(T);
    end = clock();
    printf("1h Planets: %.2fs\n", (double)(end-begin)/CLOCKS_PER_SEC);
}
