#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T15:28:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    color.cpp \
    colorpoint.cpp \
    edge.cpp \
    global.cpp \
    glwidget.cpp \
    matrix.cpp \
    pixelbuffer.cpp \
    vector2.cpp \
    vector3.cpp \
    curve.cpp \
    bezier.cpp \
    spline.cpp

HEADERS  += mainwindow.h \
    color.h \
    colorpoint.h \
    edge.h \
    global.h \
    glwidget.h \
    matrix.h \
    pixelbuffer.h \
    vector2.h \
    vector3.h \
    curve.h \
    bezier.h \
    spline.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    Project4.pro.user
