#ifndef RUNGEKUTTASOLVER_H
#define RUNGEKUTTASOLVER_H

#include <math.h>
#include <stdio.h>

class RungeKuttaSolver
{
    public:
        RungeKuttaSolver(double *z0, int dim, double tau, void (*dgl)(double*, double, int, double*, double*), double *param, int num_param);
        ~RungeKuttaSolver();
        void steps(double T);    // calculates a step and returns the new state
        double *z;      // current state

    private:
        double tau;     // timestep
        double t;       // current time
        int dim;        // phase space dimension
        double r_desired;
        double *p;
        void (*dgl)(double*, double, int, double*, double*);  // DGL to solve
        double get_new_tau();
        void step();
        void step(double*, double);

        double **Hs;
        double *ztmp, *z1, *z2;
        double max_tau;
};

#endif // RUNGEKUTTASOLVER_H
