TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    person.cpp \
    PersonTest.cpp \
    personqueue.cpp \
    PersonQueueTest.cpp \
    elevator.cpp

HEADERS += \
    person.h \
    personqueue.h \
    elevator.h

