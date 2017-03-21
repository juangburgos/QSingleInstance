#-------------------------------------------------
#
# Project created by QtCreator 2017-03-21T09:48:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET    = Example
TEMPLATE  = app

CONFIG   -= flat

SOURCES  += main.cpp \
            dialog.cpp

HEADERS  += dialog.h

FORMS    += dialog.ui

include($$PWD/../src/qsingleinstance.pri)