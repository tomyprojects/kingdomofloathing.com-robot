#-------------------------------------------------
#
# Project created by QtCreator 2012-02-24T19:14:22
#
#-------------------------------------------------

QT       += core gui network webkit

TARGET = qtnetwork
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qrobot.cpp \
    kolqrobot.cpp \
    step.cpp \
    output.cpp \
    domtraversal.cpp \
    myfirstrobot.cpp

HEADERS  += mainwindow.h \
    statuscode.h \
    qrobot.h \
    kolqrobot.h \
    step.h \
    output.h \
    httprequest.h \
    domtraversal.h \
    myfirstrobot.h

FORMS    += mainwindow.ui

CONFIG  += console
