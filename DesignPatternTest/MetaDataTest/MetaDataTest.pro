QT += testlib xml core
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++latest
CONFIG -= app_bundle


INCLUDEPATH += $${PWD}/../../DesignPattern

TEMPLATE = app

SOURCES += \
    metadatatest.cpp
