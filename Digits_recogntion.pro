#-------------------------------------------------
#
# Project created by QtCreator 2018-11-23T11:06:33
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Digits_recogntion
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    network.cpp

HEADERS += \
        mainwindow.h \
    network.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += /home/alex/armadillo-9.200.6/include

INCLUDEPATH += /usr/lib/x86_64-linux-gnu/hdf5/serial/include

LIBS += -L/usr/lib/x86_64-linux-gnu/hdf5/serial  -lhdf5

LIBS += -llapack -lblas



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
