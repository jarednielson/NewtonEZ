#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T22:30:30
#
#-------------------------------------------------

QT       += core gui
QT       += script

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
        newtonsceneobject.cpp \
        newtonformula.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/Shapes/b2ChainShape.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/Shapes/b2CircleShape.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2BroadPhase.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2CollideCircle.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2CollideEdge.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2CollidePolygon.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2Collision.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2Distance.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2DynamicTree.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Collision/b2TimeOfImpact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Common/b2BlockAllocator.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Common/b2Draw.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Common/b2Math.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Common/b2Settings.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Common/b2StackAllocator.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Common/b2Timer.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2Contact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2Joint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/b2Body.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/b2ContactManager.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/b2Fixture.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/b2Island.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/b2World.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Dynamics/b2WorldCallbacks.cpp \
    ../WindowsResources/Box2D-master/Box2D/Box2D/Rope/b2Rope.cpp

HEADERS += \
        mainwindow.h \
        Interfaces/buildertool.h \
        newtongraphicsview.h \
        newtondatamodels.h \
        newtonmodel.h \
        newtonformula.h

FORMS += \
        mainwindow.ui

unix|win32: LIBS += -L$$PWD/../libs/Box2D/ -lBox2D

INCLUDEPATH += $$PWD/../libs
INCLUDEPATH += ../../WindowsResources/Box2D-master/Box2D/Box2D
DEPENDPATH += $$PWD/../libs

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../libs/Box2D/Box2D.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../libs/Box2D/libBox2D.a

RESOURCES += \
    resources.qrc
