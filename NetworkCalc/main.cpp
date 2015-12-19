#include "mainwindow.h"
#include <QApplication>

#include "math/networkgraph.h"
#include "math/algorithms.h"
#include "tools/tools.h"

using namespace PlanningAlgoritms;

int main(int argc, char *argv[])
{
    /*Work w1(1,1,4), w2(2,1,6), w3(3,1,7), w4(4,1,3), w5(5,1,4), w6(6,1,5), w7(7,1,3), pw(8,0,0);
    Event e1(1), e2(2), e3(3), e4(4), e5(5), e6(6), e7(7);

    QVector<IEvent*> events = {&e1, &e2, &e3, &e4, &e5, &e6, &e7};
    QVector<IWork*> works = {&w1, &w2, &w3, &w4, &w5, &w6, &w7, &pw};

    int n = -1;
    int a[7][7] = {
        {n, 1, 0, n, n, n, n},
        {n, n, 7, n, 3, n, n},
        {n, n, n, 2, n, n, n},
        {n, n, n, n, n, 4, n},
        {n, n, n, n, n, 5, n},
        {n, n, n, n, n, n, 6},
        {n, n, n, n, n, n, n},
    };
    Matrix<int> m((int*)a, 7, 7);

    NetworkGraph graph(events, works, m);
    Debug::printGraph(graph);

    NetworkGraphAlgoritm().compute(&graph);

    Debug::printEvents(graph);
    Debug::printCriticalPath(CriticalPathAlgorithm().compute(&graph));

    return 0;*/

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
