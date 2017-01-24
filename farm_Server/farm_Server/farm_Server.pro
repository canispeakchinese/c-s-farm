#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T20:09:57
#
#-------------------------------------------------

QT       += core gui

QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = farm_Server
TEMPLATE = app


SOURCES += main.cpp \
    farm_server.cpp \
    thread.cpp

HEADERS  += \
    connect_mysql.h \
    farm_server.h \
    thread.h
