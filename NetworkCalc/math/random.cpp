#include "random.h"
#include <math.h>
#include "function.h"
#include <stdlib.h>

namespace Math {
namespace Random {

using namespace Functions;

double Uniform::rand() {
    return ((double)::rand() / (double)(RAND_MAX));;
}

double Uniform::random() {
    return rand();
}


Value::Value(IRandom *random)
    : m_random(random)
{}

double Value::f(double prob) {
    if (!m_random) return 0;
    return m_random->f(prob);
}

double Value::F(double prob) {
    if (!m_random) return 0;
    return m_random->F(prob);
}

double Value::mathExpected() {
    if (!m_random) return 0;
    return m_random->mathExpected();
}

void Value::setMathExpected(double value) {
    if (!m_random) return;
    m_random->setMathExpected(value);
}

double Value::dispersion() {
    if (!m_random) return 0;
    return m_random->dispersion();
}

void Value::setDispersion(double value) {
    if (!m_random) return;
    m_random->setDispersion(value);
}

double Value::random() {
    if (!m_random) return 0;

    m_value = m_random->random();
    return m_value;
}

void Value::setRandom(IRandom *random) {
    if (m_random) delete m_random;
    m_random = random;
}

double Value::value() const {
    return m_value;
}

void Value::setValue(double value) {
    m_value = value;
}


Beta::Beta(double a, double b, double m)
    : m_a(a), m_b(b), m_m(m)
{}

double Beta::f(double val) {
    return 0;
}

double Beta::F(double val) {
    return 0;
}

void Beta::setMathExpected(double value) {

}

double Beta::mathExpected() {
    return 0;
}

void Beta::setDispersion(double value) {

}

double Beta::dispersion() {
    return 0;
}

double Beta::random() {
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

double PertBeta::random() {
    return random(m_a, m_b, m_m);
}


PertNormal::PertNormal()
{}

double PertNormal::f(double val) {
    return 0;
}

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

double PertNormal::random() {
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
    double y = Uniform::rand();

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

void Triangle::setMathExpected(double) {
}

double Triangle::mathExpected() {
    return mathExpected(m_a, m_b, m_m);
}

void Triangle::setDispersion(double) {
}

double Triangle::dispersion() {
    return dispersion(m_a, m_b, m_m);
}

double Triangle::random() {
    return random(m_a, m_b, m_m);
}

} // namespace Randoms
} // namespace Math
