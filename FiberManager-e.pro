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
    fmgishandler.cpp \
    optique.cpp \
    handler/database/mdbhandler.cpp \
    handler/database/psqldatabase.cpp \
    handler/confighandler.cpp \
    ui/mainfm.cpp \
    ui/execsqldialog.cpp \
    ui/doegcdlg.cpp \
    dao/parameters.cpp \
    ui/configdialog.cpp

HEADERS += \
    dbhandler.h \
    fmprocedure.h \
    fmgishandler.h \
    optique.h \
    handler/database/mdbhandler.h \
    handler/database/psqldatabase.h \
    handler/confighandler.h \
    ui/mainfm.h \
    ui/execsqldialog.h \
    ui/doegcdlg.h \
    dao/parameters.h \
    ui/configdialog.h

FORMS += \
    ui/mainfm.ui \
    ui/execsqldialog.ui \
    ui/doegcdlg.ui \
    ui/configdialog.ui
