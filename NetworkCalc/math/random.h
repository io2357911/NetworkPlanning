#ifndef RANDOM_H
#define RANDOM_H

#include "interfaces.h"

namespace Math {

/**
 * @brief The Random class Класс случайной величины после генерации (random()) сохраняющий значение
 */
class Random : IRandom {
public:
    Random(IRandom* random = 0) : m_random(random) {}

    // IRandom interface
    IFunction* f();
    IFunction* F();
    double mathExpected();
    double dispersion();
    double random();

    void setRandom(IRandom *random);

    double value() const;

private:
    IRandom*    m_random;
    double      m_value;
};

namespace Randoms {

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

}

}

#endif // RANDOM_H
