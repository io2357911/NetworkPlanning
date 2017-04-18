#ifndef WORKRESOURSE_H
#define WORKRESOURSE_H

#include "../tools/ini.h"
#include "../math/networkgraph.h"

using namespace Math::Planning;

class WorkResourse : public Resourse {
public:
    WorkResourse(QString name = "", double quantity = 0, double cost = 0);

    void store(QString fileName);
    void restore(QString fileName);
};

#endif // WORKRESOURSE_H
