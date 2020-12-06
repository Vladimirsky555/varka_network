#-------------------------------------------------
#
# Project created by QtCreator 2020-09-13T12:36:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = var_network
TEMPLATE = app

win32: RC_ICONS = $$PWD/bin/images/app.ico


SOURCES += main.cpp\
        mainwindow.cpp \
    data.cpp \
    model.cpp \
    application.cpp \
    showitemwindow.cpp \
    regexphighlighter.cpp \
    worker.cpp \
    jsonworker.cpp

HEADERS  += mainwindow.h \
    data.h \
    model.h \
    application.h \
    showitemwindow.h \
    regexphighlighter.h \
    worker.h \
    jsonworker.h

FORMS    += mainwindow.ui \
    showitemwindow.ui

RESOURCES += \
    bin/images.qrc
