LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_FLAGS     := -std=c99

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(MY_APP_SDL_INCLUDE_PATH)
LOCAL_C_INCLUDES += $(MY_APP_FFMPEG_INCLUDE_PATH)

LOCAL_SRC_FILES := ff_ffplay.c \
                   android/yyplayer_jni.c \
                   android/yyplayer_c2j.c \
                   android/yyplayer_android.c \
                   yyplayer.c \

LOCAL_SHARED_LIBRARIES := yyffmpeg yysdl

LOCAL_MODULE := yyplayer

include $(BUILD_SHARED_LIBRARY)