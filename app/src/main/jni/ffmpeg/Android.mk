LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := yyffmpeg
LOCAL_SRC_FILES := $(LOCAL_PATH)/armeabi-v7a/libffmpeg.so

include $(PREBUILT_SHARED_LIBRARY)