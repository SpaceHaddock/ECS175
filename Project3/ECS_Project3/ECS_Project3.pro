#-------------------------------------------------
#
# Project created by QtCreator 2015-02-16T12:57:38
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ECS_Project3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    color.cpp \
    edge.cpp \
    global.cpp \
    glwidget.cpp \
    matrix.cpp \
    polygon.cpp \
    vector2.cpp \
    vector3.cpp \
    triangle.cpp \
    colorpoint.cpp \
    pixelbuffer.cpp

HEADERS  += mainwindow.h \
    color.h \
    edge.h \
    global.h \
    glwidget.h \
    matrix.h \
    polygon.h \
    vector2.h \
    vector3.h \
    triangle.h \
    colorpoint.h \
    pixelbuffer.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    ECS_Project3.pro.user
