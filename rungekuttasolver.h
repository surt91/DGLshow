#ifndef RUNGEKUTTASOLVER_H
#define RUNGEKUTTASOLVER_H

#include <stdio.h>

class RungeKuttaSolver
{
    public:
        RungeKuttaSolver(double *z0, int dim, double tau, void (*dgl)(double*, double, int, double*, double*), double *param, int num_param);
        ~RungeKuttaSolver();
        void step(double T);    // calculates a step and returns the new state
        double *z;      // current state

    private:
        double tau;     // timestep
        double t;       // current time
        int dim;        // phase space dimension
        double *p;
        void (*dgl)(double*, double, int, double*, double*);  // DGL to solve

        double **Hs;
        double *ztmp;
};

#endif // RUNGEKUTTASOLVER_H
