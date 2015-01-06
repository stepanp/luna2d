LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= libbox2d
LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/Collision/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Collision/Shapes/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Common/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Dynamics/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Dynamics/Contacts/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Dynamics/Joints/*.cpp) \
	$(wildcard $(LOCAL_PATH)/Rope/*.cpp))
LOCAL_C_INCLUDES := \ $(LOCAL_PATH)/../

LOCAL_OPTIM := release

include $(BUILD_STATIC_LIBRARY)