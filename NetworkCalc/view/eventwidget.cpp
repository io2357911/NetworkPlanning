#include "eventwidget.h"
#include "ui_eventwidget.h"
#include <QPainter>
#include <QMenu>

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

    INI::store(setts, fileName, QString("Event_%1").arg(getID()));
}

void EventWidget::restore(QString fileName)
{
    QStringList keys = {
        "widget_pos_x",
        "widget_pos_y",
        "widget_width",
        "widget_height"
    };

    INI::Settings setts = INI::restore(keys, fileName, QString("Event_%1").arg(getID()));

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

int EventWidget::getID() const
{
    return ui->lID->text().toInt();
}

void EventWidget::setID(int value)
{
    ui->lID->setText(QString::number(value));
}

int EventWidget::getEarlyTime() const
{
    return ui->lEarlyTime->text().toInt();
}

void EventWidget::setEarlyTime(int value)
{
    ui->lEarlyTime->setText(QString::number(value));
}

int EventWidget::getLateTime() const
{
    return ui->lLateTime->text().toInt();
}

void EventWidget::setLateTime(int value)
{
    ui->lLateTime->setText(QString::number(value));
}

int EventWidget::getReserve() const
{
    return ui->lReserve->text().toInt();
}

void EventWidget::setReserve(int value)
{
    ui->lReserve->setText(QString::number(value));
}

bool EventWidget::getIsCalculated() const
{
    return isCalculated;
}

void EventWidget::setIsCalculated(bool value)
{
    isCalculated = value;
}






