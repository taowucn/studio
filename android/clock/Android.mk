
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := clock.cpp

LOCAL_MODULE := clock_test

include $(BUILD_EXECUTABLE)

