#include "rungekuttasolver.h"

RungeKuttaSolver::RungeKuttaSolver(double *z0, int dim_in, double tau_in, void (*dgl_in)(double*, double, int, double*, double*),  double *param, int num_param)
{
    dim = dim_in;

    r_desired = 0.00000001;

    max_tau = 1.0/120.0;

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
    z1 = new double[dim];
    z2 = new double[dim];
}

RungeKuttaSolver::~RungeKuttaSolver()
{
    for(int i=0;i<4;i++)
        delete[] Hs[i];
    delete[] Hs;
    delete[] ztmp;
    delete[] z;
    delete[] p;
    delete[] z1;
    delete[] z2;
}

void RungeKuttaSolver::steps(double T)
{
    for(double j=0; j<T; j+=tau)
        step();
}

void RungeKuttaSolver::step()
{
    tau = get_new_tau();
    step(z, tau);
}

void RungeKuttaSolver::step(double *z, double tau)
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


double RungeKuttaSolver::get_new_tau()
{
    int i;
    double S = 0.9, tau_neu, r, tmp1, tmp2;

    for(i=0;i<dim;i++)
    {
        z1[i]=z[i];
        z2[i]=z[i];
    }

    step(z1, tau);
    step(z2, tau/2);
    step(z2, tau/2);

    tmp1 = 0; tmp2 = 0;
    for(i=0;i<dim;i++)
    {
        //Norm^2 of z2-z1
        tmp1 += (z2[i]-z1[i])*(z2[i]-z1[i]);
        //Norm^2 of (z1+z2)/2
        tmp2 += (z1[i]+z2[i])*(z1[i]+z2[i])/4;
    }
    r = sqrt(tmp1)/sqrt(tmp2);

    tau_neu = tau * pow((r_desired/r),1/5.);
    tau_neu = S*tau_neu;
    if(tau_neu < tau/4)
        tau_neu = tau/4;
    if(tau_neu > 4*tau)
        tau_neu = 4*tau;

    if(tau_neu > max_tau)
        tau_neu = max_tau;

    return tau_neu;
}
