#include "random.h"

#include <math.h>

namespace Math {

IFunction* Random::f() {
    if (!m_random) return 0;
    return m_random->f();
}

IFunction* Random::F() {
    if (!m_random) return 0;
    return m_random->F();
}

double Random::mathExpected() {
    if (!m_random) return 0;
    return m_random->mathExpected();
}

double Random::dispersion() {
    if (!m_random) return 0;
    return m_random->dispersion();
}

double Random::random() {
    if (!m_random) return 0;

    m_value = m_random->random();
    return m_value;
}

void Random::setRandom(IRandom *random) {
    if (m_random) delete m_random;
    m_random = random;
}

double Random::value() const {
    return m_value;
}

namespace Randoms {


Beta::Beta(double a, double b, double m)
    : m_a(a), m_b(b), m_m(m)
{}

IFunction *Beta::f() {
    return 0;
}

IFunction *Beta::F() {
    return 0;
}

double Beta::mathExpected() {
    return 0;
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

IFunction *PertBeta::f() {
    return 0;
}

IFunction *PertBeta::F() {
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


} // namespace Randoms

} // namespace Math
