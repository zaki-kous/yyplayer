//
// Created by 朱乾 on 2018/1/12.
//

#ifndef YYPLAYER_YYSDL_ANDROID_JNI_H
#define YYPLAYER_YYSDL_ANDROID_JNI_H

#include "jni/jni_base.h"
#define JNI_IS_Exception_Class "java/lang/IllegalStateException"

int   SDL_SetupThreadEnv(JNIEnv **p_env);
void  SDL_DetachThreadEnv();
int   SDL_JNI_ThrowException(JNIEnv *env, const char *exception_class_name, const char *message);


#define JNI_EXCEPTION_CHECK_GOTO(expression__, env__, exception__, msg__, goto_label) \
        do { \
            if (!(expression__)) \
            { \
                if (exception__) \
                { \
                    SDL_JNI_ThrowException(env__, exception__, msg__); \
                } \
                goto goto_label; \
            } \
        } while(0)
#endif //YYPLAYER_YYSDL_ANDROID_JNI_H
