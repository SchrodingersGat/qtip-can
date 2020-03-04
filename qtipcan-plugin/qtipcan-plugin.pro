#-------------------------------------------------
#
# Project created by QtCreator 2020-03-04T21:45:27
#
#-------------------------------------------------

QT       += network serialbus

QT       -= gui

TARGET = qtipcan-plugin
TEMPLATE = lib

DEFINES += QTIPCANPLUGIN_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../src \
               ../inc \


SOURCES += \
        ../src/qtipcan.cpp \
        qtipcanplugin.cpp

HEADERS += \
        ../inc/qtipcan.h \
        qtipcanplugin.h \
        qtipcan-plugin_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}
