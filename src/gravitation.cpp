#include <math.h>

void gravitation(double *z, double , int dim, double *m, double *out)
{
    int num_particles = dim/4;
    double gamma = 1;

    // delta r = v
    for(int i=0; i<dim/2; i++)
        out[i] = z[i+dim/2];

    // delta v = a = F/m
    for(int i=0; i<num_particles; i++)
    {
        double dv_x = 0;
        double dv_y = 0;
        for(int j=0; j<num_particles; j++)
            if(j != i)
            {
                double x = z[2*i] - z[2*j];
                double y = z[2*i + 1] - z[2*j + 1];
                double tmp = sqrt(x*x + y*y) * (x*x + y*y); // distance ^ 3
                dv_x += m[j]/tmp*x;
                dv_y += m[j]/tmp*y;
            }
        out[dim/2 + i*2] = -gamma*dv_x;
        out[dim/2 + i*2+1] = -gamma*dv_y;
    }
}

