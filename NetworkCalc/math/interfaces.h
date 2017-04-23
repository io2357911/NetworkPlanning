#ifndef INTERFACES_H
#define INTERFACES_H

#define IS_ZERO(dbl, eps) (dbl < eps)

#include <vector>

using namespace std;

/**
 * @brief Содержит описания математических классов и структур
 */
namespace Math {

/**
 * @brief The IFunction class Интерфейс для представления функции
 */
class IFunction {
public:
    virtual double value(const vector<double> &args) = 0;
    double value(const double &arg) { return value(vector<double>({arg})); }
};

//template <typename P>
//struct InnerFunction : public IFunction {
//    P* p;
//    InnerFunction(P* parent) : p(parent) {}
//};

/**
 * @brief The IRandom class Интерфейс для представления случайной величины
 */
class IRandom {
public:
    virtual ~IRandom() {}
    /**
     * @brief f функция плотности вероятности
     * @return
     */
//    virtual IFunction* f() = 0;
    virtual double f(double value) = 0;
    /**
     * @brief F функция распределения вероятностей
     * @return
     */
//    virtual IFunction* F() = 0;
    virtual double F(double value) = 0;
    /**
     * @brief invF обратная функция распределения вероятностей
     * @param value
     * @return
     */
    virtual double invF(double value) = 0;
    /**
     * @brief setMathExpected
     * @param value
     */
    virtual void setMathExpected(double value) = 0;
    /**
     * @brief mathExpected мат. ожидание
     * @return
     */
    virtual double mathExpected() = 0;
    /**
     * @brief setDispersion
     * @param value
     */
    virtual void setDispersion(double value) = 0;
    /**
     * @brief dispersion дисперсия
     * @return
     */
    virtual double dispersion() = 0;
    /**
     * @brief value случайное значение
     * @return
     */
    virtual double random() = 0;
    /**
     * @brief setValue
     * @param value
     */
    virtual void setValue(double value) = 0;
    /**
     * @brief value
     * @return
     */
    virtual double value() = 0;
};

/**
 * @brief The IAlgorithm class Интерфеййс некоторого алгоритма
 */
class IAlgorithm {
public:
    virtual ~IAlgorithm() {}

    virtual bool compute() = 0;

};

class IIterator {
public:
    virtual ~IIterator() {}

    virtual bool reset() = 0;
    virtual bool toNext() = 0;
};

template <typename T>
class Iterator : public IIterator {
public:
    virtual ~Iterator() {}

    virtual T next() = 0;
};

}

#endif // INTERFACES_H
