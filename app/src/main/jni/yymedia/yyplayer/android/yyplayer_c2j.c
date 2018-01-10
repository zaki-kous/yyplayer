//
// Created by 朱乾 on 18/1/8.
//
#include "yyplayer_c2j.h"

typedef struct JB_YYPlayer_Native {
    jclass id;

    jfieldID mNativePlayer;
} JB_YYPlayer_Native;

static JB_YYPlayer_Native native_player;

int JB_YYPlayer_Init(JNIEnv *env)
{
    int ret = -1;
    const char *sign = "com/me/yyplayer/media/YYPlayer";
    const char *name = "mNativeYYPlayer";
    if (native_player.id)
        return 0;
    native_player.id = JB_FindClass(env, sign);

    sign = "J";
    native_player.mNativePlayer = JB_GetFieldID(env, native_player.id, name, sign);
    LOGI("YPlayer_Native_Init success id :%p, mNativePlayer :%p", native_player.id, native_player.mNativePlayer);

    return ret;
}
jlong    JB_GetNative_YYPlayer(JNIEnv *env, jobject thiz)
{
    return (*env)->GetLongField(env, thiz, native_player.mNativePlayer);
}
