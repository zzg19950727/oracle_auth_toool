#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T19:02:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = oracle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Form.cpp \
    new_form.cpp \
    list_search.cpp \
    change_view.cpp \
    change_info.cpp \
    deal_data.cpp \
    log.cpp \
    tes.cpp

HEADERS  += mainwindow.h \
    info.h \
    Form.h \
    list_search.h \
    log.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    myapp.rc
RC_FILE = myapp.rc
