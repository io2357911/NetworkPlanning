#include "workwidget.h"
#include "ui_workwidget.h"
#include <QMenu>

#include "../tools/tools.h"

WorkWidget::WorkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkWidget),
    firstEvent(NULL),
    secondEvent(NULL) {

    ui->setupUi(this);

    aProperties = new QAction(tr("Свойства"), this);
    aProperties->setShortcuts(QKeySequence::New);
    aProperties->setStatusTip(tr("Настраиваемые свойства работы"));
    connect(aProperties, SIGNAL(triggered()), this, SLOT(onProperties()));

    aDelete = new QAction(tr("Удалить"), this);
    aDelete->setShortcuts(QKeySequence::Delete);
    aDelete->setStatusTip(tr("Удалить работу"));
    connect(aDelete, SIGNAL(triggered()), this, SLOT(onDelete()));

    onWorkChanged();
}

WorkWidget::~WorkWidget() {
    delete ui;
}

void WorkWidget::draw(QPainter *painter) {
    if (!(firstEvent && secondEvent)) return;

    QPoint c1 = firstEvent->center();
    QPoint c2 = secondEvent->center();

    painter->pen();

    QPoint loc((c1.x()+c2.x())/2, (c1.y()+c2.y())/2);

    move(loc - QPoint(width()/2, height()));

    QPen origPen = painter->pen();

    if (isCritical()) {
        if (isVirtual()) {
            painter->setPen(QPen(Qt::red, origPen.width()+1, Qt::DashLine));
        } else {
            painter->setPen(QPen(Qt::red, origPen.width()+1));
        }
    } else if (isVirtual()){
        painter->setPen(QPen(origPen.color(), origPen.width(), Qt::DashLine));
    }
    painter->drawLine(c1, c2);
    painter->setPen(origPen);
}

void WorkWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(aProperties);
    menu.addAction(aDelete);
    menu.exec(event->globalPos());
}

void WorkWidget::setName(QString value) {
    Work::setName(value);
    onWorkChanged();
}

void WorkWidget::setResourseCount(double value) {
    Work::setResourseCount(value);
    onWorkChanged();
}

void WorkWidget::setFullReserve(double value) {
    Work::setFullReserve(value);
    onWorkChanged();
}

void WorkWidget::setIsCritical(bool value) {
    Work::setIsCritical(value);
    onWorkChanged();
}

void WorkWidget::setIsVirtual(bool value) {
    Work::setIsVirtual(value);
    onWorkChanged();
}

void WorkWidget::release() {
    close();
}

void WorkWidget::onWorkChanged() {
    QString info;

    info += QString("%1").arg(name());

    info += QString("\n{%1,%2,%3}").arg(timeEstimation()->min()).arg(timeEstimation()->avg()).arg(timeEstimation()->max());
    if (!IS_ZERO(time()->value(), 0.0001)) info += QString(" %1").arg(Tools::formatDouble(time()->value()));

    if (resourse()) info += QString("\n%1(%2)").arg(resourse()->name()).arg(resourseCount());

    ui->lInfo->setText(info);
    ui->lInfo->setVisible(!isVirtual());

    adjustSize();
}

EventWidget *WorkWidget::getFirstEvent() const {
    return firstEvent;
}

void WorkWidget::setFirstEvent(EventWidget *widget) {
    firstEvent = widget;
    setVertex1(widget);
    connect(widget, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onDeleteEvent(EventWidget*)));
}

EventWidget *WorkWidget::getSecondEvent() const {
    return secondEvent;
}

void WorkWidget::setSecondEvent(EventWidget *widget) {
    secondEvent = widget;
    setVertex2(widget);
    connect(widget, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onDeleteEvent(EventWidget*)));
}

void WorkWidget::store(QString fileName) {
    INI::Settings setts;

    setts["firstEvent"] = getFirstEvent()->name();
    setts["secondEvent"] = getSecondEvent()->name();

    if (resourse())
        setts["resourseName"] = resourse()->name();

    setts["resourseCount"] = resourseCount();
    setts["timeMin"] = timeEstimation()->min();
    setts["timeAvg"] = timeEstimation()->avg();
    setts["timeMax"] = timeEstimation()->max();
    setts["virtual"] = isVirtual();

    INI::store(setts, fileName, QString("Work_%1").arg(name()));
}

void WorkWidget::restore(QString fileName) {
    QStringList keys = {
        "resourseCount",
        "timeMin",
        "timeAvg",
        "timeMax",
        "virtual"
    };

    INI::Settings setts = INI::restore(keys, fileName, QString("Work_%1").arg(name()));

    if (setts["resourseCount"].isValid())
        setResourseCount(setts["resourseCount"].toDouble());

    if (setts["timeMin"].isValid())
        timeEstimation()->setMin(setts["timeMin"].toDouble());

    if (setts["timeAvg"].isValid())
        timeEstimation()->setAvg(setts["timeAvg"].toDouble());

    if (setts["timeMax"].isValid())
        timeEstimation()->setMax(setts["timeMax"].toDouble());

    if (setts["virtual"].isValid())
        setIsVirtual(setts["virtual"].toBool());
}
