#include "eventwidget.h"
#include "ui_eventwidget.h"
#include <QPainter>
#include <QMenu>

#include "../tools/tools.h"

EventWidget::EventWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventWidget) {

    ui->setupUi(this);

    aConnection = new QAction(tr("Связать"), this);
    //aConnection->setShortcuts(QKeySequence::New);
    aConnection->setStatusTip(tr("Связать два события"));
    connect(aConnection, SIGNAL(triggered()), this, SLOT(onConnection()));

    aProperties = new QAction(tr("Свойства"), this);
    aProperties->setShortcuts(QKeySequence::New);
    aProperties->setStatusTip(tr("Настраиваемые свойства события"));
    connect(aProperties, SIGNAL(triggered()), this, SLOT(onProperties()));

    aDelete = new QAction(tr("Удалить"), this);
    aDelete->setShortcuts(QKeySequence::Delete);
    aDelete->setStatusTip(tr("Удалить событие"));
    connect(aDelete, SIGNAL(triggered()), this, SLOT(onDelete()));
}

EventWidget::~EventWidget() {
    delete ui;
}

void EventWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/pics/event.png"));
}

void EventWidget::store(QString fileName)
{
    INI::Settings setts;
    setts["widget_pos_x"] = pos().x();
    setts["widget_pos_y"] = pos().y();
    setts["widget_width"] = width();
    setts["widget_height"] = height();

    INI::store(setts, fileName, QString("Event_%1").arg(name()));
}

void EventWidget::restore(QString fileName)
{
    QStringList keys = {
        "widget_pos_x",
        "widget_pos_y",
        "widget_width",
        "widget_height"
    };

    INI::Settings setts = INI::restore(keys, fileName, QString("Event_%1").arg(name()));

    if (setts["widget_width"].isValid() && setts["widget_height"].isValid())
        move(setts["widget_width"].toInt(), setts["widget_height"].toInt());

    if (setts["widget_pos_x"].isValid() && setts["widget_pos_y"].isValid())
        move(setts["widget_pos_x"].toInt(), setts["widget_pos_y"].toInt());
}

void EventWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(aProperties);
    menu.addAction(aConnection);
    menu.addAction(aDelete);
    menu.exec(event->globalPos());
}

void EventWidget::mousePressEvent(QMouseEvent *event) {
    offset = event->pos();

    if (event->buttons() == Qt::LeftButton) emit clicked(this);
}

void EventWidget::mouseMoveEvent(QMouseEvent *event) {
    if(event->buttons() & Qt::LeftButton) {
        move(mapToParent(event->pos() - offset));
        emit moved(this);
    }
}

void EventWidget::wheelEvent(QWheelEvent *event)
{
    if (size() == minimumSize() && event->delta() > 0) return;

    int step = 10;

    if (event->delta() > 0) step = -step;
    resize(width() + step, height() + step);

    move(pos() - QPoint(step/2, step/2));
}

QPoint EventWidget::center()
{
    return QPoint(pos().x() + width()/2, pos().y() + height()/2);
}

void EventWidget::setName(QString value) {
    Event::setName(value);
    onEventChanged();
}

void EventWidget::setEarlyTime(double value) {
    Event::setEarlyTime(value);
    onEventChanged();
}

void EventWidget::setLateTime(double value) {
    Event::setLateTime(value);
    onEventChanged();
}

void EventWidget::setReserve(double value) {
    Event::setReserve(value);
    onEventChanged();
}

void EventWidget::release() {
    close();
}

void EventWidget::onEventChanged() {
    ui->lID->setText(name());
    ui->lEarlyTime->setText(Tools::formatDouble(earlyTime()));
    ui->lLateTime->setText(Tools::formatDouble(lateTime()));
    ui->lReserve->setText(Tools::formatDouble(reserve()));
}
