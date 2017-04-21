#include "function.h"
#include <math.h>

namespace Math {
namespace Functions {

double min(const QVector<double> &vec) {
    double min = 0;
    if (!vec.size()) {
        return min;
    }

    min = vec[0];
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] < min) min = vec[i];
    }

    return min;
}

double max(const QVector<double> &vec) {
    double max = 0;
    if (!vec.size()) {
        return max;
    }

    max = vec[0];
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > max) max = vec[i];
    }

    return max;
}

double linear(double x) {
    return x;
}

double normalGaussianDensity(double x, double m, double v) {
    return 1 / sqrt(v * 2 * M_PI) * exp(-1 * pow(x - m, 2) / (2 * v));
//    return 1 / sqrt(/*v **/ 2 * M_PI) * exp(-1 * pow(x - m, 2) / (2 * v));
}

double normalGaussian(double x) {
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 0.5*(1.0 + sign*y);
}

double Linear::value(const vector<double> &args) {
    if (args.size() < 1) return 0;

    return linear(args[0]);
}

double NormalGaussian::value(const vector<double> &args) {
    if (args.size() < 1) return 0;

    return normalGaussian(args[0]);
}

double normalGaussianInverse(double p) {
    if (p <= 0.0 || p >= 1.0) {
        //        std::stringstream os;
        //        os << "Invalid input argument (" << p
        //        << "); must be larger than 0 but less than 1.";
        //        throw std::invalid_argument( os.str() );
        return 0;
    }

    // See article above for explanation of this section.
    if (p < 0.5) {
        // F^-1(p) = - G^-1(p)
        return -rationalApproximation( sqrt(-2.0*log(p)) );
    } else {
        // F^-1(p) = G^-1(1-p)
        return rationalApproximation( sqrt(-2.0*log(1-p)) );
    }
}

double rationalApproximation(double t) {
    // Abramowitz and Stegun formula 26.2.23.
    // The absolute value of the error should be less than 4.5 e-4.
    double c[] = {2.515517, 0.802853, 0.010328};
    double d[] = {1.432788, 0.189269, 0.001308};
    return t - ((c[2]*t + c[1])*t + c[0]) /
            (((d[2]*t + d[1])*t + d[0])*t + 1.0);
}

} // namespace Functions
} // namespace Math
