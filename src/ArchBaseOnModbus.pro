#-------------------------------------------------
#
# Project created by QtCreator 2018-09-06T17:05:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Arch
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

INCLUDEPATH += \
        3rdparty \
        model


SOURCES += \
        main.cpp \
    mainwindow.cpp \
    comm/AtcUtility.cpp \
    comm/comm.cpp \
    model/MbsRtu.cpp \
    model/modbusstack.cpp \
    model/modbustable.cpp \
    model/modbusunit.cpp \
    3rdparty/modbus/modbus.c \
    3rdparty/modbus/modbus-data.c \
    3rdparty/modbus/modbus-rtu.c \
    3rdparty/modbus/modbus-tcp.c \
    comm/log.cpp

HEADERS += \
        mainwindow.h \
    comm/AtcAfx.h \
    comm/AtcUtility.h \
    comm/call_once.h \
    comm/comm.h \
    comm/singleton.h \
    comm/worker.h \
    model/MbsRtu.h \
    model/modbusstack.h \
    model/modbustable.h \
    model/modbusunit.h \
    3rdparty/modbus/config.h \
    3rdparty/modbus/modbus.h \
    3rdparty/modbus/modbus-private.h \
    3rdparty/modbus/modbus-rtu.h \
    3rdparty/modbus/modbus-rtu-private.h \
    3rdparty/modbus/modbus-tcp.h \
    3rdparty/modbus/modbus-tcp-private.h \
    3rdparty/modbus/modbus-version.h \
    comm/log.h

FORMS += \
        mainwindow.ui

#libmodbus库（windows版）调用了库ws2_32.dll，需添加此库链接
LIBS += -lws2_32
