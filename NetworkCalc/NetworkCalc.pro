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


SOURCES += main.cpp\
        mainwindow.cpp \
    math/networkgraph.cpp \
    math/matrix.cpp \
    math/graph.cpp \
    math/algorithms.cpp \
    tools/tools.cpp \
    view/eventwidget.cpp \
    view/networkgraphwidget.cpp \
    view/workwidget.cpp

HEADERS  += mainwindow.h \
    math/graph.h \
    math/networkgraph.h \
    math/matrix.h \
    math/algorithms.h \
    tools/tools.h \
    view/eventwidget.h \
    view/networkgraphwidget.h \
    view/workwidget.h

FORMS    += mainwindow.ui \
    view/eventwidget.ui \
    view/networkgraphwidget.ui \
    view/workwidget.ui

RESOURCES += \
    pics.qrc

DISTFILES += \
    pics/event.png
