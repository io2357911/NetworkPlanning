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

void EventWidget::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap(":/pics/event.png"));
}

void EventWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(aConnection);
    menu.addAction(aProperties);
    menu.addAction(aDelete);
    menu.exec(event->globalPos());
}

QPoint EventWidget::center()
{
    return QPoint(pos().x() + width()/2, pos().y() + height()/2);
}

EventWidget::~EventWidget() {
    delete ui;
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






