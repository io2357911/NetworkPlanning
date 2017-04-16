#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include "math/algorithms.h"

using namespace Math;
using namespace Math::Planning;
using namespace Math::Planning::Algorithms;

class Debug
{
private:
    Debug();

public:
    static void printGraph(NetworkGraph &graph);
    static void printEdges(QVector<Work*> edges);
    static void printEvents(NetworkGraph &graph);

    static void printCriticalPath(const QVector<Work*> &path);
};

#endif // TOOLS_H
