//
// Created by 朱乾 on 18/1/6.
//

#ifndef YYPLAYER_JNI_BASE_H
#define YYPLAYER_JNI_BASE_H

#include <jni.h>
#include <stdbool.h>
#include "yysdl_log.h"
#include "../../../../../../../../../../java/sdk/ndk-bundle/sysroot/usr/include/jni.h"


#define JB_FUNC_FAIL_STACK()    do{LOGE("%s: fail.", __func__);}while(0)
#define JB_FUNC_FAIL_STACK1(x__)    do{LOGE("%s, fail: %s.", __func__, x__);}while(0)
#define JB_FUNC_FAIL_STACK2(x1__, x2__)    do{LOGE("%s, fail: %s, %s.", __func__, x1__, x2__);}while(0)

bool        JB_Exception_Check(JNIEnv *env);
jclass      JB_NewGlobalRef(JNIEnv *env, jobject obj);
void        JB_DeleteGlobalRef(JNIEnv *env, jobject obj);
void        JB_DeleteGlobalRef_P(JNIEnv *env, jobject *obj);
void        JB_DeleteLocalRef(JNIEnv *env, jobject obj);
void        JB_DeleteLocalRef_P(JNIEnv *env, jobject *obj);

const char *JB_JString2UTFChars(JNIEnv *env, jstring str);
void        JB_ReleaseStringUTFChars(JNIEnv *env, jstring str, const char *c_str);
void        JB_ReleaseStringUTFChars_P(JNIEnv *env, jstring str, const char **c_str);

jclass      JB_FindClass(JNIEnv *env, const char *class_sign);
jclass      JB_FindClass_asGlobalRef(JNIEnv *env, const char *class_sign);

jmethodID   JB_GetMethodID(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign);
jmethodID   JB_GetStaticMethodID(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign);

jfieldID    JB_GetFieldID(JNIEnv *env, jclass clazz, const char *field_name, const char *field_sign);
jfieldID    JB_GetStaticFieldID(JNIEnv *env, jclass clazz, const char *field_name, const char *field_sign);

#endif //YYPLAYER_JNI_BASE_H
