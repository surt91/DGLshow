#include "rungekuttasolver.h"

RungeKuttaSolver::RungeKuttaSolver(double *z0, int dim_in, double tau_in, void (*dgl_in)(double*, double, int, double*, double*),  double *param, int num_param)
{
    dim = dim_in;

    z = new double[dim];
    p = new double[num_param];
    for(int i=0; i<num_param; i++)
        p[i] = param[i];

    for(int i=0; i<dim; i++)
        z[i] = z0[i];

    tau = tau_in;
    dgl = dgl_in;

    Hs = new double*[4];
    for(int i=0;i<4;i++)
        Hs[i] = new double[dim];
    ztmp = new double[dim];
}

RungeKuttaSolver::~RungeKuttaSolver()
{
    for(int i=0;i<4;i++)
        delete[] Hs[i];
    delete[] Hs;
    delete[] ztmp;
    delete[] z;
    delete[] p;
}

void RungeKuttaSolver::step(int N)
{
    for(int j=0; j<N; j++)
    {
        int i;
        dgl(z, t, dim, p, Hs[0]);
        for(i=0;i<dim;i++)
            ztmp[i] = z[i]+tau/2*Hs[0][i];
        dgl(ztmp, t+tau/2, dim, p, Hs[1]);
        for(i=0;i<dim;i++)
            ztmp[i] = z[i]+tau/2*Hs[1][i];
        dgl(ztmp, t+tau/2, dim, p, Hs[2]);
        for(i=0;i<dim;i++)
            ztmp[i] = z[i]+tau  *Hs[2][i];
        dgl(ztmp, t+tau, dim, p, Hs[3]);

        for(i=0;i<dim;i++)
            z[i] = z[i]+ tau/6*(Hs[0][i]+2*Hs[1][i]+2*Hs[2][i]+Hs[3][i]);
    }
}
