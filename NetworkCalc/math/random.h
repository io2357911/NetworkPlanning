#ifndef RANDOM_H
#define RANDOM_H

#include "interfaces.h"

namespace Math {
namespace Random {

/**
 * @brief The Value class Класс случайной величины после генерации (random()) сохраняющий значение
 */
class Value : public IRandom {
public:
    Value(IRandom* random = 0);

    // IRandom interface
    IFunction* f();
    IFunction* F();
    double mathExpected();
    void setMathExpected(double value);
    double dispersion();
    void setDispersion(double value);
    double random();

    void setRandom(IRandom *random);

    double value() const;
    void setValue(double value);

private:
    IRandom*    m_random;
    double      m_value;
};

class Beta : public IRandom {
public:
    Beta(double a, double b, double m);

    // IRandom interface
    virtual IFunction *f();
    virtual IFunction *F();
    virtual void setMathExpected(double value);
    virtual double mathExpected();
    virtual void setDispersion(double value);
    virtual double dispersion();
    virtual double random();

protected:
    double m_a;
    double m_b;
    double m_m;
};

class PertBeta : public Beta {
public:
    PertBeta(double a, double b, double m);

    // IRandom interface
    IFunction *f();
    IFunction *F();
    double mathExpected();
    double dispersion();
    double random();
};

class PertNormal : public IRandom {
public:
    PertNormal();

    // IRandom interface
    IFunction *f();
    IFunction *F();
    void setMathExpected(double value);
    double mathExpected();
    void setDispersion(double value);
    double dispersion();
    double random();

private:
    struct Function_f : public InnerFunction<PertNormal> {
        Function_f(PertNormal* parent = 0) : InnerFunction<PertNormal>(parent) {}

        // IFunction interface
        double value(const vector<double> &args);
    } m_f;
    struct Function_F : public InnerFunction<PertNormal> {
        Function_F(PertNormal* parent = 0) : InnerFunction<PertNormal>(parent) {}

        // IFunction interface
        double value(const vector<double> &args);
    } m_F;
    double m_mathExpected;
    double m_dispersion;
};

} // namespace Random
} // namespace Math

#endif // RANDOM_H
