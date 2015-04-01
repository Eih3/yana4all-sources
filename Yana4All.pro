#-------------------------------------------------
#
# Project created by QtCreator 2014-09-26T11:36:08
#
#-------------------------------------------------

QT       += core gui network
QT += webkit webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = Yana4All
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    functions.cpp \
    file.cpp \
    json.cpp \
    configuration.cpp \
    mainicon.cpp \
    speak.cpp \
    server.cpp \
    listen.cpp \
    log.cpp

HEADERS  += mainwindow.h \
    functions.h \
    file.h \
    json.h \
    configuration.h \
    constant.h \
    mainicon.h \
    speak.h \
    server.h \
    listen.h \
    log.h \
    singleton.h

FORMS    += mainwindow.ui

RC_FILE = icon.rc



OTHER_FILES += \
    icon.rc

INCLUDEPATH +=  D:\libs\sphinxbase\include\ \
 D:\libs\pocketsphinx\include\

LIBS += -LD:\libs\pocketsphinx\bin\Release\ -lpocketsphinx -LD:\libs\sphinxbase\bin\Release\ -lsphinxbase


#INCLUDEPATH += "D:/Programme_installes/Microsoft Speech SDK 5.1/Include"
#LIBS += "D:/Programme_installes/Microsoft Speech SDK 5.1/Lib/i386/sapi.lib"
#LIBS += "C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/User32.lib"


