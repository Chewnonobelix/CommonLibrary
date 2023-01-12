QT += testlib xml core
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++17
CONFIG -= app_bundle


INCLUDEPATH += $${PWD}/../../DesignPattern
win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../../DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../../DesignPattern/release -lDesignPattern
else:unix:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../../DesignPattern/ -lDesignPattern

unix:message("Proute")
TEMPLATE = app

SOURCES += \
    metadatatest.cpp
