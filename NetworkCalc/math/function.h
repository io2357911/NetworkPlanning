#ifndef FUNCTION_H
#define FUNCTION_H

#include "interfaces.h"
#include <QVector>

namespace Math {
namespace Functions {

double min(const QVector<double>& vec);
double max(const QVector<double>& vec);
/**
 * @brief linear
 * @param x
 * @return y = x
 */
double linear(double x);
/**
 * @brief normalGaussianDensity
 * @param x - значение
 * @param m - мат ожидание
 * @param v - дисперсия
 *
 * http://ru.math.wikia.com/wiki/%D0%9D%D0%BE%D1%80%D0%BC%D0%B0%D0%BB%D1%8C%D0%BD%D0%BE%D0%B5_%D1%80%D0%B0%D1%81%D0%BF%D1%80%D0%B5%D0%B4%D0%B5%D0%BB%D0%B5%D0%BD%D0%B8%D0%B5
 */
double normalGaussianDensity(double x, double m, double v);
/**
 * @brief normalGaussian
 * @param x
 * @return
 */
double normalGaussian(double x);

/**
 * @brief normalGaussianInverse
 * @param p
 * @return
 *
 * https://www.johndcook.com/blog/normal_cdf_inverse/
 */
double normalGaussianInverse(double p);
double rationalApproximation(double t);

/**
 * @brief The IFunction class y = x
 */
class Linear : public IFunction {
public:
    static IFunction* instance() { static Linear f; return &f; }

    double value(const vector<double> &args);
};

/**
 * @brief The IFunction class Функция нормального распределения F(x)
 * https://www.johndcook.com/blog/cpp_phi/
 */
class NormalGaussian : public IFunction {
public:
    static IFunction* instance() { static NormalGaussian f; return &f; }

    double value(const vector<double> &args);
};

}
}

#endif // FUNCTION_H
