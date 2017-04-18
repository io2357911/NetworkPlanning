#include "random.h"
#include <math.h>
#include "function.h"

namespace Math {
namespace Random {

using namespace Functions;

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

double PertBeta::f(double val) {
    return 0;
}

double PertBeta::F(double val) {
    return 0;
}

double PertBeta::mathExpected() {
    return (m_a + 4*m_m + m_b) / 6;
}

double PertBeta::dispersion() {
    return pow((m_b - m_a) / 6, 2);
}

double PertBeta::random() {
    return mathExpected();
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

} // namespace Randoms
} // namespace Math
