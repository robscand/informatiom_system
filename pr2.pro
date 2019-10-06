#-------------------------------------------------
#
# Project created by QtCreator 2017-08-30T18:30:55
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pr2
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    departform.cpp \
    employeeform.cpp \
    mainform.cpp \
    staffform.cpp

HEADERS  += mainwindow.h \
    database.h \
    departform.h \
    employeeform.h \
    mainform.h \
    staffform.h

RESOURCES += \
    res.qrc

TRANSLATIONS += lang_en.ts \
               lang_ru.ts

