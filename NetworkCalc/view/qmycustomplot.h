#ifndef QMYCUSTOMPLOT_H
#define QMYCUSTOMPLOT_H

#include "qcustomplot.h"
#include "../math/networkgraph.h"

using namespace Math::Planning;

class QMyCustomPlot : public QCustomPlot {
    Q_OBJECT
public:
    enum FunctionType { f, F };

    explicit QMyCustomPlot(QWidget *parent = 0);

    void setFunctionType(FunctionType type);

public slots:
    void onGraphComputed(NetworkGraph* graph);

private:
    QCPGraph*       data;
    FunctionType    m_fnType;
};

#endif // QMYCUSTOMPLOT_H
