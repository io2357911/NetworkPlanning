#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include "math/networkgraph.h"

class Debug
{
private:
    Debug();

public:
    static void printGraph(NetworkGraph &graph);
    static void printEdgesInfo(QVector<IWork*> edges);

    static void printEvents(NetworkGraph &graph);

    static void printCriticalPath(const QVector<IWork*> &path);
};

#endif // TOOLS_H
