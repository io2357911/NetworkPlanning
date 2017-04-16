#ifndef FUNCTION_H
#define FUNCTION_H

#include "interfaces.h"

namespace Math {
namespace Functions {
/**
 * @brief The IFunction class Интерфейс для представления функции
 */
class Linear : public IFunction {
public:
    double value(const vector<double> &args);
};

}
}

#endif // FUNCTION_H
