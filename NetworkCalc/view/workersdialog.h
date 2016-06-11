#ifndef WORKERSDIALOG_H
#define WORKERSDIALOG_H

#include <QVector>
#include <QDialog>
#include <QSpinBox>

#include "../math/networkgraph.h"

namespace Ui {
class WorkersDialog;
}

class WorkersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkersDialog(QWidget *parent = 0);
    ~WorkersDialog();

    void setWorkers(QVector<IWorker *> *value);

signals:
    void newWorker();
    void deleteWorker(IWorker*);

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
    Ui::WorkersDialog *ui;

    QVector<IWorker*> *workers;
};

#endif // WORKERSDIALOG_H
