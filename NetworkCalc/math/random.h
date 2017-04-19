#ifndef RANDOM_H
#define RANDOM_H

#include "interfaces.h"

namespace Math {
namespace Random {

class Uniform : public IRandom {
public:
    static double rand();

    virtual double random();
};

/**
 * @brief The Value class Класс случайной величины после генерации (random()) сохраняющий значение
 */
class Value : public IRandom {
public:
    Value(IRandom* random = 0);

    // IRandom interface
    double f(double prob);
    double F(double prob);
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
    double f(double val);
    double F(double val);
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

    static double f(double val, double a, double b, double m);
    static double F(double val, double a, double b, double m);
    static double mathExpected(double a, double b, double m);
    static double dispersion(double a, double b, double m);
    static double random(double a, double b, double m);

    // IRandom interface
    double f(double val);
    double F(double val);
    double mathExpected();
    double dispersion();
    double random();
};

class PertNormal : public IRandom {
public:
    PertNormal();

    // IRandom interface
    double f(double val);
    double F(double val);
    void setMathExpected(double value);
    double mathExpected();
    void setDispersion(double value);
    double dispersion();
    double random();

private:
    double m_mathExpected;
    double m_dispersion;
};

/**
 * @brief The Triangle class Треугольное распределение
 * http://rdostudio.raox.ru/help/help/rdo_lang_rus/html/rdo_theory/rdo_theory_seq_triangular.htm
 */
class Triangle : public IRandom {
public:
    Triangle(double a, double b, double m);

    static double f(double val, double a, double b, double m);
    static double F(double val, double a, double b, double m);
    static double mathExpected(double a, double b, double m);
    static double dispersion(double a, double b, double m);
    static double random(double a, double b, double m);

    // IRandom interface
    double f(double val);
    double F(double val);
    void setMathExpected(double value);
    double mathExpected();
    void setDispersion(double value);
    double dispersion();
    double random();

private:
    double m_a;
    double m_b;
    double m_m;
};

} // namespace Random
} // namespace Math

#endif // RANDOM_H
