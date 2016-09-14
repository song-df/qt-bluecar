#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T11:23:18
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QArduinoExample
TEMPLATE = app


SOURCES += main.cpp\
        qarduinoexamplewidget.cpp \
    QArduino.cpp

HEADERS  += qarduinoexamplewidget.h \
    QArduino.h

FORMS    += qarduinoexamplewidget.ui

CONFIG += mobility
MOBILITY = 

INCLUDEPATH += ../
