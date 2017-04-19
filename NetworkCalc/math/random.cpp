#include "random.h"
#include <math.h>
#include "function.h"
#include <stdlib.h>

namespace Math {


double Random::f(double) {
    return 0;
}

double Random::F(double) {
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


double Uniform::_random() {
    return ((double)::rand() / (double)(RAND_MAX));
}


double Empirical::f(double value) {
    return 0;
}

double Empirical::F(double value) {
    return 0;
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
}

double Empirical::value() {
    return mathExpected();
}

double Empirical::_random() {
    return 0;
}


Beta::Beta(double a, double b, double m)
    : m_a(a), m_b(b), m_m(m)
{}

double Beta::_random() {
    return 0;
}


PertBeta::PertBeta(double a, double b, double m)
    : Beta(a, b, m)
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
    return f(val, m_a, m_b, m_m);
}

double PertBeta::F(double val) {
    return F(val, m_a, m_b, m_m);
}

double PertBeta::mathExpected() {
    return mathExpected(m_a, m_b, m_m);
}

double PertBeta::dispersion() {
    return dispersion(m_a, m_b, m_m);
}

double PertBeta::_random() {
    return random(m_a, m_b, m_m);
}


PertNormal::PertNormal()
{}

double PertNormal::F(double val) {
    double M = mathExpected();
    double V = dispersion();

    return normalGaussian((val - M)/sqrt(V));
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



Triangle::Triangle(double a, double b, double m)
    : m_a(a), m_b(b), m_m(m)
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

double Triangle::f(double x) {
    return f(x, m_a, m_b, m_m);
}

double Triangle::F(double x) {
    return F(x, m_a, m_b, m_m);
}

double Triangle::mathExpected() {
    return mathExpected(m_a, m_b, m_m);
}

double Triangle::dispersion() {
    return dispersion(m_a, m_b, m_m);
}

double Triangle::random() {
    return random(m_a, m_b, m_m);
}

} // namespace Randoms
} // namespace Math
