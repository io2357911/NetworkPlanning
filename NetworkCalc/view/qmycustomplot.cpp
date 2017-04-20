#include "qmycustomplot.h"
#include "../project.h"

using namespace Math;

QMyCustomPlot::QMyCustomPlot(QWidget *parent)
    : QCustomPlot(parent)/*, minY(0), maxY(0)*/ {

    data = addGraph();

    // set names and colors:
    QPen pen;
    pen.setWidthF(1.2);
//    data->setName("Функция распределения F(x)");
    pen.setColor(QColor(150, 222, 0));
    data->setPen(pen);
    data->setBrush(QColor(150, 222, 0, 70));

    // prepare y axis:
    yAxis->setPadding(10); // a bit more space to the left border
    yAxis->setLabel("Вероятность");
    xAxis->setLabel("Значение");
    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    gridPen.setStyle(Qt::DotLine);

    // setup legend:
    legend->setVisible(true);
    legend->setBrush(QColor(255, 255, 255, 200));
    QPen legendPen;
    legendPen.setColor(QColor(130, 130, 130, 200));
    legend->setBorderPen(legendPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    legend->setFont(legendFont);

    setFunctionType(f);
}

void QMyCustomPlot::setFunctionType(QMyCustomPlot::FunctionType type) {
    this->m_fnType = type;

    if (m_fnType == f) {
        data->setName("Функция плотности вероятности f(x)");
    } else {
        data->setName("Функция распределения F(x)");
    }
}

void QMyCustomPlot::onGraphChanged() {
    Random* time = Project::instance()->graph()->time();
    if (!time) return;

    double m = time->mathExpected();
    double v = time->dispersion();

    double minX = m - 1 * v;
    if (minX < 0) minX = 0;
    double maxX = m + 1 * v;
    xAxis->setRange(minX, maxX);
    yAxis->setRange(0, 1);

    QVector<double> x, y;
    for (double xi = minX; xi <= maxX; xi += 1) {
        x.append(xi);
        y.append(m_fnType == f ? time->f(xi) : time->F(xi));
    }

    data->setData(x, y);

    replot();
}
