//
// Created by 朱乾 on 2018/1/12.
//

#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "yysdl_android_jni.h"
#include "yysdl_log.h"

static pthread_once_t g_thread_once = PTHREAD_ONCE_INIT;
static pthread_key_t g_thread_key;

static JavaVM *g_jvm;

static void thread_key_destroy(void *value)
{
    JNIEnv *env = value;
    if (!env)
    {
        LOGI("DetachCurrentThread tid :%d", (int) gettid());
        (*g_jvm)->DetachCurrentThread(g_jvm);
        pthread_setspecific(g_thread_key, NULL);
    }
}

static void make_thread_key()
{
    pthread_key_create(&g_thread_key, thread_key_destroy);
}

int SDL_SetupThreadEnv(JNIEnv **p_env)
{
    JavaVM *jvm = g_jvm;
    if (!jvm)
    {
        LOGE("SDL_SetupThreadEnv fial jvm point is null ?");
        return -1;
    }
    pthread_once(&g_thread_once, make_thread_key);
    JNIEnv *env = pthread_getspecific(g_thread_key);
    if (env)
    {
        *p_env = env;
        return 0;
    }

    if ((*jvm)->AttachCurrentThread(jvm, &env, NULL) == JNI_OK)
    {
        *p_env = env;
        pthread_setspecific(g_thread_key, env);
        return 0;
    }

    return -1;
}

void SDL_DetachThreadEnv()
{
    JavaVM *jvm = g_jvm;
    pthread_once(&g_thread_once, make_thread_key);

    JNIEnv *env = pthread_getspecific(g_thread_key);
    if (!env)
        return;
    pthread_setspecific(g_thread_key, NULL);
    int ret = (*jvm)->DetachCurrentThread(g_jvm);
    LOGI("DetachCurrentThread ret :%d", ret);
}

int SDL_JNI_ThrowException(JNIEnv *env, const char *exception_class_name, const char *message)
{
    if ((*env)->ExceptionCheck(env))
    {
        jthrowable throwable = (*env)->ExceptionOccurred(env);
        (*env)->ExceptionClear(env);
        LOGI("ExceptionClear throw to (%s).", exception_class_name);
        if (throwable)
        {
            JB_DeleteLocalRef(env, &throwable);
        }
    }

    jclass exception_clazz = JB_FindClass(env, exception_class_name);
    if (!exception_clazz)
    {
        LOGE("don't find class :%s ", exception_class_name);
        goto fail;
    }

    if ((*env)->ThrowNew(env, exception_clazz, message) != JNI_OK)
    {
        LOGE("JNI_ThrowException fail :%s, :%s", exception_class_name, message);
        goto fail;
    }

    return 0;
fail:
    if (exception_clazz)
        JB_DeleteLocalRef(env, exception_clazz);
    return -1;
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved)
{
    g_jvm = vm;

    return JNI_VERSION_1_4;
}