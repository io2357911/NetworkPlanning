#include "function.h"


namespace Math {
namespace Functions {


double Linear::value(const vector<double> &args) {
    if (args.size() < 1) return 0;

    return args[0];
}


} // namespace Functions
} // namespace Math
