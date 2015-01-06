#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T16:35:03
#
#-------------------------------------------------

QT       += core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Emulator
TEMPLATE = app

CONFIG += c++11

QMAKE_CFLAGS_WARN_ON -= -Wextra
QMAKE_CXXFLAGS_WARN_ON -= -Wextra

QMAKE_CFLAGS_WARN_ON += -Werror=return-type
QMAKE_CXXFLAGS_WARN_ON += -Werror=return-type

INCLUDEPATH += ../../../luna2d \
        ../../../luna2d/graphics/ \
        ../../../luna2d/lua/ \
        ../../../luna2d/modules/ \
        ../../../luna2d/platform/ \
        ../../../luna2d/platform/qt/ \
        ../../../luna2d/thirdparty/lua/ \
        ../../../luna2d/utils/

CONFIG(debug, debug|release) {
	LIBS += -L$$PWD/../../../lib/qt/debug/ -lluna2d
	PRE_TARGETDEPS += $$PWD/../../../lib/qt/debug/libluna2d.a
}

CONFIG(release, debug|release) {
	LIBS += -L$$PWD/../../../lib/qt/release/ -lluna2d
	PRE_TARGETDEPS += $$PWD/../../../lib/qt/release/libluna2d.a
}

LIBS += -lz

SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    watcherdialog.cpp

HEADERS  += mainwindow.h \
    settings.h \
    resolution.h \
    watcherdialog.h

FORMS    += mainwindow.ui \
    settings.ui \
    watcherdialog.ui

RESOURCES += \
    resources.qrc

QMAKE_MAC_SDK = macosx10.9
