#include "networkgraph.h"

Event::Event(int id, int earlyTime, int lateTime, int reserve, bool isCalculated)
    : id(id),
      earlyTime(earlyTime),
      lateTime(lateTime),
      reserve(reserve),
      isCalculated(isCalculated)
{}

int Event::getID() const
{
    return id;
}

void Event::setID(int value)
{
    id = value;
}
int Event::getEarlyTime() const
{
    return earlyTime;
}

void Event::setEarlyTime(int value)
{
    earlyTime = value;
}
int Event::getLateTime() const
{
    return lateTime;
}

void Event::setLateTime(int value)
{
    lateTime = value;
}
int Event::getReserve() const
{
    return reserve;
}

void Event::setReserve(int value)
{
    reserve = value;
}
bool Event::getIsCalculated() const
{
    return isCalculated;
}

void Event::setIsCalculated(bool value)
{
    isCalculated = value;
}



Work::Work(Event *event1, Event *event2, int id, int cost, int time, bool isCritical, bool isVirtual, int fullReserve)
    : IWork(event1, event2),
      id(id),
      cost(cost),
      time(time),
      fullReserve(fullReserve),
      m_isCritical(isCritical),
      m_isVirtual(isVirtual)
{}

int Work::getID() const
{
    return id;
}

void Work::setID(int value)
{
    id = value;
}
int Work::getCost() const
{
    return cost;
}

void Work::setCost(int value)
{
    cost = value;
}
int Work::getTime() const
{
    return time;
}

void Work::setTime(int value)
{
    time = value;
}

bool Work::isCritical() const { return m_isCritical; }

void Work::setIsCritical(bool value) { m_isCritical = value; }



IWork::IWork(IEvent *event1, IEvent *event2)
    : GraphEdge<IEvent, IWork>(event1, event2), worker(NULL)
{}

void IWork::setWorker(IWorker *worker) {
    this->worker = worker;
    if (worker) {
        setCost(worker->getCost(this));
        setTime(worker->getTime(this));
    }
}

IWorker *IWork::getWorker() { return worker; }

IWorkerCapability::IWorkerCapability(IWork *work, int cost, int time)
    : work(work), cost(cost), time(time)
{}

bool IWorkerCapability::operator==(const IWorkerCapability &other){
    return work->getID() == other.work->getID();
}

int IWorker::getCost(IWork *work) const {
    int ind = caps.indexOf(IWorkerCapability(work));
    return ind != -1 ? caps[ind].cost : 0;
}

int IWorker::getTime(IWork *work) const {
    int ind = caps.indexOf(IWorkerCapability(work));
    return ind != -1 ? caps[ind].time : 0;
}

void IWorker::setCost(IWork *work, int value) {
    IWorkerCapability cap(work, value, 0);
    int ind = caps.indexOf(cap);
    if (ind == -1) {
        caps.append(cap);

    } else {
        cap.time = caps[ind].time;
        caps.replace(ind, cap);
    }
}

void IWorker::setTime(IWork *work, int value) {
    IWorkerCapability cap(work, 0, value);
    int ind = caps.indexOf(cap);
    if (ind == -1) {
        caps.append(cap);

    } else {
        cap.cost = caps[ind].cost;
        caps.replace(ind, cap);
    }
}

void IWorker::addWork(IWork *work) {
    IWorkerCapability cap(work);
    int ind = caps.indexOf(cap);
    if (ind == -1) {
        caps.append(cap);
    }
}

void IWorker::deleteWork(IWork *work) {
    IWorkerCapability cap(work);
    int ind = caps.indexOf(cap);
    if (ind != -1) {
        caps.remove(ind);
    }
}
