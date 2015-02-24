#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T15:20:14
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = luna2d
TEMPLATE = lib
CONFIG += staticlib c++11

QMAKE_CFLAGS_WARN_ON -= -Wextra
QMAKE_CXXFLAGS_WARN_ON -= -Wextra

QMAKE_CFLAGS_WARN_ON += -Werror=return-type
QMAKE_CXXFLAGS_WARN_ON += -Werror=return-type

CONFIG(debug, debug|release) {
	DESTDIR = $$PWD/../../../lib/qt/debug/
}

CONFIG(release, debug|release) {
	DESTDIR = $$PWD/../../../lib/qt/release/
}

INCLUDEPATH += ../../../luna2d \
	../../../luna2d/graphics/ \
	../../../luna2d/lua/ \
	../../../luna2d/physics/ \
	../../../luna2d/modules/ \
	../../../luna2d/platform/ \
	../../../luna2d/platform/qt/ \
	../../../luna2d/thirdparty/ \
	../../../luna2d/thirdparty/lua/ \
	../../../luna2d/thirdparty/libpng/ \
	../../../luna2d/thirdparty/json11/ \
	../../../luna2d/thirdparty/Box2D/ \
	../../../luna2d/utils/

SOURCES += \
    ../../../luna2d/graphics/lunagraphics.cpp \
    ../../../luna2d/graphics/lunaimage.cpp \
    ../../../luna2d/graphics/lunapngformat.cpp \
    ../../../luna2d/graphics/lunarenderer.cpp \
    ../../../luna2d/graphics/lunashader.cpp \
    ../../../luna2d/graphics/lunatexture.cpp \
    ../../../luna2d/lua/luafunction.cpp \
    ../../../luna2d/lua/luaref.cpp \
    ../../../luna2d/lua/luascript.cpp \
    ../../../luna2d/lua/luatable.cpp \
    ../../../luna2d/modules/lunalogmodule.cpp \
    ../../../luna2d/platform/qt/lunaqtwidget.cpp \
    ../../../luna2d/utils/lunacolor.cpp \
    ../../../luna2d/lunaassets.cpp \
    ../../../luna2d/lunaengine.cpp \
    ../../../luna2d/lunascenes.cpp \
	../../../luna2d/thirdparty/libpng/png.c \
	../../../luna2d/thirdparty/libpng/pngerror.c \
	../../../luna2d/thirdparty/libpng/pngget.c \
	../../../luna2d/thirdparty/libpng/pngmem.c \
	../../../luna2d/thirdparty/libpng/pngpread.c \
	../../../luna2d/thirdparty/libpng/pngread.c \
	../../../luna2d/thirdparty/libpng/pngrio.c \
	../../../luna2d/thirdparty/libpng/pngrtran.c \
	../../../luna2d/thirdparty/libpng/pngrutil.c \
	../../../luna2d/thirdparty/libpng/pngset.c \
	../../../luna2d/thirdparty/libpng/pngtrans.c \
	../../../luna2d/thirdparty/libpng/pngwio.c \
	../../../luna2d/thirdparty/libpng/pngwrite.c \
	../../../luna2d/thirdparty/libpng/pngwtran.c \
	../../../luna2d/thirdparty/libpng/pngwutil.c \
    ../../../luna2d/thirdparty/lua/lapi.c \
    ../../../luna2d/thirdparty/lua/lauxlib.c \
    ../../../luna2d/thirdparty/lua/lbaselib.c \
    ../../../luna2d/thirdparty/lua/lbitlib.c \
    ../../../luna2d/thirdparty/lua/lcode.c \
    ../../../luna2d/thirdparty/lua/lcorolib.c \
    ../../../luna2d/thirdparty/lua/lctype.c \
    ../../../luna2d/thirdparty/lua/ldblib.c \
    ../../../luna2d/thirdparty/lua/ldebug.c \
    ../../../luna2d/thirdparty/lua/ldo.c \
    ../../../luna2d/thirdparty/lua/ldump.c \
    ../../../luna2d/thirdparty/lua/lfunc.c \
    ../../../luna2d/thirdparty/lua/lgc.c \
    ../../../luna2d/thirdparty/lua/linit.c \
    ../../../luna2d/thirdparty/lua/liolib.c \
    ../../../luna2d/thirdparty/lua/llex.c \
    ../../../luna2d/thirdparty/lua/lmathlib.c \
    ../../../luna2d/thirdparty/lua/lmem.c \
    ../../../luna2d/thirdparty/lua/loadlib.c \
    ../../../luna2d/thirdparty/lua/lobject.c \
    ../../../luna2d/thirdparty/lua/lopcodes.c \
    ../../../luna2d/thirdparty/lua/loslib.c \
    ../../../luna2d/thirdparty/lua/lparser.c \
    ../../../luna2d/thirdparty/lua/lstate.c \
    ../../../luna2d/thirdparty/lua/lstring.c \
    ../../../luna2d/thirdparty/lua/lstrlib.c \
    ../../../luna2d/thirdparty/lua/ltable.c \
    ../../../luna2d/thirdparty/lua/ltablib.c \
    ../../../luna2d/thirdparty/lua/ltm.c \
    ../../../luna2d/thirdparty/lua/lundump.c \
    ../../../luna2d/thirdparty/lua/lvm.c \
    ../../../luna2d/thirdparty/lua/lzio.c \
    ../../../luna2d/lunaconfig.cpp \
    ../../../luna2d/lunasizes.cpp \
    ../../../luna2d/graphics/lunatextureregion.cpp \
    ../../../luna2d/graphics/lunatextureatlas.cpp \
    ../../../luna2d/graphics/lunasprite.cpp \
    ../../../luna2d/modules/lunaintersect.cpp \
    ../../../luna2d/thirdparty/json11/json11.cpp \
    ../../../luna2d/modules/lunadebug.cpp \
    ../../../luna2d/utils/lunawatcher.cpp \
    ../../../luna2d/lua/luadynamictype.cpp \
    ../../../luna2d/graphics/lunamesh.cpp \
    ../../../luna2d/modules/lunaplatformmodule.cpp \
    ../../../luna2d/modules/lunamathmodule.cpp \
    ../../../luna2d/utils/lunamathutils.cpp \
    ../../../luna2d/lua/luaobject.cpp \
    ../../../luna2d/physics/lunaphysicsmodule.cpp \
    ../../../luna2d/physics/lunaphysicsworld.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2ChainShape.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2CircleShape.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2BroadPhase.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2CollideCircle.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2CollideEdge.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2CollidePolygon.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2Collision.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2Distance.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2DynamicTree.cpp \
    ../../../luna2d/thirdparty/Box2D/Collision/b2TimeOfImpact.cpp \
    ../../../luna2d/thirdparty/Box2D/Common/b2BlockAllocator.cpp \
    ../../../luna2d/thirdparty/Box2D/Common/b2Draw.cpp \
    ../../../luna2d/thirdparty/Box2D/Common/b2Math.cpp \
    ../../../luna2d/thirdparty/Box2D/Common/b2Settings.cpp \
    ../../../luna2d/thirdparty/Box2D/Common/b2StackAllocator.cpp \
    ../../../luna2d/thirdparty/Box2D/Common/b2Timer.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2Contact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2Joint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2Body.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2ContactManager.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2Fixture.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2Island.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2World.cpp \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2WorldCallbacks.cpp \
    ../../../luna2d/thirdparty/Box2D/Rope/b2Rope.cpp \
    ../../../luna2d/physics/lunaphysicsbody.cpp \
    ../../../luna2d/physics/lunaphysicsfixture.cpp \
    ../../../luna2d/physics/lunaphysicsdebugrenderer.cpp \
    ../../../luna2d/utils/lunaglhelpers.cpp \
    ../../../luna2d/physics/shapes/lunaphysicscircleshape.cpp \
    ../../../luna2d/physics/shapes/lunaphysicsedgeshape.cpp \
    ../../../luna2d/physics/shapes/lunaphysicsrectshape.cpp \
    ../../../luna2d/physics/shapes/lunaphysicsshape.cpp \
    ../../../luna2d/physics/joints/lunaphysicsjoint.cpp \
    ../../../luna2d/physics/joints/lunaphysicsdistancejoint.cpp \
    ../../../luna2d/physics/lunaphysicsutils.cpp \
    ../../../luna2d/modules/lunasplinesmodule.cpp \
    ../../../luna2d/utils/lunavector2.cpp \
    ../../../luna2d/platform/lunafiles.cpp \
    ../../../luna2d/platform/qt/lunaqtfiles.cpp \
    ../../../luna2d/platform/lunalog.cpp \
    ../../../luna2d/platform/qt/lunaqtlog.cpp \
    ../../../luna2d/platform/qt/lunaqtgl.cpp \
    ../../../luna2d/platform/lunaplatformutils.cpp \
    ../../../luna2d/platform/qt/lunaqtutils.cpp \
    ../../../luna2d/graphics/lunacurverenderer.cpp

HEADERS += \
    ../../../luna2d/graphics/shaders/default.frag.h \
    ../../../luna2d/graphics/shaders/default.vert.h \
	../../../luna2d/graphics/shaders/primitives.frag.h \
    ../../../luna2d/graphics/lunagraphics.h \
    ../../../luna2d/graphics/lunaimage.h \
    ../../../luna2d/graphics/lunaimageformat.h \
    ../../../luna2d/graphics/lunapngformat.h \
    ../../../luna2d/graphics/lunarenderer.h \
    ../../../luna2d/graphics/lunashader.h \
    ../../../luna2d/graphics/lunatexture.h \
    ../../../luna2d/lua/scripts/logtable.lua.h \
    ../../../luna2d/lua/scripts/oopsupport.lua.h \
    ../../../luna2d/lua/scripts/userdatapairs.lua.h \
    ../../../luna2d/lua/luaclass.h \
    ../../../luna2d/lua/luafunction.h \
    ../../../luna2d/lua/luaproxy.h \
    ../../../luna2d/lua/luaref.h \
    ../../../luna2d/lua/luascript.h \
    ../../../luna2d/lua/luastack.h \
    ../../../luna2d/lua/luatable.h \
    ../../../luna2d/lua/lunalua.h \
    ../../../luna2d/modules/lunalogmodule.h \
    ../../../luna2d/modules/lunamodule.h \
    ../../../luna2d/modules/lunamoduleslist.h \
    ../../../luna2d/platform/qt/lunaqtwidget.h \
    ../../../luna2d/platform/lunafiles.h \
    ../../../luna2d/platform/lunagl.h \
    ../../../luna2d/platform/lunalog.h \
    ../../../luna2d/platform/lunaplatform.h \
    ../../../luna2d/platform/lunaplatformutils.h \
	../../../luna2d/thirdparty/libpng/config.h \
	../../../luna2d/thirdparty/libpng/png.h \
	../../../luna2d/thirdparty/libpng/pngconf.h \
	../../../luna2d/thirdparty/libpng/pngpriv.h \
    ../../../luna2d/thirdparty/lua/lapi.h \
    ../../../luna2d/thirdparty/lua/lauxlib.h \
    ../../../luna2d/thirdparty/lua/lcode.h \
    ../../../luna2d/thirdparty/lua/lctype.h \
    ../../../luna2d/thirdparty/lua/ldebug.h \
    ../../../luna2d/thirdparty/lua/ldo.h \
    ../../../luna2d/thirdparty/lua/lfunc.h \
    ../../../luna2d/thirdparty/lua/lgc.h \
    ../../../luna2d/thirdparty/lua/llex.h \
    ../../../luna2d/thirdparty/lua/llimits.h \
    ../../../luna2d/thirdparty/lua/lmem.h \
    ../../../luna2d/thirdparty/lua/lobject.h \
    ../../../luna2d/thirdparty/lua/lopcodes.h \
    ../../../luna2d/thirdparty/lua/lparser.h \
    ../../../luna2d/thirdparty/lua/lstate.h \
    ../../../luna2d/thirdparty/lua/lstring.h \
    ../../../luna2d/thirdparty/lua/ltable.h \
    ../../../luna2d/thirdparty/lua/ltm.h \
    ../../../luna2d/thirdparty/lua/lua.h \
    ../../../luna2d/thirdparty/lua/lua.hpp \
    ../../../luna2d/thirdparty/lua/luaconf.h \
    ../../../luna2d/thirdparty/lua/lualib.h \
    ../../../luna2d/thirdparty/lua/lundump.h \
    ../../../luna2d/thirdparty/lua/lvm.h \
    ../../../luna2d/thirdparty/lua/lzio.h \
    ../../../luna2d/utils/lunacolor.h \
    ../../../luna2d/utils/lunaindexlist.h \
    ../../../luna2d/lunaassets.h \
    ../../../luna2d/lunaengine.h \
    ../../../luna2d/lunascenes.h \
    ../../../luna2d/lunaconstants.h \
    ../../../luna2d/lunaconfig.h \
    ../../../luna2d/lunasizes.h \
    ../../../luna2d/graphics/lunatextureregion.h \
    ../../../luna2d/graphics/lunatextureatlas.h \
    ../../../luna2d/graphics/lunasprite.h \
    ../../../luna2d/modules/lunaintersect.h \
    ../../../luna2d/thirdparty/json11/json11.hpp \
    ../../../luna2d/modules/lunadebug.h \
    ../../../luna2d/utils/lunawatcher.h \
    ../../../luna2d/lua/luadynamictype.h \
    ../../../luna2d/graphics/lunamesh.h \
    ../../../luna2d/modules/lunaplatformmodule.h \
    ../../../luna2d/modules/lunamathmodule.h \
    ../../../luna2d/utils/lunamathutils.h \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2ChainShape.h \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2CircleShape.h \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2EdgeShape.h \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2PolygonShape.h \
    ../../../luna2d/thirdparty/Box2D/Collision/Shapes/b2Shape.h \
    ../../../luna2d/thirdparty/Box2D/Collision/b2BroadPhase.h \
    ../../../luna2d/thirdparty/Box2D/Collision/b2Collision.h \
    ../../../luna2d/thirdparty/Box2D/Collision/b2Distance.h \
    ../../../luna2d/thirdparty/Box2D/Collision/b2DynamicTree.h \
    ../../../luna2d/thirdparty/Box2D/Collision/b2TimeOfImpact.h \
    ../../../luna2d/thirdparty/Box2D/Common/b2BlockAllocator.h \
    ../../../luna2d/thirdparty/Box2D/Common/b2Draw.h \
    ../../../luna2d/thirdparty/Box2D/Common/b2GrowableStack.h \
    ../../../luna2d/thirdparty/Box2D/Common/b2Math.h \
    ../../../luna2d/thirdparty/Box2D/Common/b2Settings.h \
    ../../../luna2d/thirdparty/Box2D/Common/b2StackAllocator.h \
    ../../../luna2d/thirdparty/Box2D/Common/b2Timer.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2CircleContact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2Contact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Contacts/b2PolygonContact.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2FrictionJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2GearJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2Joint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2MotorJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2MouseJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2RopeJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2WeldJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/Joints/b2WheelJoint.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2Body.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2ContactManager.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2Fixture.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2Island.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2TimeStep.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2World.h \
    ../../../luna2d/thirdparty/Box2D/Dynamics/b2WorldCallbacks.h \
    ../../../luna2d/thirdparty/Box2D/Rope/b2Rope.h \
    ../../../luna2d/thirdparty/Box2D/Box2D.h \
    ../../../luna2d/lua/luanil.h \
    ../../../luna2d/lua/luaobject.h \
    ../../../luna2d/physics/lunaphysicsmodule.h \
    ../../../luna2d/physics/lunaphysicsworld.h \
    ../../../luna2d/physics/lunaphysicsbody.h \
    ../../../luna2d/physics/lunaphysicsfixture.h \
    ../../../luna2d/utils/lunamakeweak.h \
    ../../../luna2d/physics/lunaphysicsdebugrenderer.h \
    ../../../luna2d/graphics/shaders/primitives.vert.h \
    ../../../luna2d/utils/lunaglhelpers.h \
	../../../luna2d/utils/lunamacro.h \
    ../../../luna2d/physics/shapes/lunaphysicscircleshape.h \
    ../../../luna2d/physics/shapes/lunaphysicsedgeshape.h \
    ../../../luna2d/physics/shapes/lunaphysicsrectshape.h \
    ../../../luna2d/physics/shapes/lunaphysicsshape.h \
    ../../../luna2d/physics/joints/lunaphysicsjoint.h \
    ../../../luna2d/physics/joints/lunaphysicsdistancejoint.h \
    ../../../luna2d/physics/lunaphysicsutils.h \
    ../../../luna2d/modules/lunasplinesmodule.h \
    ../../../luna2d/utils/lunavector2.h \
    ../../../luna2d/utils/lunaglm.h \
    ../../../luna2d/platform/qt/lunaqtfiles.h \
    ../../../luna2d/platform/qt/lunaqtlog.h \
    ../../../luna2d/platform/qt/lunaqtgl.h \
    ../../../luna2d/platform/qt/lunaqtutils.h \
    ../../../luna2d/graphics/lunacurverenderer.h \
    ../../../luna2d/lua/luaptr.h

QMAKE_MAC_SDK = macosx10.9
