#-------------------------------------------------
#
# Project created by QtCreator 2014-03-15T14:17:30
#
#-------------------------------------------------

QT       += core gui #opengl
#CONFIG += debug
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DGLshow
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE += -O3 -march=native -flto
QMAKE_LFLAGS_RELEASE += -O3 -march=native -flto

SOURCES += main.cpp\
    rungekuttasolver.cpp \
    lorenzattraktor.cpp \
    trajectory.cpp \
    gravitation.cpp \
    double_pendulum.cpp \
    mainwindow.cpp

HEADERS  += rungekuttasolver.h \
    trajectory.h \
    dgls.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui
