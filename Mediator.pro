#-------------------------------------------------
#
# Project created by QtCreator 2020-04-15T19:31:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mediator
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        widget.cpp \
    mediator.cpp \
    player.cpp

HEADERS  += widget.h \
    mediator.h \
    player.h
