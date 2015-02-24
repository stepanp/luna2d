LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := luna2d

LOCAL_SRC_FILES := \
	$(LOCAL_PATH)/../../../../luna2d/platform/android/jni/lunajnibridge.cpp \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/modules/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/graphics/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/physics/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/physics/shapes/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/physics/joints/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/utils/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/lua/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/platform/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/platform/android/*.cpp) \
	$(wildcard $(LOCAL_PATH)/../../../luna2d/thirdparty/json11/*.cpp))
	
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../../../luna2d/ \
	$(LOCAL_PATH)/../../../luna2d/modules/ \
	$(LOCAL_PATH)/../../../luna2d/graphics/ \
	$(LOCAL_PATH)/../../../luna2d/physics/ \
	$(LOCAL_PATH)/../../../luna2d/utils/ \
	$(LOCAL_PATH)/../../../luna2d/lua/ \
	$(LOCAL_PATH)/../../../luna2d/platform/ \
	$(LOCAL_PATH)/../../../luna2d/platform/android/ \
	$(LOCAL_PATH)/../../../luna2d/thirdparty/ \
	$(LOCAL_PATH)/../../../luna2d/thirdparty/lua/ \
	$(LOCAL_PATH)/../../../luna2d/thirdparty/libzip/ \
	$(LOCAL_PATH)/../../../luna2d/thirdparty/libpng/ \
	$(LOCAL_PATH)/../../../luna2d/thirdparty/json11/ \
	$(LOCAL_PATH)/../../../luna2d/thirdparty/Box2D/
	
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2 -ljnigraphics -lz
LOCAL_STATIC_LIBRARIES := lua libzip libpng libbox2d

include $(BUILD_SHARED_LIBRARY)

SAVED_LOCAL_PATH := $(LOCAL_PATH)
include $(SAVED_LOCAL_PATH)/../../../luna2d/thirdparty/lua/Android.mk
include $(SAVED_LOCAL_PATH)/../../../luna2d/thirdparty/libzip/Android.mk
include $(SAVED_LOCAL_PATH)/../../../luna2d/thirdparty/libpng/Android.mk
include $(SAVED_LOCAL_PATH)/../../../luna2d/thirdparty/Box2D/Android.mk