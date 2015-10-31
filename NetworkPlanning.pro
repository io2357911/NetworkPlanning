#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T13:51:52
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++0x

TARGET = NetworkPlanning
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    math/algoritms.cpp \
    math/structures.cpp

HEADERS  += mainwindow.h \
    math/algoritms.h \
    math/structures.h

FORMS    += mainwindow.ui
