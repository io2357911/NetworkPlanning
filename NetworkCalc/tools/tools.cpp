#include "tools.h"

namespace Tools {

QString formatDouble(double value) {
    QString text;
    text.sprintf("%.1f", value);
    return text;

//    return QString::number(value, 'g', 3);
}

}


