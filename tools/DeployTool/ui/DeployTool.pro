#-------------------------------------------------
#
# Project created by QtCreator 2016-01-09T12:04:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DeployTool
TEMPLATE = app
CONFIG += c++11

CONFIG(debug, debug|release) {
	TARGET = DeployToold
}

CONFIG(release, debug|release) {
	TARGET = DeployTool
}

QMAKE_CFLAGS_WARN_ON -= -Wextra
QMAKE_CXXFLAGS_WARN_ON -= -Wextra

QMAKE_CFLAGS_WARN_ON += -Werror=return-type
QMAKE_CXXFLAGS_WARN_ON += -Werror=return-type

DESTDIR = $$PWD/../

INCLUDEPATH += ../../../thirdparty/json11/

SOURCES += main.cpp\
		mainwindow.cpp \
		../../../thirdparty/json11/json11.cpp \
    utils.cpp \
    template.cpp

HEADERS  += mainwindow.h \
    utils.h \
    template.h

FORMS    += mainwindow.ui

QMAKE_MAC_SDK = macosx10.9
