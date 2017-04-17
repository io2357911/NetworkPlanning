#ifndef INTERFACES_H
#define INTERFACES_H

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
    virtual IFunction* f() = 0;
//    virtual double f(double prob) = 0;
    /**
     * @brief F функция распределения вероятностей
     * @return
     */
    virtual IFunction* F() = 0;
//    virtual double F(double prob) = 0;
    /**
     * @brief mathExpected мат. ожидание
     * @return
     */
    virtual double mathExpected() = 0;
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
};

/**
 * @brief The IAlgorithm class Интерфеййс некоторого алгоритма
 */
class IAlgorithm {
public:
    virtual bool compute() = 0;
};

}

#endif // INTERFACES_H
