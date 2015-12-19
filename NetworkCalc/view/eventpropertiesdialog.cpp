#include "eventpropertiesdialog.h"
#include "ui_eventpropertiesdialog.h"

EventPropertiesDialog::EventPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventPropertiesDialog),
    event(NULL)
{
    ui->setupUi(this);
}

EventPropertiesDialog::~EventPropertiesDialog()
{
    delete ui;
}

void EventPropertiesDialog::on_buttonBox_accepted()
{
    if (!event) return;

    event->setID(ui->sbID->value());
}

void EventPropertiesDialog::setEvent(IEvent *value)
{
    if (!value) return;

    event = value;

    ui->sbID->setValue(event->getID());
    ui->sbEarlyTime->setValue(event->getEarlyTime());
    ui->sbLateTime->setValue(event->getLateTime());
    ui->sbReserve->setValue(event->getReserve());
}

