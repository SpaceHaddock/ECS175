#-------------------------------------------------
#
# Project created by QtCreator 2015-01-17T17:14:04
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECS_Project1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    vector2.cpp \
    polygon.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    vector2.h \
    polygon.h

FORMS    += mainwindow.ui
