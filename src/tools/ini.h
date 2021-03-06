#ifndef STORABLE_H
#define STORABLE_H

#include <QPair>
#include <QVector>
#include <QSettings>
#include <QSemaphore>
#include <QMap>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLineEdit>

#define DEFAULT_INI_FILE    "config.ini"
#define DEFAULT_GROUP       "allWidgets"

namespace INI
{
    typedef QMap<QString, QVariant> Settings;

    static QSemaphore smSetts(1);

    __attribute__((unused))
    static QList<QWidget*> findWidgetsWithProperty(QWidget *parent, QString propName) {
        QList<QWidget*> withProperty;
        QList<QWidget*> list = parent->findChildren<QWidget *>();
        foreach(QWidget *w, list){
            if (w->property(propName.toStdString().c_str()).isValid())
                withProperty.append(w);
        }
        return withProperty;
    }

    __attribute__((unused))
    static void addNameSuffixToChildren(QWidget *parent, QString suffix) {
        QList<QWidget*> list = parent->findChildren<QWidget *>();
        foreach(QWidget *w, list){
            if (w->objectName().isEmpty()) continue;
            w->setObjectName(w->objectName() + suffix);
        }
    }

    __attribute__((unused))
    static void store(Settings setts, QString file = DEFAULT_INI_FILE, QString group = DEFAULT_GROUP) {
        smSetts.acquire();

        QSettings settings(file, QSettings::IniFormat);

        settings.beginGroup(group);

        QStringList keys = setts.keys();
        for (int i = 0; i < keys.size(); i++) {
            settings.setValue(keys[i], setts[keys[i]]);
        }
        settings.endGroup();

        smSetts.release();
    }

    __attribute__((unused))
    static Settings restore(QStringList keys, QString file = DEFAULT_INI_FILE, QString group = DEFAULT_GROUP) {
        smSetts.acquire();

        QSettings settings(file, QSettings::IniFormat);

        settings.beginGroup(group);

        Settings setts;
        for (int i = 0; i < keys.size(); i++) {
            setts[keys[i]] = settings.value(keys[i]);
        }

        settings.endGroup();

        smSetts.release();
        return setts;
    }

    __attribute__((unused))
    static void store(QWidget *parent, QString file = DEFAULT_INI_FILE, QString group = DEFAULT_GROUP) {
        QList<QWidget *> widgetsToStore = findWidgetsWithProperty(parent, "Store");

        Settings setts;
        for (int i = 0; i < widgetsToStore.size(); i++) {
            QWidget *w = widgetsToStore[i];

            QString wName = w->objectName();
            const char *wType = w->metaObject()->className();

            if (strcmp(wType, "QLineEdit") == 0)
                setts[wName] = ((QLineEdit*)w)->text();

            else if (strcmp(wType, "QSpinBox") == 0)
                setts[wName] = ((QSpinBox*)w)->value();

            else if (strcmp(wType, "QComboBox") == 0)
                setts[wName] = ((QComboBox*)w)->currentIndex();

            else if (strcmp(wType, "QDoubleSpinBox") == 0)
                setts[wName] = ((QDoubleSpinBox*)w)->value();

            else if (strcmp(wType, "QDateTimeEdit") == 0)
                setts[wName] = ((QDateTimeEdit*)w)->dateTime();

            else if (strcmp(wType, "QCheckBox") == 0)
                setts[wName] = ((QCheckBox*)w)->isChecked();

            else if (strcmp(wType, "QRadioButton") == 0)
                setts[wName] = ((QRadioButton*)w)->isChecked();
        }

        INI::store(setts, file, group);
    }

    __attribute__((unused))
    static void restore(QWidget *parent, QString file = DEFAULT_INI_FILE, QString group = DEFAULT_GROUP) {
        QList<QWidget *> widgetsToStore = findWidgetsWithProperty(parent, "Store");
        QStringList keys;
        for (int i = 0; i < widgetsToStore.size(); i++) {
            keys.append(widgetsToStore[i]->objectName());
        }

        Settings setts = INI::restore(keys, file, group);

        for (int i = 0; i < widgetsToStore.size(); i++) {
            QWidget *w = widgetsToStore[i];

            QString wName = w->objectName();
            const char *wType = w->metaObject()->className();

            if (!setts[wName].isValid()) continue;

            if (strcmp(wType, "QLineEdit") == 0)
                ((QLineEdit*)w)->setText(setts[wName].toString());

            else if (strcmp(wType, "QSpinBox") == 0)
                ((QSpinBox*)w)->setValue(setts[wName].toInt());

            else if (strcmp(wType, "QComboBox") == 0)
                ((QComboBox*)w)->setCurrentIndex(setts[wName].toInt());

            else if (strcmp(wType, "QDoubleSpinBox") == 0)
                ((QDoubleSpinBox*)w)->setValue(setts[wName].toDouble());

            else if (strcmp(wType, "QDateTimeEdit") == 0)
                ((QDateTimeEdit*)w)->setDateTime(setts[wName].toDateTime());

            else if (strcmp(wType, "QCheckBox") == 0)
                ((QCheckBox*)w)->setChecked(setts[wName].toBool());

            else if (strcmp(wType, "QRadioButton") == 0)
                ((QRadioButton*)w)->setChecked(setts[wName].toBool());
        }
    }
}


#endif // STORABLE_H

