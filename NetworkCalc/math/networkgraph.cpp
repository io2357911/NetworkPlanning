#include "networkgraph.h"

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


