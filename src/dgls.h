#ifndef DGLS_H
#define DGLS_H

void gravitation(double *z, double t, int dim, double *param, double *out);
void lorenz(double *r, double t, int dim, double *param, double *out);
void double_pendulum(double *r, double t, int dim, double *param, double *out);

#endif // DGLS_H
