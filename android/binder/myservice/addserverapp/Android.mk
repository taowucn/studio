LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= addserverapp.cpp
LOCAL_MODULE_TAGS := eng

LOCAL_C_INCLUDES:= $(JNI_H_INCLUDE)  \
	frameworks/base/include

LOCAL_SHARED_LIBRARIES:= libutils libCalService libbinder

LOCAL_MODULE:= addserverapp

include $(BUILD_EXECUTABLE)
