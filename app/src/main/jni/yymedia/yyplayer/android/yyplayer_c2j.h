//
// Created by 朱乾 on 18/1/8.
//

#ifndef YYPLAYER_YYPLAYER_C2J_H
#define YYPLAYER_YYPLAYER_C2J_H

#include "android/jni/jni_base.h"

int      JB_YYPlayer_Init(JNIEnv *env);
jlong    JB_GetNative_YYPlayer(JNIEnv *env, jobject thiz);
void     JB_SetNative_YYPlayer(JNIEnv *env, jobject thiz, jlong jvalue);
#endif //YYPLAYER_YYPLAYER_C2J_H
