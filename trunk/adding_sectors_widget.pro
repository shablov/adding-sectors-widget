#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T18:18:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bin/app
TEMPLATE = app

OBJECTS_DIR = obj
MOC_DIR = obj

SOURCES += main.cpp \
    tablesectorswidget.cpp \
    graphicssectorswidget.cpp \
    sectorsmodel.cpp \
    sectorswidget.cpp \
    tools.cpp \
    editsectorwidget.cpp

HEADERS  += \
    include/tablesectorswidget.h \
    include/graphicssectorswidget.h \
    include/sectorsmodel.h \
    include/sectorswidget.h \
    include/tools.h \
    include/editsectorwidget.h

INCLUDEPATH = include
