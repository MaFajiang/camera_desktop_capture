#-------------------------------------------------
#
# Project created by QtCreator 2022-01-05T07:25:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = capture
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


SOURCES += \
        main.cpp \
        player.cpp \
    capture.cpp

HEADERS += \
        player.h \
    capture.h

FORMS += \
        player.ui

LIBS += -L$$PWD/ffmpeg-4.4.1/lib/ -lavcodec \
-L$$PWD/ffmpeg-4.4.1/lib/ -lavdevice \
-L$$PWD/ffmpeg-4.4.1/lib/ -lavfilter \
-L$$PWD/ffmpeg-4.4.1/lib/ -lavformat \
-L$$PWD/ffmpeg-4.4.1/lib/ -lavutil \
-L$$PWD/ffmpeg-4.4.1/lib/ -lpostproc \
-L$$PWD/ffmpeg-4.4.1/lib/ -lswresample \
-L$$PWD/ffmpeg-4.4.1/lib/ -lswscale

INCLUDEPATH += $$PWD/ffmpeg-4.4.1/include
DEPENDPATH += $$PWD/ffmpeg-4.4.1/include
