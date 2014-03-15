#include "gravitation.h"

void gravitation(double *z, double t, int dim, double *out)
{
    int num_particles = dim/4;
    double gamma = 1;
    double *m = new double[num_particles];
    for(int i=0; i<num_particles; i++)
        m[i] = 1;
    m[1] = 2;
    m[2] = 3;

    double *distance_x = new double[num_particles * num_particles];
    double *distance_y = new double[num_particles * num_particles];

    // delta r = v
    for(int i=0; i<dim/2; i++)
        out[i] = z[i+dim/2];

    // Abstände
    for(int i=0; i<num_particles; i++)
        for(int j=0; j<num_particles; j++)
        {
            distance_x[i*num_particles + j] = z[2*i] - z[2*j];
            distance_y[i*num_particles + j] = z[2*i + 1] - z[2*j + 1];
        }

    // Kräfte
    for(int i=0; i<num_particles; i++)
    {
        double dv_x = 0;
        double dv_y = 0;
        for(int j=0; j<num_particles; j++)
            if(j != i)
            {
                double x = distance_x[i*num_particles + j];
                double y = distance_y[i*num_particles + j];
                double tmp = pow(sqrt(x*x + y*y), 3);
                dv_x += m[j]/tmp*x;
                dv_y += m[j]/tmp*y;
            }
        out[dim/2 + i*2] = -gamma*dv_x;
        out[dim/2 + i*2+1] = -gamma*dv_y;
    }
//    printf("%f, %f", out[0], out[1]);
}

//void run_grav()
//{
//    double z0[] = { 0,0,  0,1,  0,2,
//                   -1,0,  1,0, -1,0};
//    RungeKuttaSolver *rk4 = new RungeKuttaSolver(z0, 0.05, gravitation, 12);

//    for(int i=0; i<100; i++)
//    {
//        rk4->step();
//        printf("%f %f %f\n", rk4->z[0], rk4->z[1], rk4->z[2]);
//    }
//}
