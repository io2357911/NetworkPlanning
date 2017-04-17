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
    virtual double mathExpected();
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
    double mathExpected();
    double dispersion();
    double random();

    void setMathExpected(double value);
    void setDispersion(double value);

private:
    double m_mathExpected;
    double m_dispersion;
};

} // namespace Random
} // namespace Math

#endif // RANDOM_H
