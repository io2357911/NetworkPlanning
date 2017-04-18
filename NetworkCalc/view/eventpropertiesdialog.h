#ifndef EVENTPROPERTIESDIALOG_H
#define EVENTPROPERTIESDIALOG_H

#include <QDialog>
#include "../math/networkgraph.h"

using namespace Math::Planning;

namespace Ui {
class EventPropertiesDialog;
}

class EventPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EventPropertiesDialog(QWidget *parent = 0);
    ~EventPropertiesDialog();

    void setEvent(Event *value);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EventPropertiesDialog *ui;

    Event *event;
};

#endif // EVENTPROPERTIESDIALOG_H
