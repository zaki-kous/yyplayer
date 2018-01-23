//
// Created by 朱乾 on 17/12/18.
//
#include <jni.h>
#include "yyplayer_c2j.h"
#include "yysdl_internal.h"
#include "android/yysdl_android_jni.h"
#include "yyplayer_android.h"

#define JNI_CLASS_YYPLAYER "com/me/yyplayer/media/YYPlayer"

static JavaVM* jvm;
typedef struct jni_yyplayer_field {
    pthread_mutex_t jni_mutex;

    jclass clazz;
} jni_yyplayer_field;
static jni_yyplayer_field native_field;

static YYPlayer *jni_get_native_yyplayer(JNIEnv *env, jobject thiz)
{
    PTHREAD_LOCK(&native_field.jni_mutex);
    YYPlayer *mPlayer = (YYPlayer *) (intptr_t) JB_GetNative_YYPlayer(env, thiz);
    PTHREAD_UNLOCK(&native_field.jni_mutex);
    return mPlayer;
}

static void jni_set_native_yyplayer(JNIEnv *env, jobject thiz, YYPlayer *playerRef)
{
    PTHREAD_LOCK(&native_field.jni_mutex);
    YYPlayer *oldRef = (YYPlayer *) (intptr_t) JB_GetNative_YYPlayer(env, thiz);
    JB_SetNative_YYPlayer(env, thiz, (intptr_t) playerRef);
    PTHREAD_UNLOCK(&native_field.jni_mutex);
    if (oldRef)
        LOGW("oldRef :%p.", oldRef);

}

//---------------YYPlayer jni methods----------
static void yyplayer__native_setup(JNIEnv *env, jobject thiz, jobject weak_this)
{
    YYPlayer *yyPlayer = yyp_android_create();
    jni_set_native_yyplayer(env, thiz, yyPlayer);
    yyp_set_weak_ref(yyPlayer, JB_NewGlobalRef(env, weak_this));
}

static void yyplayer_set_datasource(JNIEnv *env, jobject thiz, jstring path)
{
    const char* url = JB_JString2UTFChars(env, path);
    YYPlayer *player = jni_get_native_yyplayer(env, thiz);
    int ret = yyp_set_data_source(player, url);
    JB_ReleaseStringUTFChars(env, path, url);
}

static void yyplayer_open_async(JNIEnv *env, jobject thiz)
{
    int ret = 0;
    YYPlayer *player = jni_get_native_yyplayer(env, thiz);
    JNI_EXCEPTION_CHECK_GOTO(player, env, JNI_IS_Exception_Class, "yyplayer_open_async fial : native_yyplayer is null.", fail);
    ret = yyp_prepare_async(player);

fail:
    ret = -1;
}


const JNINativeMethod jni_methods[] = {
        {
                "_nativeSetup",
                "(Ljava/lang/Object;)V",
                (void *) yyplayer__native_setup
        },
        {
                "_setDataSource",
                "(Ljava/lang/String;)V",
                (void *) yyplayer_set_datasource
        },
        {
                "_openAsync",
                "()V",
                (void *) yyplayer_open_async
        }
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;
    jvm = vm;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    pthread_mutex_init(&native_field.jni_mutex, NULL);

    jclass clazz = JB_FindClass(env, JNI_CLASS_YYPLAYER);
    native_field.clazz = JB_NewGlobalRef(env, clazz);
    JB_DeleteLocalRef(env, clazz);

    //初始化YYPlayer(c2j)
    JB_YYPlayer_Init(env);

    yyp_global_init();

    (*env)->RegisterNatives(env, native_field.clazz, jni_methods, ARRAY_SIZ_ELEN(jni_methods));
    return JNI_VERSION_1_4;
}
