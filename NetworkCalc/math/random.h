#ifndef RANDOM_H
#define RANDOM_H

#include <QVector>
#include "interfaces.h"


namespace Math {

/**
 * @brief The Random class Класс случайной величины после генерации (random()) сохраняющий значение
 */
class Random : public IRandom {
public:
    virtual ~Random() {}
    /**
     * @brief neumannRandom Разыгрывает случайную величину методом Неймана
     * @param f - функция плотности вероятности
     * @param a - начало интервала
     * @param b - конец интервала
     * @param m - такое значение, при ктором fn->value(m) -> max
     * @return случайная величина x распределенная по f(x)
     *
     * http://sernam.ru/book_dm.php?id=7
     */
    static double neumannRandom(IFunction *f, double a, double b, double m);

    // IRandom interface
    virtual double f(double value);
    virtual double F(double value);
    virtual double invF(double value);
    virtual void setMathExpected(double value);
    virtual double mathExpected();
    virtual void setDispersion(double value);
    virtual double dispersion();
    virtual void setValue(double value);
    virtual double value();

    double random();

protected:
    virtual double _random() = 0;

protected:
    double m_value;
};

namespace Randoms {

class Uniform : public Random {
protected:
    double _random();
};

typedef QVector<double> Intervals;

/**
 * @brief The Empirical class Случайная величина, полученная вследствии эксперимента
 */
class Empirical : public Random {
public:
    Empirical(uint intervals = 10);

    // Random interface
    double f(double value);
    double F(double value);
    double invF(double value);
    double mathExpected();
    double dispersion();
    void setValue(double value);
    double value();
    double _random();

private:
    void prepareFunctions();
    void clearFunctions();

    Intervals countByIntervals(const QVector<double>& vals, const Intervals &intervals);
    Intervals createIntervals(double from, double to, int count);
    int intervalIndex(const Intervals &intervals, double val);

    Intervals integrate(Intervals vals);

private:
    uint m_intervals;
    QVector<double> m_vals;
    Intervals m_xi, m_f, m_F;
};


/**
 * @brief The Beta class
 *
 * Голенко Д. И. Статистические методы сетевого планирования и управления. М.: Наука, 1968. (уклон на математику) - c. 97
 */
class Beta : public Random {
public:
    Beta(double a, double b, double m);

    static double f(double val, double a, double b, double m);
    static double F(double val, double a, double b, double m);
    static double mathExpected(double a, double b, double m);
    static double dispersion(double a, double b, double m);
    static double random(double a, double b, double m);

    // Random interface
    virtual double f(double value);
//    double F(double value);
    virtual double mathExpected();
    virtual double dispersion();
    virtual double _random();

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

    // Random interface
    double f(double val);
    double F(double val);
    double mathExpected();
    double dispersion();
    double _random();
};

class PertNormal : public Random {
public:
    PertNormal();

    // Random interface
    double f(double value);
    double F(double value);
    double invF(double value);
    void setMathExpected(double value);
    double mathExpected();
    void setDispersion(double value);
    double dispersion();
    double _random();

private:
    double m_mathExpected;
    double m_dispersion;
};

/**
 * @brief The Triangle class Треугольное распределение
 * http://rdostudio.raox.ru/help/help/rdo_lang_rus/html/rdo_theory/rdo_theory_seq_triangular.htm
 */
class Triangle : public Random {
public:
    Triangle(double a, double b, double m);

    static double f(double val, double a, double b, double m);
    static double F(double val, double a, double b, double m);
    static double invF(double val, double a, double b, double m);
    static double mathExpected(double a, double b, double m);
    static double dispersion(double a, double b, double m);
    static double random(double a, double b, double m);

    // IRandom interface
    double f(double value);
    double F(double value);
    double invF(double value);
    double mathExpected();
    double dispersion();
    double _random();

private:
    double m_a;
    double m_b;
    double m_m;
};

} // namespace Random
} // namespace Math

#endif // RANDOM_H
