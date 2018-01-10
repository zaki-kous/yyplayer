//
// Created by 朱乾 on 17/12/24.
//
#include <android/log.h>

#include "../yysdl_log.h"

void SDL_LOGI(const char *log, ...)
{
    va_list argptr;
    va_start(argptr, log);

    __android_log_vprint(ANDROID_LOG_INFO, LOG_TAG, log, argptr);
}

void SDL_LOGD(const char *log, ...)
{
    va_list argptr;
    va_start(argptr, log);

    __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, log, argptr);
}

void SDL_LOGW(const char *log, ...)
{
    va_list argptr;
    va_start(argptr, log);

    __android_log_vprint(ANDROID_LOG_WARN, LOG_TAG, log, argptr);
}

void SDL_LOGE(const char *log, ...)
{
    va_list argptr;
    va_start(argptr, log);

    __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, log, argptr);
}