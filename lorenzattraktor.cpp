void lorenz(double *r, double t, int dim, double *param, double *out)
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
