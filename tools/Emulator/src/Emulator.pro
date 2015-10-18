#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T16:35:03
#
#-------------------------------------------------

QT       += core gui opengl multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG(debug, debug|release) {
	TARGET = Emulatord
}

CONFIG(release, debug|release) {
	TARGET = Emulator
}

TEMPLATE = app
CONFIG += c++11

QMAKE_CFLAGS_WARN_ON -= -Wextra
QMAKE_CXXFLAGS_WARN_ON -= -Wextra

QMAKE_CFLAGS_WARN_ON += -Werror=return-type
QMAKE_CXXFLAGS_WARN_ON += -Werror=return-type

DESTDIR = $$PWD/../

INCLUDEPATH += ../../../luna2d \
		../../../luna2d/common/ \
		../../../luna2d/lua/ \
        ../../../luna2d/platform/ \
		../../../luna2d/platform/qt/ \
		../../../thirdparty/lua/ \
		../../../luna2d/utils/

CONFIG(debug, debug|release) {
	LIBS += -L$$PWD/../../../lib/qt/ -lluna2dd
	PRE_TARGETDEPS += $$PWD/../../../lib/qt/libluna2dd.a
}

CONFIG(release, debug|release) {
	LIBS += -L$$PWD/../../../lib/qt/ -lluna2d
	PRE_TARGETDEPS += $$PWD/../../../lib/qt/libluna2d.a
}

SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    watcherdialog.cpp \
	logdialog.cpp \
    logstorage.cpp

HEADERS  += mainwindow.h \
    settings.h \
    resolution.h \
    watcherdialog.h \
	logdialog.h \
    logstorage.h

FORMS    += mainwindow.ui \
    settings.ui \
    watcherdialog.ui \
	logdialog.ui

RESOURCES += \
    resources.qrc

QMAKE_MAC_SDK = macosx10.9
