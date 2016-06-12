#ifndef WORKERSWIDGET_H
#define WORKERSWIDGET_H

#include <QWidget>
#include <QSpinBox>

#include "../math/networkgraph.h"

namespace Ui {
class WorkersWidget;
}

class WorkersWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WorkersWidget(QWidget *parent = 0);
    ~WorkersWidget();

    void setWorkers(QVector<IWorker *> *value);

signals:
    void newWorker();
    void deleteWorker(IWorker*);

    void workerChanged();

public slots:
    void updateWorkers();
    void updateWorks();

private slots:
    void on_pbWorkerAdd_clicked();
    void on_pbWorkerDelete_clicked();

    void on_cbWorker_currentIndexChanged(int index);
    void onWorkCostChanged(int value);
    void onWorkTimeChanged(int value);

private:
    IWorker *currentWorker();
    IWork *getWork(QSpinBox *box);

private:
    Ui::WorkersWidget *ui;

    QVector<IWorker*> *workers;
};

#endif // WORKERSWIDGET_H
