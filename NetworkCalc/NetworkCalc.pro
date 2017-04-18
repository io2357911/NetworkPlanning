#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:28:32
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++0x

TARGET = NetworkCalc
TEMPLATE = app


SOURCES += \
    main.cpp \
    math/networkgraph.cpp \
    math/algorithms.cpp \
    math/random.cpp \
    tools/debug.cpp \
    math/function.cpp \
    view/eventwidget.cpp \
    view/networkgraphwidget.cpp \
    view/workwidget.cpp \
    view/eventpropertiesdialog.cpp \
    view/workpropertiesdialog.cpp \
    mainwindow.cpp \
    view/resourseswidget.cpp \
    view/workresourse.cpp
#    view/workerswidget.cpp

HEADERS  += \
    math/graph.h \
    math/networkgraph.h \
    math/algorithms.h \
    math/interfaces.h \
    math/matrix.h \
    math/random.h \
    tools/ini.h \
    tools/debug.h \
    math/function.h \
    view/eventwidget.h \
    view/networkgraphwidget.h \
    view/workwidget.h \
    view/eventpropertiesdialog.h \
    view/workpropertiesdialog.h \
    mainwindow.h \
    view/resourseswidget.h \
    view/workresourse.h
#    view/workerswidget.h


FORMS    += \
    mainwindow.ui \
    view/eventwidget.ui \
    view/networkgraphwidget.ui \
    view/workwidget.ui \
    view/eventpropertiesdialog.ui \
    view/workpropertiesdialog.ui \
    view/resourseswidget.ui
#    view/workerswidget.ui

RESOURCES += \
    pics.qrc

DISTFILES += \
    pics/event.png
