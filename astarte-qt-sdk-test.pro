QT += core
QT -= gui
TARGET = astarte-qt-sdk-test
TEMPLATE = app
CONFIG += console c++14

MOC_DIR     = build/moc
OBJECTS_DIR = build/obj
UI_DIR      = build/ui
RCC_DIR     = build/

INCLUDEPATH += include \
                /usr/include/AstarteDeviceSDKQt5

LIBS += -lmosquitto \
        -lmosquittopp \
        -lAstarteDeviceSDKQt5

HEADERS += include/sdkTester.hpp

SOURCES += src/main.cpp \
            src/sdkTester.cpp