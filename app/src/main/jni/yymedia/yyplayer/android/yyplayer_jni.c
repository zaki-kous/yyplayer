//
// Created by 朱乾 on 17/12/18.
//
#include <jni.h>
#include "yyplayer_c2j.h"
#include "yysdl_internal.h"
#include "yyplayer_android.h"

#define JNI_CLASS_YYPLAYER "com/me/yyplayer/media/YYPlayer"

static JavaVM* jvm;

//---------------YYPlayer jni methods----------
static jlong yyplayer__native_setup(JNIEnv *env, jobject thiz, jobject weak_this)
{
    YYPlayer *yyPlayer = yyp_android_create();

    return (jlong) yyPlayer;
}

static void yyplayer_set_datasource(JNIEnv *env, jobject thiz, jstring path)
{
    const char* url = (const char *) (*env)->GetStringChars(env, path, NULL);
}

static void yyplayer_open_async(JNIEnv *env, jobject thiz)
{

}


const JNINativeMethod jni_methods[] = {
        {
                "_nativeSetup",
                "(Ljava/lang/Object;)J",
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

    jclass clazz = JB_FindClass(env, JNI_CLASS_YYPLAYER);
    jobject jplayer = JB_NewGlobalRef(env, clazz);
    JB_DeleteLocalRef(env, clazz);

    //初始化YYPlayer(c2j)
    JB_YYPlayer_Init(env);

    (*env)->RegisterNatives(env, jplayer, jni_methods, ARRAY_SIZ_ELEN(jni_methods));
    return JNI_VERSION_1_4;
}
