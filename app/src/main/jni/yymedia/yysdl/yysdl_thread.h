//
// Created by 朱乾 on 2018/1/12.
//

#ifndef YYPLAYER_YYSDL_THREAD_H
#define YYPLAYER_YYSDL_THREAD_H

#include <pthread.h>

typedef struct SDL_Thread {
    pthread_t  tid;

    int (*func)(void *);
    void *data;
    char name[32];
    int ret;
} SDL_Thread;

SDL_Thread *SDL_CreateThreadEx(SDL_Thread *thread, int (*func)(void *), void *data, const char *name);


#endif //YYPLAYER_YYSDL_THREAD_H
