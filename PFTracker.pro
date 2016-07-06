TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


CONFIG += link_pkgconfig
PKGCONFIG += opencv


SOURCES += \
    main.cpp \
    particle.cpp \
    tracker.cpp \
    particle_filter.cpp \
    haar.cpp

HEADERS += \
    particle.h \
    tracker.h \
    particle_filter.h \
    haar.h
