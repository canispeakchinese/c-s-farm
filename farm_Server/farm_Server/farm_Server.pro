#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T20:09:57
#
#-------------------------------------------------

QT       += sql

LIBS += -L/usr/lib/ -lmuduo_base_cpp11
LIBS += -L/usr/lib/ -lmuduo_net_cpp11

TARGET = farm_Server
TEMPLATE = app

CONFIG += console c++11

SOURCES += main.cpp \
    farm_server.cpp

HEADERS  += \
    connect_mysql.h \
    farm_server.h
