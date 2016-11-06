#-------------------------------------------------
#
# Project created by QtCreator 2015-04-19T19:35:34
#
#-------------------------------------------------

TEMPLATE = app
QT       += core gui widgets
TARGET = guibpgenc

CONFIG(debug, debug|release): {
    DESTDIR = $$PWD/../bin/debug
}
CONFIG(release, debug|release): {
    DESTDIR = $$PWD/../bin/release
}



SOURCES += main.cpp\
    selectphotospage.cpp \
    optionspage.cpp \
    convertpage.cpp \
    converterwizard.cpp \
    bpgconverter.cpp

HEADERS  += \
    selectphotospage.h \
    optionspage.h \
    convertpage.h \
    converterwizard.h \
    bpgconverter.h

RESOURCES += \
    resources.qrc
