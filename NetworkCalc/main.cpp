//#include "mainwindow.h"
//#include <QApplication>

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

double phi(double x) {
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 0.5*(1.0 + sign*y);
}

//#include <vector>

void testPhi() {
    vector<double> x = vector<double>({
//        -5.604485383,
//        -5.329979977,
//        -5.05547457,
//        -4.780969164,
//        -4.506463757,
//        -4.231958351,
//        -3.957452944,
//        -3.682947538,
//        -3.408442131,
//        -3.133936724,
//        -2.859431318,
//        -2.584925911,
//        -2.310420505,
        -2.035915098,
        1.25814978,
    });

    for (uint i = 0; i < x.size(); i++) {
        qDebug("phi(%f) = %f", x[i], phi(x[i]));
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

//    testNetworkGraph();
    testPhi();


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
