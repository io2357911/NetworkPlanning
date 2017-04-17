#include "random.h"
#include <math.h>
#include "function.h"

namespace Math {
namespace Random {

Value::Value(IRandom *random)
    : m_random(random)
{}

IFunction* Value::f() {
    if (!m_random) return 0;
    return m_random->f();
}

IFunction* Value::F() {
    if (!m_random) return 0;
    return m_random->F();
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


PertNormal::PertNormal()
{}

IFunction *PertNormal::f() {
    return 0;
}

IFunction *PertNormal::F() {
    return 0;
}

//IFunction *PertNormal::F() {
//    class inner : public IFunction {
//    public:
//        inner(PertNormal* p) : m_p(p) {}

//        double value(const vector<double> &args) {
//            if (args.size() < 1) return 0;
//            if (m_p->dispersion() == 0) return 0;

//            double x = args[0];

//            return Math::Functions::NormalGaussian::instance()
//                    ->value((x-m_p->mathExpected())/sqrt(m_p->dispersion()));
//            return 0;
//        }

//    private:
//        PertNormal* m_p;

//    } m_F(this);

//    return &m_F;
//}

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

//double PertNormal::innerF::value(const vector<double> &args) {
//    if (args.size() < 1) return 0;
//    if (m_dispersion == 0) return 0;

//    double x = args[0];

//    return Math::Functions::NormalGaussian::instance()
//            ->value((x-m_mathExpected)/sqrt(m_dispersion));
//}




} // namespace Randoms

} // namespace Math
