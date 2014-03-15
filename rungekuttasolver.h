#ifndef RUNGEKUTTASOLVER_H
#define RUNGEKUTTASOLVER_H

#include <stdio.h>

class RungeKuttaSolver
{
    public:
        RungeKuttaSolver(double *z0, double tau, void (*dgl)(double*, double, int, double*), int dim);
        ~RungeKuttaSolver();
        void step(int N);    // calculates a step and returns the new state
        double *z;      // current state
    private:
        double tau;     // timestep
        double t;       // current time
        int dim;        // phase space dimension
        void (*dgl)(double*, double, int, double*);  // DGL to solve

        double **Hs;
        double *ztmp;
};

#endif // RUNGEKUTTASOLVER_H
