//
// Created by 朱乾 on 18/1/6.
//
#include <jni.h>
#include "jni_base.h"


bool JB_Exception_Check(JNIEnv *env)
{
    if ((*env)->ExceptionCheck(env))
    {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        return true;
    }
    return false;
}

jclass  JB_NewGlobalRef(JNIEnv *env, jobject obj)
{
    jclass global_ref = (*env)->NewGlobalRef(env, obj);

    if (JB_Exception_Check(env) || !global_ref)
    {
        JB_FUNC_FAIL_STACK();
        goto fail;
    }
fail:
    return global_ref;
}

void     JB_DeleteGlobalRef(JNIEnv *env, jobject obj)
{
    if (!obj)
        return;
    (*env)->DeleteGlobalRef(env, obj);
}

void     JB_DeleteGlobalRef_P(JNIEnv *env, jobject *obj)
{
    if (!obj)
        return;
    (*env)->DeleteGlobalRef(env, *obj);
    *obj = NULL;
}

void     JB_DeleteLocalRef(JNIEnv *env, jobject obj)
{
    if (!obj)
        return;
    (*env)->DeleteLocalRef(env, obj);
}

void     JB_DeleteLocalRef_P(JNIEnv *env, jobject *obj)
{
    if (!obj)
        return;

    (*env)->DeleteLocalRef(env, *obj);
    *obj = NULL;
}

void    JB_ReleaseStringUTFChars(JNIEnv *env, jstring str, const char *c_str)
{
    if (!str || !c_str)
        return;
    (*env)->ReleaseStringUTFChars(env, str, c_str);
}

void    JB_ReleaseStringUTFChars_P(JNIEnv *env, jstring str, const char **c_str)
{
    if (!str || !c_str)
        return;
    (*env)->ReleaseStringUTFChars(env, str, *c_str);
    *c_str = NULL;
}

jclass JB_FindClass(JNIEnv *env, const char *class_sign)
{
    jclass clazz = (*env)->FindClass(env, class_sign);
    if (JB_Exception_Check(env) || !clazz)
    {
        JB_FUNC_FAIL_STACK();
        clazz = NULL;
        goto fail;
    }

fail:
    return clazz;
}

jclass  JB_FindClass_asGlobalRef(JNIEnv *env, const char *class_sign)
{
    jclass global_clazz = NULL;
    jclass clazz = JB_FindClass(env, class_sign);
    if (!clazz)
    {
        JB_FUNC_FAIL_STACK1(class_sign);
        goto fail;
    }
    global_clazz = JB_NewGlobalRef(env, clazz);
    if (!global_clazz)
    {
        goto fail;
    }
fail:
    JB_DeleteLocalRef(env, clazz);
    return NULL;
}


jmethodID JB_GetMethodID(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign)
{
    jmethodID method_id = (*env)->GetMethodID(env, clazz, method_name, method_name);
    if (JB_Exception_Check(env) || !method_id)
    {
        JB_FUNC_FAIL_STACK2(method_name, method_sign);
        method_id = NULL;
        goto fail;
    }
fail:
    return method_id;
}

jmethodID JB_GetStaticMethodID(JNIEnv *env, jclass clazz, const char *method_name, const char *method_sign)
{
    jmethodID method_id = (*env)->GetStaticMethodID(env, clazz, method_name, method_name);
    if (JB_Exception_Check(env) || !method_id)
    {
        JB_FUNC_FAIL_STACK2(method_name, method_sign);
        method_id = NULL;
        goto fail;
    }
    fail:
    return method_id;
}

jfieldID  JB_GetFieldID(JNIEnv *env, jclass clazz, const char *field_name, const char *field_sign)
{
    jfieldID field_id = (*env)->GetFieldID(env, clazz, field_name, field_sign);
    if (JB_Exception_Check(env) || !field_id)
    {
        JB_FUNC_FAIL_STACK2(field_name, field_sign);
        field_id = NULL;
        goto fail;
    }

fail:
    return field_id;
}

jfieldID  JB_GetStaticFieldID(JNIEnv *env, jclass clazz, const char *field_name, const char *field_sign)
{
    jfieldID field_id = (*env)->GetStaticFieldID(env, clazz, field_name, field_sign);
    if (JB_Exception_Check(env) || !field_id)
    {
        JB_FUNC_FAIL_STACK2(field_name, field_sign);
        field_id = NULL;
        goto fail;
    }

fail:
    return field_id;
}