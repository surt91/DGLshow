#-------------------------------------------------
#
# Project created by QtCreator 2014-03-15T14:17:30
#
#-------------------------------------------------

QT       += core gui
CONFIG += debug
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DGLshow
TEMPLATE = app


SOURCES += main.cpp\
    rungekuttasolver.cpp \
    lorenzattraktor.cpp \
    trajectory.cpp \
    gravitation.cpp

HEADERS  += rungekuttasolver.h \
    lorenzattraktor.h \
    trajectory.h \
    gravitation.h

FORMS    +=
