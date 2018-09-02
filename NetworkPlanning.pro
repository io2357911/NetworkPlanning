#-------------------------------------------------
#
# Project created by QtCreator 2015-12-17T15:28:32
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
QMAKE_CXXFLAGS += -std=c++0x

TARGET = NetworkCalc
TEMPLATE = app

INCLUDEPATH += src

SOURCES += \
    src/main.cpp \
    src/math/networkgraph.cpp \
    src/math/algorithms.cpp \
    src/math/random.cpp \
    src/tools/debug.cpp \
    src/math/function.cpp \
    src/view/eventwidget.cpp \
    src/view/networkgraphwidget.cpp \
    src/view/workwidget.cpp \
    src/view/eventpropertiesdialog.cpp \
    src/view/workpropertiesdialog.cpp \
    src/mainwindow.cpp \
    src/view/resourseswidget.cpp \
    src/view/qcustomplot.cpp \
    src/tools/tools.cpp \
    src/view/qmycustomplot.cpp \
    src/project.cpp

HEADERS  += \
    src/math/graph.h \
    src/math/networkgraph.h \
    src/math/algorithms.h \
    src/math/matrix.h \
    src/math/random.h \
    src/tools/ini.h \
    src/tools/debug.h \
    src/math/function.h \
    src/view/eventwidget.h \
    src/view/networkgraphwidget.h \
    src/view/workwidget.h \
    src/view/eventpropertiesdialog.h \
    src/view/workpropertiesdialog.h \
    src/view/qcustomplot.h \
    src/mainwindow.h \
    src/view/resourseswidget.h \
    src/tools/tools.h \
    src/view/qmycustomplot.h \
    src/project.h \
    src/math/interfaces.h

FORMS    += \
    src/mainwindow.ui \
    src/view/eventwidget.ui \
    src/view/networkgraphwidget.ui \
    src/view/workwidget.ui \
    src/view/eventpropertiesdialog.ui \
    src/view/workpropertiesdialog.ui \
    src/view/resourseswidget.ui

RESOURCES += \
    pics.qrc

DISTFILES += \
    pics/event.png
