#include "mainwindow.h"
#include <QApplication>

#include "math/networkgraph.h"
#include "math/algorithms.h"
#include "tools/debug.h"

using namespace PlanningAlgoritms;

int main(int argc, char *argv[])
{
    Work w1(1,1,4), w2(2,1,6), w3(3,1,7), w4(4,1,3), w5(5,1,4), w6(6,1,5), w7(7,1,3), pw(8,0,0);
    Event e1(1), e2(2), e3(3), e4(4), e5(5), e6(6), e7(7);

    QVector<NetworkGraph::Vertex*> vertices = {
        new NetworkGraph::Vertex(&e1), // 1
        new NetworkGraph::Vertex(&e2), // 2
        new NetworkGraph::Vertex(&e3), // 3
        new NetworkGraph::Vertex(&e4), // 4
        new NetworkGraph::Vertex(&e5), // 5
        new NetworkGraph::Vertex(&e6), // 6
        new NetworkGraph::Vertex(&e7), // 7
    };
    QVector<NetworkGraph::Edge*> edges = {
        new NetworkGraph::Edge(&w1, vertices[0], vertices[2]),
        new NetworkGraph::Edge(&w2, vertices[0], vertices[1]),
        new NetworkGraph::Edge(&w3, vertices[2], vertices[3]),
        new NetworkGraph::Edge(&w4, vertices[1], vertices[4]),
        new NetworkGraph::Edge(&w5, vertices[3], vertices[5]),
        new NetworkGraph::Edge(&w6, vertices[4], vertices[5]),
        new NetworkGraph::Edge(&w7, vertices[5], vertices[6]),
        new NetworkGraph::Edge(&pw, vertices[1], vertices[2]),
    };



    NetworkGraph graph(vertices, edges);

    Debug::printGraph(graph);

    NetworkGraphAlgoritm().compute(&graph);

    Debug::printEvents(graph);
    Debug::printCriticalPath(CriticalPathAlgorithm().compute(&graph));


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
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
