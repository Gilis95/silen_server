TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.c \
    src/hashmap.c \

HEADERS += \
    src/headers/hashmap.h \
    src/headers/person.h
