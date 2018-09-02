#include "random.h"
#include <math.h>
#include "function.h"
#include <stdlib.h>

namespace Math {

using namespace Functions;

double Random::neumannRandom(IFunction *f, double a, double b, double m) {
    if (!f)    return 0;
    if (a >= b) return 0;

    while (1) {
        double x1 = Randoms::Uniform().random();
        double x2 = Randoms::Uniform().random();

        x1 = a + (b - a) * x1;
        x2 = f->value(m) * x2;

        if (x2 <= f->value(x1)) return x1;
    }
}

double Random::f(double) {
    return 0;
}

double Random::F(double) {
    return 0;
}

double Random::invF(double) {
    return 0;
}

void Random::setMathExpected(double) {
}

double Random::mathExpected() {
    return 0;
}

void Random::setDispersion(double) {
}

double Random::dispersion() {
    return 0;
}

void Random::setValue(double value) {
    m_value = value;
}

double Random::value() {
    return m_value;
}

double Random::random() {
    m_value = _random();
    return m_value;
}


namespace Randoms {

using namespace Functions;


Estimation::Estimation(double min, double max, double avg)
    : m_min(min), m_max(max), m_avg(avg)
{}

double Estimation::min() {
    return m_min;
}

void Estimation::setMin(double value) {
    m_min = value;
}

double Estimation::max() {
    return m_max;
}

void Estimation::setMax(double value) {
    m_max = value;
}

double Estimation::avg() {
    return m_avg;
}

void Estimation::setAvg(double value) {
    m_avg = value;
}

double Uniform::_random() {
    return ((double)::rand() / (double)(RAND_MAX));
}


Empirical::Empirical(uint intervals)
    : m_intervals(intervals)
{}

double Empirical::f(double value) {
    prepareFunctions();
    int x = intervalIndex(m_xi, value);
    return m_f[x];
}

double Empirical::F(double value) {
    prepareFunctions();
    int x = intervalIndex(m_xi, value);
    return m_F[x];
}

double Empirical::invF(double value) {
    prepareFunctions();
    int y = intervalIndex(m_F, value);
    return m_xi[y];
}

double Empirical::mathExpected() {
    if (m_vals.isEmpty()) return 0;

    double m = 0;
    double N = m_vals.size();
    for (int i = 0; i < N; i++) {
        m += m_vals[i];
    }

    return m / N;
}

double Empirical::dispersion() {
    if (m_vals.isEmpty()) return 0;

    double m = 0;
    double m2 = 0;
    double N = m_vals.size();
    for (int i = 0; i < N; i++) {
        double y = m_vals[i];
        m += y;
        m2 += pow(y, 2);
    }

    return m2 / N - pow(m / N, 2);
}

void Empirical::setValue(double value) {
    m_value = value;
    m_vals.append(value);
    clearFunctions();
}

double Empirical::value() {
    return mathExpected();
}

double Empirical::_random() {
    return mathExpected();
}

void Empirical::prepareFunctions() {
    if (m_F.isEmpty()) {
        m_xi = createIntervals(min(m_vals), max(m_vals), m_intervals);
        m_f = countByIntervals(m_vals, m_xi);
        m_F = integrate(m_f);
    }
}

void Empirical::clearFunctions() {
    m_xi.clear();
    m_f.clear();
    m_F.clear();
}

Intervals Empirical::countByIntervals(const QVector<double> &vals, const Intervals &intervals) {
    Intervals res(intervals.size(), 0);

    for (int i = 0; i < vals.size(); i++) {
        int ind = intervalIndex(intervals, vals[i]);
        res[ind]++;
    }
    for (int i = 0; i < res.size(); i++) {
        res[i] /= vals.size();
    }
    return res;
}

Intervals Empirical::createIntervals(double from, double to, int count) {
    double step = ((to - from) / count);
    Intervals interval(count + 1);

    double cur = from;
    for (int i = 0; i < count; i++) {
        interval[i] = cur;
        cur += step;
    }
    interval[count] = to;
    return interval;
}

int Empirical::intervalIndex(const Intervals &intervals, double val) {
    if (intervals.empty()) return -1;

    if (val < intervals[0]) return 0;
    for (int j = 0; j < intervals.size() - 1; j++) {
        double a = intervals[j];
        double b = intervals[j + 1];
        if (val >= a && val < b) {
            return j;
        }
    }
    return intervals.size() - 1;
}

Intervals Empirical::integrate(Intervals vals) {
    Intervals integral(vals.size(), 0);
    integral[0] = vals[0];
    for (int i = 1; i < vals.size(); i++) {
        integral[i] = integral[i - 1] + vals[i];
    }
    return integral;
}


Beta::Beta(IEstimation *est)
    : m_est(est)
{}

double Beta::f(double x, double a, double b, double /*m*/) {
    // 12/(b-a)^4*(x-a)(b-x)^2
    return 12 / pow(b-a, 4) * (x-a)*pow(b-x, 2);
}

double Beta::F(double /*val*/, double /*a*/, double /*b*/, double /*m*/) {
    return 0;
}

double Beta::mathExpected(double a, double b, double /*m*/) {
    // (3*a+2*b)/5
    return (3*a+2*b)/5;
}

double Beta::dispersion(double a, double b, double /*m*/) {
    // 0.04*(b-a)^2
    return 0.004*pow(b-a,2);
}

double Beta::random(double a, double b, double m) {
    struct Function : public IFunction {
        double _a,_b,_m;
        Function(double a, double b, double m) : _a(a), _b(b), _m(m) {}
        double value(const vector<double> &args) {
            return Beta::f(args[0], _a, _b, _m);
        }
    } f(a,b,m);

    double mod = (2*a+b)/3; // мода
//    double mod = mathExpected(a,b,m); // мода
    return neumannRandom(&f, a, b, mod);
}

double Beta::f(double x) {
    return f(x, m_est->min(), m_est->max(), m_est->avg());
}

double Beta::mathExpected() {
    return mathExpected(m_est->min(), m_est->max(), m_est->avg());
}

double Beta::dispersion() {
    return dispersion(m_est->min(), m_est->max(), m_est->avg());
}

double Beta::_random() {
    return random(m_est->min(), m_est->max(), m_est->avg());
}


PertBeta::PertBeta(IEstimation *est)
    : Beta(est)
{}

double PertBeta::f(double /*val*/, double /*a*/, double /*b*/, double /*m*/) {
    return 0;
}

double PertBeta::F(double /*val*/, double /*a*/, double /*b*/, double /*m*/) {
    return 0;
}

double PertBeta::mathExpected(double a, double b, double m) {
    return (a + 4*m + b) / 6;
}

double PertBeta::dispersion(double a, double b, double /*m*/) {
    return pow((b - a) / 6, 2);
}

double PertBeta::random(double a, double b, double m) {
    return mathExpected(a, b, m);
}

double PertBeta::f(double val) {
    return f(val, m_est->min(), m_est->max(), m_est->avg());
}

double PertBeta::F(double val) {
    return F(val, m_est->min(), m_est->max(), m_est->avg());
}

double PertBeta::mathExpected() {
    return mathExpected(m_est->min(), m_est->max(), m_est->avg());
}

double PertBeta::dispersion() {
    return dispersion(m_est->min(), m_est->max(), m_est->avg());
}

double PertBeta::_random() {
    return random(m_est->min(), m_est->max(), m_est->avg());
}


PertNormal::PertNormal()
{}

double PertNormal::f(double x) {
    double M = mathExpected();
    double V = dispersion();

//    return normalGaussianDensity((x - M)/sqrt(V), 0, 1);
    return normalGaussianDensity(x, M, V);
}

double PertNormal::F(double x) {
    double M = mathExpected();
    double V = dispersion();

    return normalGaussian((x - M)/sqrt(V));
}

double PertNormal::invF(double value) {
    double M = mathExpected();
    double V = dispersion();

    return normalGaussianInverse(value) * sqrt(V) + M;
}

double PertNormal::mathExpected() {
    return m_mathExpected;
}

double PertNormal::dispersion() {
    return m_dispersion;
}

double PertNormal::_random() {
    return mathExpected();
}

void PertNormal::setMathExpected(double value) {
    m_mathExpected = value;
}

void PertNormal::setDispersion(double value) {
    m_dispersion = value;
}



Triangle::Triangle(IEstimation *est)
    : m_est(est)
{}

double Triangle::f(double x, double a, double b, double c) {
    if (x < a) {
        return 0;

    } else if (x <= c) {
        // 2*(x-a)/((b-a)*(c-a))
        return 2*(x-a)/((b-a)*(c-a));

    } else if (x <= b) {
        // 2*(b-x)/((b-a)*(b-c))
        return 2*(b-x)/((b-a)*(b-c));

    } else {
        return 0;
    }
}

double Triangle::F(double x, double a, double b, double c) {
    if (x < a) {
        return 0;

    } else if (x < c) {
        // (x-a)^2/((b-a)*(c-a))
        return pow(x-a,2)/((b-a)*(c-a));

    } else if (x <= b) {
        // 1-(b-x)^2/((b-a)*(b-c))
        return 1-pow(b-x,2)/((b-a)*(b-c));

    } else {
        return 1;
    }
}

double Triangle::invF(double y, double a, double b, double c) {
    // (c-a)*(b-a)
    double m = (c-a)*(b-a);

    if (y <= m) {
        // sqrt(y*(b-a)*(c-a))+a
        return sqrt(y*(b-a)*(c-a))+a;

    } else {
        // b-sqrt((1-y)*(b-a)*(b-c))
        return b-sqrt((1-y)*(b-a)*(b-c));
    }
}

double Triangle::mathExpected(double a, double b, double c) {
    // (a+b+c)/3
    return (a+b+c)/3;
}

double Triangle::dispersion(double a, double b, double c) {
    // (a^2+b^2+c^2-a*b-a*c-b*c)/18
    return (pow(a,2)+pow(b,2)+pow(c,2)-a*b-a*c-b*c)/18;
}

double Triangle::random(double a, double b, double c) {
    double y = Uniform().random();
    return invF(y, a, b, c);
}

double Triangle::f(double x) {
    return f(x, m_est->min(), m_est->max(), m_est->avg());
}

double Triangle::F(double x) {
    return F(x, m_est->min(), m_est->max(), m_est->avg());
}

double Triangle::invF(double x) {
    return invF(x, m_est->min(), m_est->max(), m_est->avg());
}

double Triangle::mathExpected() {
    return mathExpected(m_est->min(), m_est->max(), m_est->avg());
}

double Triangle::dispersion() {
    return dispersion(m_est->min(), m_est->max(), m_est->avg());
}

double Triangle::_random() {
    return random(m_est->min(), m_est->max(), m_est->avg());
}

} // namespace Randoms
} // namespace Math
