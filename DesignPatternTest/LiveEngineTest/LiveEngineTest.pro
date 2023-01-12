QT += testlib xml core quick qml
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase c++20
CONFIG -= app_bundle


INCLUDEPATH += $${PWD}/../../DesignPattern
LIBS += -L$$OUT_PWD/../../DesignPattern/ -lDesignPattern

win32:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../../DesignPattern/debug -lDesignPattern
else:win32:CONFIG(release, debug| release): LIBS += -L$$OUT_PWD/../../DesignPattern/release -lDesignPattern
else:unix:CONFIG(debug, debug| release): LIBS += -L$$OUT_PWD/../../DesignPattern/ -lDesignPattern

TEMPLATE = app

SOURCES += \
    tst_liveengine.cpp

DEFINES += ENABLE_HOTRELOADING

RESOURCES += \
    liveenginetest.qrc

DEFINES += FOLDER=\\\"$$PWD/Folder1/|$$PWD/Folder2/\\\"

win32:message("Bite" + $$PWD)
else:unix:message("Chatte " + $$OUT_PWD)
