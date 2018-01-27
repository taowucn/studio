LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := eng
LOCAL_SRC_FILES:=com_android_service_cal.cpp

LOCAL_C_INCLUDES:= $(JNI_H_INCLUDE)

LOCAL_SHARED_LIBRARIES := libutils \
libCalService \
libCal


LOCAL_MODULE := libJniCal

LOCAL_PRELINK_MODULE:= false

include $(BUILD_SHARED_LIBRARY)
