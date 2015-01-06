LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := lua
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,,	$(wildcard $(LOCAL_PATH)/*.c))
LOCAL_C_INCLUDES := $(LOCAL_PATH)/src
LOCAL_OPTIM := release
	
include $(BUILD_STATIC_LIBRARY)