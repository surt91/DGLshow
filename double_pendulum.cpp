#include <math.h>

void double_pendulum(double *z, double , int , double *m, double *out)
{
    double g = 10;

    double m1 = m[0];
    double m2 = m[2];
    double l1 = m[1];
    double l2 = m[3];
    double M = m1 + m2;

    double theta1 = z[0];
    double theta2 = z[1];
    double dotTheta1 = z[2];
    double dotTheta2 = z[3];

    // Differentialgleichung:
    // http://de.wikipedia.org/wiki/Doppelpendel
    // reduziert auf 1. Ordnung
    double dTheta1 = dotTheta1;
    double dTheta2 = dotTheta2;


    // Trigonometrische Funktionen cachen
    double C = cos(theta1-theta2);
    double S = sin(theta1-theta2);


    // DGL
    double dDotTheta1 = (m2*C*(l1*S*(dotTheta1*dotTheta1) - g*sin(theta2)) + m2*l2*S*(dotTheta2*dotTheta2) + M*g*sin(theta1)) / (m2*l1*(C*C) - M*l1);
    double dDotTheta2 = (m2*l2*C*S*(dotTheta2*dotTheta2)  + M*l1*S*(dotTheta1*dotTheta1) + M*g*C*sin(theta1) - M*g*sin(theta2)) / (M*l2 - m2*l2*(C*C));

    out[0] = dTheta1;
    out[1] = dTheta2;
    out[2] = dDotTheta1;
    out[3] = dDotTheta2;
}
