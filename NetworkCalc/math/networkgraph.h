#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include <QObject>
#include <QMap>

#include "graph.h"
#include "random.h"
#include "function.h"


namespace Math {
namespace Planning {

class Work;
class Event;

class Resourse {
public:
    Resourse(QString name = "", double quantity = 0, double cost = 0)
        : m_name(name), m_quantity(quantity), m_cost(cost)
    {}

    QString name() const;
    void setName(const QString &name);

    double quantity() const;
    void setQuantity(double quantity);

    double cost() const;
    void setCost(double cost);

private:
    QString m_name;       // Наименование ресурса
    double  m_quantity;   // Доступное количество ресурса
    double  m_cost;       // стоимость единицы ресурсов
};

class Event : public GraphVertex<Event, Work> {
public:
    Event(QString name = "",
          int earlyTime = 0,
          int lateTime = 0,
          int reserve = 0,
          bool isCalculated = false);

    QString name() const;
    void setName(int name);

    double earlyTime() const;
    void setEarlyTime(double earlyTime);

    double lateTime() const;
    void setLateTime(double lateTime);

    double reserve() const;
    void setReserve(double reserve);

    bool isCalculated() const;
    void setIsCalculated(bool isCalculated);

private:
    QString m_name;
    double m_earlyTime;
    double m_lateTime;
    double m_reserve;

    bool m_isCalculated;
};

/**
 * @brief The IRandomFactory class Фабрика случайных величин
 */
class IRandomFactory {
public:
    virtual IRandom* create(Work* work) = 0;
};

class Work : public GraphEdge<Event, Work> {
public:
    Work(Event *event1, Event *event2, QString name = "", Resourse* resourse = 0, double resourseCount = 0,
         double timeMin = 0, double timeMax = 0, double timeAvg = 0,
         IRandomFactory* timeRandom = 0, IFunction* timeSpeed = 0,
         bool isCritical = false, bool isVirtual = false, double fullReserve = 0);

    QString name() const;
    void setName(int name);

    // ресурсные характеристики
    double cost();

    // временные характеристики
    double timeMin();
    double timeMax();
    double timeAvg();
    Random::Value* time();
    void setTimeRandom(IRandomFactory* factory);

    // сетевые характеристики
    double fullReserve() const;
    void setFullReserve(double value);

    bool isCritical() const;
    void setIsCritical(bool value);

    bool isVirtual() const;

private:
    QString         m_name;             // Наименование работы

    // ресурсные характеристики
    Resourse*       m_resourse;         // тип ресурса для данной работы
    double          m_resourseCount;    // количество выделенного ресурса

    // временные характеристики
    double          m_timeMin;          // минимальная оценка времени выполнения
    double          m_timeMax;          // максимальная оценка времени выполнения
    double          m_timeAvg;          // ожидаемая оценка времени выполнения
    Random::Value   m_time;             // время выполнения с учетом ресурсов
    IFunction*      m_timeSpeed;        // скорость выполения в зависимости от кол-ва ресурсов

    // сетевые характеристики
    double          m_fullReserve;      // полный резерв работы
    bool            m_isCritical;       // критическая работа
    bool            m_isVirtual;        // виртуальная работа
};

//typedef Graph<IEvent, IWork> NetworkGraph;

class NetworkGraph : public Graph<Event, Work> {
public:
    NetworkGraph();
    NetworkGraph(QVector<Event*> vertices, QVector<Work*> edges);

    Random::Value* time();
    Random::Value* cost();

    QVector<Work*> criticalPath();

private:
    Random::Value   m_time;
    Random::Value   m_cost;
};

namespace Random {

#define WORK_RANDOM(randomClass)  \
class randomClass : public IRandom { \
public: \
    randomClass(Work* work) \
        : m_work(work) \
    {} \
    IFunction *f() { \
        if (!m_work) return 0; \
        return Math::Random::randomClass(m_work->timeMin(), m_work->timeMax(), m_work->timeAvg()).f(); \
    } \
    IFunction *F() { \
        if (!m_work) return 0; \
        return Math::Random::randomClass(m_work->timeMin(), m_work->timeMax(), m_work->timeAvg()).F(); \
    } \
    void setMathExpected(double /*value*/) {} \
    double mathExpected() { \
        if (!m_work) return 0; \
        return Math::Random::randomClass(m_work->timeMin(), m_work->timeMax(), m_work->timeAvg()).mathExpected(); \
    } \
    void setDispersion(double /*value*/) {} \
    double dispersion() { \
        if (!m_work) return 0; \
        return Math::Random::randomClass(m_work->timeMin(), m_work->timeMax(), m_work->timeAvg()).dispersion(); \
    } \
    double random() { \
        if (!m_work) return 0; \
        return Math::Random::randomClass(m_work->timeMin(), m_work->timeMax(), m_work->timeAvg()).random(); \
    } \
protected: \
    Work *m_work; \
}; \
class randomClass##Factory : public IRandomFactory { \
public: \
    IRandom* create(Work* work) { return new randomClass(work); } \
};

WORK_RANDOM(Beta)
WORK_RANDOM(PertBeta)

} // namespace Randoms

} // namespace Planning
} // namespace Math

#endif // NETWORKGRAPH_H

