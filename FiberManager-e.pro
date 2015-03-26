#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T21:09:07
#
#-------------------------------------------------

QT       += core\
            sql

QT       -= gui

TARGET = FiberManager-e
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    dbhandler.cpp \
    parameters.cpp \
    fmprocedure.cpp

HEADERS += \
    dbhandler.h \
    parameters.h \
    fmprocedure.h
