#-------------------------------------------------
#
# Project created by QtCreator 2016-01-25T20:21:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qSnake
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11
CONFIG += qt c++11

SOURCES += main.cpp\
    gui/mainwindow.cpp \
    core/observer.cpp \
    core/subject.cpp \
    core/brain.cpp \
    core/snake.cpp \
    core/game.cpp \
    gui/qgameboard.cpp \
    gui/qtile.cpp \
    gui/qgameoverwindow.cpp \
    gui/qresetbutton.cpp

HEADERS  += gui/mainwindow.h \
    core/snake.h \
    core/snake_graph.h \
    core/observer.h \
    core/subject.h \
    core/game.h \
    core/brain.h \
    gui/qgameboard.h \
    gui/qtile.h \
    gui/qgameoverwindow.h \
    gui/qresetbutton.h

