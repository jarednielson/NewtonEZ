#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T22:30:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NewtonCore
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        newtongraphicsview.cpp \
        newtonscene.cpp \
        newtonbody.cpp \
        newtonmodel.cpp \
        newtonsceneobject.cpp

HEADERS += \
        mainwindow.h \
        Interfaces/buildertool.h \
        newtongraphicsview.h \
        newtondatamodels.h \
        newtonmodel.h \
    exprtk.h

FORMS += \
        mainwindow.ui

unix|win32: LIBS += -L$$PWD/../libs/Box2D/ -lBox2D

INCLUDEPATH += $$PWD/../libs
DEPENDPATH += $$PWD/../libs

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libs/Box2D/Box2D.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../libs/Box2D/libBox2D.a
