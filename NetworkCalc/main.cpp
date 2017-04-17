//#include "mainwindow.h"
//#include <QApplication>

#include "math/networkgraph.h"
#include "math/algorithms.h"
#include "tools/debug.h"

using namespace Math;
using namespace Math::Planning;
using namespace Math::Planning::Algorithms;

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    Resourse res("People", 20, 1);

    Event e1("1"), e2("2"), e3("3"), e4("4"), e5("5"), e6("6"), e7("7");
    Work    w1(&e1,&e3,"1",&res,1,1,4,3),
            w2(&e1,&e2,"2",&res,1,1,6,3),
            w3(&e3,&e4,"3",&res,1,1,7,3),
            w4(&e2,&e5,"4",&res,1,1,3,2),
            w5(&e4,&e6,"5",&res,1,1,4,2),
            w6(&e5,&e6,"6",&res,1,1,5,3),
            w7(&e6,&e7,"7",&res,1,1,3,2),
            pw(&e2,&e3,"8",&res,1,0,0,0);

    QVector<Event*> events = {
        &e1, &e2, &e3, &e4, &e5, &e6, &e7,
    };
    QVector<Work*> works = {
        &w1, &w2, &w3, &w4, &w5, &w6, &w7, &pw
    };

    NetworkGraph graph(events, works);

    Debug::printGraph(graph);

    NetworkAlgorithm(&graph).compute();
    CriticalPathAlgorithm(&graph).compute();
    TimeAlgorithm(&graph).compute();
    CostAlgorithm(&graph).compute();


    Debug::printEvents(graph);
    Debug::printEdges(graph.edges());
    Debug::printCriticalPath(graph.criticalPath());


//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
//    return a.exec();


    return 0;
}

/*
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
*/
