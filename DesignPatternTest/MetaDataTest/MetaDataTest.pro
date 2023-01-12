QT += testlib xml core
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++20
CONFIG -= app_bundle


INCLUDEPATH += $${PWD}/../../DesignPattern
LIBS += -L$$OUT_PWD/../../DesignPattern/ -lDesignPattern

message($$OUT_PWD)

TEMPLATE = app

SOURCES += \
    metadatatest.cpp
