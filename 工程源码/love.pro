#-------------------------------------------------
#
# Project created by QtCreator 2018-02-09T20:16:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql network

TARGET = love
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
    logindlg.cpp \
    common.cpp \
    cselftitledlg.cpp \
    titlebar.cpp \
    xlwidget.cpp \
    maindlg.cpp \
    log.cpp \
    note.cpp \
    noteadddlg.cpp \
    notemodifydlg.cpp \
    user.cpp

HEADERS += \
    logindlg.h \
    common.h \
    cselftitledlg.h \
    titlebar.h \
    xlwidget.h \
    maindlg.h \
    log.h \
    note.h \
    noteadddlg.h \
    notemodifydlg.h \
    user.h

FORMS += \
    logindlg.ui \
    maindlg.ui \
    log.ui \
    note.ui \
    noteadddlg.ui \
    notemodifydlg.ui \
    user.ui

RC_FILE += ico/res.rc
