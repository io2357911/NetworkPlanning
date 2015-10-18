#include "mainwindow.h"
#include <QApplication>

#include "math/algoritms.h"
using namespace PlanningAlgoritms;
using namespace std;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    /*AssignmentAlgoritm *alg(new HungarianAlgorithm);

    double a[3][3] = {
        {1,3,5},
        {5,2,7},
        {11,13,19},
    };

    Matrix m((double*)a, 3, 3);

    QVector<int> result;
    if (alg->compute(m, result)) {
        for (int j = 0; j < result.size(); j++) {
            qDebug("n: %d m: %d", result[j], j);
        }
    }

    delete alg;*/

    /*CriticalPathAlgoritm *alg(new DPCPAlgoritm);


    double a[6][6] = {
        {0,1,1,0,0,0},
        {0,0,1,0,1,0},
        {0,0,0,1,0,0},
        {0,0,0,0,0,1},
        {0,0,0,0,0,1},
        {0,0,0,0,0,0}
    };

    Matrix m((double*)a, 6, 6);
    */

    /*double a[10][10] = {
      //{0,1,2,3,4,5,6,7,8,9},
        {0,1,5,4,0,0,0,0,0,0},// 0
        {0,0,6,0,0,0,9,0,0,0},// 1
        {0,0,0,1,0,5,0,0,0,0},// 2
        {0,0,0,0,4,3,0,0,0,0},// 3
        {0,0,0,0,0,2,0,0,3,5},// 4
        {0,0,0,0,0,0,0,3,7,0},// 5
        {0,0,0,0,0,0,0,1,0,0},// 6
        {0,0,0,0,0,0,0,0,1,3},// 7
        {0,0,0,0,0,0,0,0,0,1},// 8
        {0,0,0,0,0,0,0,0,0,0} // 9
    };

    Matrix m((double*)a, 10, 10);

    Graph g(m);

    double result;
    if (alg->compute(g, result)) {
        qDebug("maxLength: %f", result);
    }

    delete alg;

    return 0;*/
}
