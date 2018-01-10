LOCAL_PATH := $(call my-dir)

MY_APP_JNI_PATH := $(realpath $(LOCAL_PATH))
MY_APP_MEDIA_INCLUDE_PATH := $(realpath $(LOCAL_PATH)/yymedia)
MY_APP_SDL_INCLUDE_PATH := $(realpath $(MY_APP_MEDIA_INCLUDE_PATH)/yysdl)
MY_APP_FFMPEG_INCLUDE_PATH := $(realpath $(LOCAL_PATH)/ffmpeg/include)

include $(call all-subdir-makefiles)