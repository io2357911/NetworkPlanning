#ifndef NETWORKGRAPH_H
#define NETWORKGRAPH_H

#include <QObject>
#include <QMap>

#include "graph.h"
#include "random.h"
#include "function.h"


namespace Math {
namespace Planning {

using namespace Randoms;

class Work;
class Event;

class Resourse {
public:
    Resourse(QString name = "", double quantity = 0, double cost = 0);
    virtual ~Resourse() {}

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

    virtual void setName(QString value);
    virtual QString name() const;

    virtual void setEarlyTime(double value);
    virtual double earlyTime() const;

    virtual void setLateTime(double value);
    virtual double lateTime() const;

    virtual void setReserve(double value);
    virtual double reserve() const;

    virtual void setIsCalculated(bool value);
    virtual bool isCalculated() const;

private:
    QString m_name;

    double m_earlyTime;
    double m_lateTime;
    double m_reserve;

    bool m_isCalculated;
};

/**
 * @brief The TimeEstimation class Оценка времени с учетом выделенного ресурса
 */
class TimeEstimation : public Estimation {
public:
    TimeEstimation(Work *work);

    // Estimation interface
    double min();
    void setMin(double value);
    double max();
    void setMax(double value);
    double avg();
    void setAvg(double value);

private:
    Work *m_work;
};

/**
 * @brief The TimeEstimation class Оценка стоимости с учетом выделенного ресурса
 */
class CostEstimation : public Estimation {
public:
    CostEstimation(Work *work);

    // Estimation interface
    double min();
    void setMin(double value);
    double max();
    void setMax(double value);
    double avg();
    void setAvg(double value);

private:
    Work *m_work;
};


class Work : public GraphEdge<Event, Work> {
public:
    Work(Event *event1 = 0, Event *event2 = 0, QString name = "", Resourse* resourse = 0, double resourseCount = 0,
         double timeMin = 0, double timeMax = 0, double timeAvg = 0,
         IRandomFactory* timeRandom = 0, IFunction* timeSpeed = 0,
         bool isCritical = false, bool isVirtual = false, double fullReserve = 0);

    virtual QString name() const;
    virtual void setName(QString value);

    // ресурсные характеристики
    virtual void setResourse(Resourse* value);
    virtual Resourse *resourse();
    virtual void setResourseCount(double value);
    virtual double resourseCount();
    Random *cost();
    void setCost(IRandomFactory *factory);
    IEstimation *costEstimation();
    double costValue();

    // временные характеристики
    Random* time();
    void setTime(IRandomFactory *factory);
    IEstimation *timeEstimation();
    IEstimation *timeResourseEstimation();
    IFunction *timeSpeed();
    void setTimeSpeed(IFunction *speed);

    // сетевые характеристики
    virtual void setFullReserve(double value);
    virtual double fullReserve() const;
    virtual void setIsCritical(bool value);
    virtual bool isCritical() const;
    virtual void setIsVirtual(bool value);
    virtual bool isVirtual() const;

private:
    QString         m_name;             // Наименование работы

    // ресурсные характеристики
    Resourse       *m_resourse;         // тип ресурса для данной работы
    double          m_resourseCount;    // количество выделенного ресурса
    CostEstimation  m_costEstimation;   // оценка стоимости выполнения с учетом ресурсов
    Random         *m_cost;             // стоимость выполнения с учетом ресурсов


    // временные характеристики
    Estimation      m_timeEstimation;   // временные оценки времени
    TimeEstimation  m_timeResEstimation;// временные оценки времени с учетом выделенного ресурса
    Random         *m_time;             // время выполнения с учетом ресурсов
    IFunction      *m_timeSpeed;        // скорость выполения в зависимости от кол-ва ресурсов

    // сетевые характеристики
    double          m_fullReserve;      // полный резерв работы
    bool            m_isCritical;       // критическая работа
    bool            m_isVirtual;        // виртуальная работа
};

//typedef Graph<IEvent, IWork> NetworkGraph;
typedef QVector<double> ResourseDistribution;

class NetworkGraph : public Graph<Event, Work> {
public:
    NetworkGraph(QString name = "Текущий");
    NetworkGraph(QString name, QVector<Event*> vertices, QVector<Work*> edges);

    QString name() const;
    void setName(const QString &name);

    void setTime(Random* time);
    Random* time();

    void setCost(Random* cost);
    Random* cost();

    QVector<Work*> criticalPath();

    ResourseDistribution resourseDistribution();
    void setResourseDistribution(const ResourseDistribution &res);

private:
    QString m_name;
    Random* m_time;
    Random* m_cost;
};

} // namespace Planning
} // namespace Math

#endif // NETWORKGRAPH_H

