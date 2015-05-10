#-------------------------------------------------
#
# Project created by QtCreator 2015-01-31T11:34:37
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECS_Project2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    polygon.cpp \
    vector3.cpp \
    glwidget.cpp \
    vector2.cpp \
    edge.cpp \
    matrix.cpp \
    color.cpp \
    global.cpp

HEADERS  += mainwindow.h \
    polygon.h \
    vector3.h \
    glwidget.h \
    vector2.h \
    edge.h \
    matrix.h \
    color.h \
    global.h

FORMS    += mainwindow.ui
