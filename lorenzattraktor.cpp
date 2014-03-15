#include "lorenzattraktor.h"

void rk_lorenz_func(double *r, double t, int dim, double *out)
{
    // Lorenz Modell
    double sigma, r_param, b;
    double x, y, z;
    sigma   = 10;
    r_param = 28;
    b       = 8/3;

    x=r[0]; y=r[1]; z=r[2];

    out[0] = sigma*(y-x);
    out[1] = r_param*x - y - x*z;
    out[2] = x*y - b*z;
}

//void run_lorenz()
//{
//    double z[] = {1,1,20};

//    RungeKuttaSolver *rk4 = new RungeKuttaSolver(z, 0.001, rk_lorenz_func, 3);

//    for(int i=0; i<10; i++)
//    {
//        rk4->step();
//        printf("%f %f %f\n", rk4->z[0], rk4->z[1], rk4->z[2]);
//    }
//}
