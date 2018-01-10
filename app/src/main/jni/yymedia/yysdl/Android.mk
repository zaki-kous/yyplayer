LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS += -llog

LOCAL_SRC_FILES := yysdl_vout.c \
                   yysdl_mutex.c \
                   android/yysdl_android_log.c \

LOCAL_MODULE := yysdl

include $(BUILD_SHARED_LIBRARY)