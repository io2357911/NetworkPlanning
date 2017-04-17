#ifndef FUNCTION_H
#define FUNCTION_H

#include "interfaces.h"

namespace Math {
namespace Functions {

double linear(double x);

/**
 * @brief The IFunction class y = x
 */
class Linear : public IFunction {
public:
    static IFunction* instance() { static Linear f; return &f; }

    double value(const vector<double> &args);
};

double normalGaussian(double x);

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
