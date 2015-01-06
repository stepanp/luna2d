#-------------------------------------------------
#
# Project created by QtCreator 2014-08-15T17:57:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CFLAGS_WARN_ON -= -Wextra
QMAKE_CXXFLAGS_WARN_ON -= -Wextra

QMAKE_CFLAGS_WARN_ON += -Werror=return-type
QMAKE_CXXFLAGS_WARN_ON += -Werror=return-type

TARGET = Pipeline
TEMPLATE = app

INCLUDEPATH += ../../../luna2d/ \
    ../../../luna2d/utils/ \
    ../../../luna2d/thirdparty/lua/ \
    ../../../luna2d/lua/ \
    ../../../luna2d/platform/ \
    ../../../luna2d/platform/qt \
    ../../../luna2d/thirdparty/RectangleBinPack/ \
    ../../../luna2d/thirdparty/

SOURCES += main.cpp\
        ui/mainwindow.cpp \
    project/project.cpp \
    project/task.cpp \
    project/filenode.cpp \
    project/foldernode.cpp \
    project/tasknode.cpp \
    ui/projecttreedata.cpp \
    pipeline/pipeline.cpp \
    pipeline/resizer.cpp \
    utils/mathutils.cpp \
    pipeline/atlasbuilder.cpp \
	ui/settings.cpp \
    ../../../luna2d/thirdparty/RectangleBinPack/MaxRectsBinPack.cpp \
    ../../../luna2d/thirdparty/RectangleBinPack/Rect.cpp \

HEADERS  += ui/mainwindow.h \
    project/project.h \
    project/task.h \
    project/filenode.h \
    project/foldernode.h \
    project/tasknode.h \
    ui/projecttreedata.h \
    pipeline/pipeline.h \
    pipeline/resizer.h \
    utils/mathutils.h \
    pipeline/atlasbuilder.h \
    ui/settings.h \
    ../../../luna2d/thirdparty/RectangleBinPack/MaxRectsBinPack.h \
    ../../../luna2d/thirdparty/RectangleBinPack/Rect.h \

FORMS    += mainwindow.ui

QMAKE_MAC_SDK = macosx10.9
