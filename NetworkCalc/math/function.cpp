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

} // namespace Functions
} // namespace Math
