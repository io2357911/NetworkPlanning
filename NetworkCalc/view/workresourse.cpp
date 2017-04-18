#include "workresourse.h"

WorkResourse::WorkResourse(QString name, double quantity, double cost)
    : Resourse(name, quantity, cost)
{}

void WorkResourse::store(QString fileName) {
    INI::Settings setts;

    setts["name"] = name();
    setts["quantity"] = quantity();
    setts["cost"] = cost();

    INI::store(setts, fileName, QString("Resourse_%1").arg(name()));
}

void WorkResourse::restore(QString fileName) {
    QStringList keys = {
        "name",
        "quantity",
        "cost",
    };

    INI::Settings setts = INI::restore(keys, fileName, QString("Resourse_%1").arg(name()));

    if (setts["name"].isValid())
        setName(setts["name"].toString());

    if (setts["quantity"].isValid())
        setQuantity(setts["quantity"].toDouble());

    if (setts["cost"].isValid())
        setCost(setts["cost"].toDouble());
}
