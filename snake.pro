TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    game.cpp \
    cell.cpp \
    input.cpp

LIBS +=  -lncurses

HEADERS += \
    game.h \
    cell.h \
    input.h
