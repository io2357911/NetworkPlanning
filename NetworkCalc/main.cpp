#include "mainwindow.h"
#include <QApplication>

#include "math/networkgraph.h"
#include "math/algorithms.h"
#include "tools/debug.h"

using namespace Math;
using namespace Math::Planning;
using namespace Math::Planning::Algorithms;
using namespace Math::Planning::Random;

void testNetworkGraph() {
    Resourse res("People", 20, 1);

    PertBetaFactory rnd;

    Event e1("1"), e2("2"), e3("3"), e4("4"), e5("5"), e6("6"), e7("7");
    Work    w1(&e1,&e3,"1",&res,1,1,4,3,&rnd),
            w2(&e1,&e2,"2",&res,1,1,6,3,&rnd),
            w3(&e3,&e4,"3",&res,1,1,7,3,&rnd),
            w4(&e2,&e5,"4",&res,1,1,3,2,&rnd),
            w5(&e4,&e6,"5",&res,1,1,4,2,&rnd),
            w6(&e5,&e6,"6",&res,1,1,5,3,&rnd),
            w7(&e6,&e7,"7",&res,1,1,3,2,&rnd),
            pw(&e2,&e3,"8",&res,1,0,0,0,&rnd);

    QVector<Event*> events = {
        &e1, &e2, &e3, &e4, &e5, &e6, &e7,
    };
    QVector<Work*> works = {
        &w1, &w2, &w3, &w4, &w5, &w6, &w7, &pw
    };

    NetworkGraph graph(events, works);

    Debug::printGraph(graph);

    if (!PertNetworkAlgorithm(&graph).compute()) {
        qDebug("Network Graph PERT Computation - FAILED!");
        return;
    }

//    NetworkAlgorithm(&graph).compute();
//    CriticalPathAlgorithm(&graph).compute();
//    TimeAlgorithm(&graph).compute();
//    CostAlgorithm(&graph).compute();

    Debug::printEvents(graph);
    Debug::printEdges(graph.edges());
    Debug::printCriticalPath(graph.criticalPath());
}


int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

//    testNetworkGraph();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    return 0;
}
