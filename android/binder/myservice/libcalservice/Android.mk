LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES:= CalService.cpp

LOCAL_C_INCLUDES:= $(JNI_H_INCLUDE)  \
	frameworks/base/include


LOCAL_SHARED_LIBRARIES:= \
 libutils \
 libcutils \
 libbinder \
 libandroid_runtime

LOCAL_MODULE:= libCalService

LOCAL_PRELINK_MODULE:= false

include $(BUILD_SHARED_LIBRARY)
