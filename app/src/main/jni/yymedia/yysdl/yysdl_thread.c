//
// Created by 朱乾 on 2018/1/12.
//
#include <assert.h>
#include "yysdl_thread.h"
#include "yysdl_log.h"
#include "android/yysdl_android_jni.h"

static void *SDL_ThreadRun(void *args)
{
    SDL_Thread *thread = args;
    LOGI("run thread :%s", thread->name);
    pthread_setname_np(pthread_self(), thread->name);
    thread->ret = thread->func(thread->data);
#ifdef __ANDROID__
    SDL_DetachThreadEnv();
#endif
    return NULL;
}

SDL_Thread *SDL_CreateThreadEx(SDL_Thread *thread, int (*func)(void *), void *data, const char *name)
{
    assert(thread);
    thread->func = func;
    thread->data = data;
    strlcpy(thread->name, name, sizeof(thread->name) - 1);
    int ret = pthread_create(&thread->tid, NULL, SDL_ThreadRun, thread);
    if (ret)
        return NULL;
    return thread;
}
