#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include "graph.h"

class IWork;

class IEvent : public GraphVertex<IEvent, IWork> {
public:
    virtual int getID() const = 0;
    virtual void setID(int value) = 0;

    virtual int getEarlyTime() const = 0;
    virtual void setEarlyTime(int value) = 0;

    virtual int getLateTime() const = 0;
    virtual void setLateTime(int value) = 0;

    virtual int getReserve() const = 0;
    virtual void setReserve(int value) = 0;

    virtual bool getIsCalculated() const = 0;
    virtual void setIsCalculated(bool value) = 0;
};

class IWork : public GraphEdge<IEvent, IWork> {
public:
    IWork(IEvent *event1 = NULL, IEvent *event2 = NULL)
        : GraphEdge<IEvent, IWork>(event1, event2)
    {}

    virtual int getID() const = 0;
    virtual void setID(int value) = 0;

    virtual int getCost() const = 0;
    virtual void setCost(int value) = 0;

    virtual int getTime() const = 0;
    virtual void setTime(int value) = 0;

    virtual int getFullReserve() const = 0;
    virtual void setFullReserve(int value) = 0;

    virtual bool isCritical() const = 0;
    virtual void setIsCritical(bool value) = 0;

    virtual bool isVirtual() const = 0;
    virtual void setIsVirtual(bool value) = 0;
};

typedef Graph<IEvent, IWork> NetworkGraph;

class Event : public IEvent {
public:
    Event(int id = 0, int earlyTime = 0, int lateTime = 0, int reserve = 0, bool isCalculated = false)
        : id(id),
          earlyTime(earlyTime),
          lateTime(lateTime),
          reserve(reserve),
          isCalculated(isCalculated)
    {}

    int getID() const;
    void setID(int value);

    int getEarlyTime() const;
    void setEarlyTime(int value);

    int getLateTime() const;
    void setLateTime(int value);

    int getReserve() const;
    void setReserve(int value);

    bool getIsCalculated() const;
    void setIsCalculated(bool value);

private:
    int id;
    int earlyTime;
    int lateTime;
    int reserve;

    bool isCalculated;
};

class Work : public IWork {
public:
    Work(Event *event1 = NULL, Event *event2 = NULL, int id = 0, int cost = 0, int time = 0,
         bool isCritical = false, bool isVirtual = false, int fullReserve = 0)
        : IWork(event1, event2),
          id(id),
          cost(cost),
          time(time),
          fullReserve(fullReserve),
          m_isCritical(isCritical),
          m_isVirtual(isVirtual)
    {}

    int getID() const;
    void setID(int value);

    int getCost() const;
    void setCost(int value);

    int getTime() const;
    void setTime(int value);

    int getFullReserve() const { return fullReserve; }
    void setFullReserve(int value) { fullReserve = value; }

    bool isCritical() const;
    void setIsCritical(bool value);

    bool isVirtual() const { return m_isVirtual; }
    void setIsVirtual(bool value) { m_isVirtual = value; }

private:
    int id;
    int cost;
    int time;
    int fullReserve;
    bool m_isCritical;
    bool m_isVirtual;
};

#endif // NETWORKGRAPH_H
