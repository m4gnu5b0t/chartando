#-------------------------------------------------
#
# Project created by QtCreator 2022-01-04T14:04:04
#
#-------------------------------------------------

# QT       += core gui

# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += core
QT += widgets
QT += xml
QT += gui
QT += charts

TARGET = chartando
TEMPLATE = app

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
# DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    ch_sale.cpp \
    ch_saleview.cpp \
    ch_saleparser.cpp \
    ch_barchart.cpp \
    ch_promosale.cpp \
    ch_linechart.cpp \
    ch_piechart.cpp \
    ch_chart.cpp

HEADERS += \
    ch_chart.h \
    ch_sale.h \
    ch_saleview.h \
    ch_saleparser.h \
    ch_barchart.h \
    ch_promosale.h \
    ch_piechart.h \
    ch_linechart.h
