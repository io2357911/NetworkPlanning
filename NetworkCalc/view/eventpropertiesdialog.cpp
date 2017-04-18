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
    event->setName(ui->leName->text());
}

void EventPropertiesDialog::setEvent(Event *value) {
    if (!value) return;

    event = value;

    ui->leName->setText(event->name());
    ui->sbEarlyTime->setValue(event->earlyTime());
    ui->sbLateTime->setValue(event->lateTime());
    ui->sbReserve->setValue(event->reserve());
}

