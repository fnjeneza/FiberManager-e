#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T21:09:07
#
#-------------------------------------------------

QT       += core\
            sql

QT       += gui\
            widgets

TARGET = FiberManager-e
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    dbhandler.cpp \
    fmprocedure.cpp \
    address.cpp \
    fmgishandler.cpp \
    optique.cpp \
    mainwindow.cpp \
    ie_form.cpp \
    doe_gc.cpp \
    handler/database/mdbhandler.cpp \
    handler/database/psqldatabase.cpp \
    ui/config.cpp \
    handler/confighandler.cpp \
    dao/fmconfigdao.cpp \
    ui/mainfm.cpp

HEADERS += \
    dbhandler.h \
    fmprocedure.h \
    address.h \
    fmgishandler.h \
    optique.h \
    mainwindow.h \
    ie_form.h \
    doe_gc.h \
    handler/database/mdbhandler.h \
    handler/database/psqldatabase.h \
    ui/config.h \
    handler/confighandler.h \
    dao/fmconfigdao.h \
    ui/mainfm.h

FORMS += \
    mainwindow.ui \
    ie_form.ui \
    doe_gc.ui \
    ui/config.ui \
    ui/mainfm.ui
