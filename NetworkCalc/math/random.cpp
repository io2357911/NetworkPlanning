#include "random.h"
#include <math.h>
#include "function.h"

namespace Math {
namespace Random {

using namespace Functions;

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
    : m_f(this), m_F(this)
{}

IFunction *PertNormal::f() {
//    struct Function : public IFunction {
//        PertNormal* p;
//        Function_F(PertNormal* parent) : p(parent) {}

//        double value(const vector<double> &args) {
//            if (args.size() < 1) return 0;

//            return 0;
//        }
//    };

//    return new Function(this);
    return &m_f;
}

IFunction *PertNormal::F() {
//    struct Function : public IFunction {
//        PertNormal* p;
//        Function_F(PertNormal* parent) : p(parent) {}

//        double value(const vector<double> &args) {
//            if (args.size() < 1) return 0;
//            if (p->dispersion() == 0) return 0;

//            double x = args[0];
//            double M = p->mathExpected();
//            double V = p->dispersion();

//            return normalGaussian((x - M)/sqrt(V));
//        }
//    };

//    return new Function(this);
    return &m_F;
}

//template <typename P>
//struct InnerFunction : public IFunction {
//public:
//    P* p;
//    InnerFunction(P* parent) : p(parent) {}
//};

//IFunction *PertNormal::F() {
//    return new (class : IFunction {

//        double value(const vector<double> &args) {
////            if (args.size() < 1) return 0;
////            if (p->dispersion() == 0) return 0;

////            double x = args[0];
////            double M = p->mathExpected();
////            double V = p->dispersion();

////            return normalGaussian((x - M)/sqrt(V));
//            return 0;
//        }
//    });
//    return 0;
////    return new Function(this);

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

double PertNormal::Function_f::value(const vector<double> &args) {
    if (args.size() < 1) return 0;

    return 0;
}

double PertNormal::Function_F::value(const vector<double> &args) {
    if (args.size() < 1) return 0;
    if (p->dispersion() == 0) return 0;

    double x = args[0];
    double M = p->mathExpected();
    double V = p->dispersion();

    return normalGaussian((x - M)/sqrt(V));
}

} // namespace Randoms
} // namespace Math
