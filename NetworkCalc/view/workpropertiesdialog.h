#ifndef WORKPROPERTIESDIALOG_H
#define WORKPROPERTIESDIALOG_H

#include <QDialog>
#include "../math/networkgraph.h"
#include "workresourse.h"

using namespace Math::Planning;

namespace Ui {
class WorkPropertiesDialog;
}

class WorkPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WorkPropertiesDialog(QWidget *parent = 0);
    ~WorkPropertiesDialog();

    void setWork(Work *value);
    Work* getWork() const;

    void setResourses(QVector<WorkResourse*>* resourses);

//    void updateWorkers();

public slots:
    void updateWork();
    void updateResourses();

private slots:
    void on_buttonBox_accepted();

private:
    Resourse* currentResourse();

private:
    Ui::WorkPropertiesDialog *ui;

    Work *work;
    QVector<WorkResourse*>* resourses;
};

#endif // WORKPROPERTIESDIALOG_H
