#-------------------------------------------------
#
# Project created by QtCreator 2019-05-04T11:10:21
#
#-------------------------------------------------

QT += core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = katieLive
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

CONFIG += c++11

SOURCES += \
        apirequests.cpp \
        fullchart.cpp \
        main.cpp \
        mainwindow.cpp \
        pushbullet.cpp \
        trading.cpp

HEADERS += \
        fullchart.hpp \
        mainwindow.hpp \
        pushbullet.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix: LIBS += -L$$PWD/libraries/libcurl-7.56.0/lib/ -lcurl

INCLUDEPATH += $$PWD/libraries/libcurl-7.56.0/include
DEPENDPATH += $$PWD/libraries/libcurl-7.56.0/include

unix: PRE_TARGETDEPS += $$PWD/libraries/libcurl-7.56.0/lib/libcurl.a


unix: LIBS += -L$$PWD/libraries/jsoncpp-1.8.3/lib/ -ljsoncpp

INCLUDEPATH += $$PWD/libraries/jsoncpp-1.8.3/include
DEPENDPATH += $$PWD/libraries/jsoncpp-1.8.3/include

unix: PRE_TARGETDEPS += $$PWD/libraries/jsoncpp-1.8.3/lib/libjsoncpp.a

unix: LIBS += -lcrypto

unix: LIBS += -L$$PWD/libraries/libwebsockets-2.4.0/lib/ -lwebsockets

INCLUDEPATH += $$PWD/libraries/libwebsockets-2.4.0/include
DEPENDPATH += $$PWD/libraries/libwebsockets-2.4.0/include

unix: PRE_TARGETDEPS += $$PWD/libraries/libwebsockets-2.4.0/lib/libwebsockets.a

unix: LIBS += -L$$PWD/libraries/libbinacpp/lib/ -lbinacpp

INCLUDEPATH += $$PWD/libraries/libbinacpp/include
DEPENDPATH += $$PWD/libraries/libbinacpp/include
