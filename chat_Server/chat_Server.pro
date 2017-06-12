QT       += sql
LIBS += -L/usr/lib/ -lmuduo_base_cpp11
LIBS += -L/usr/lib/ -lmuduo_net_cpp11

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    farm_server.cpp

HEADERS += \
    connect_mysql.h \
    farm_server.h

