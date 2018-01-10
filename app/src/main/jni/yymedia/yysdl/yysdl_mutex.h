//
// Created by 朱乾 on 17/12/24.
//

#ifndef YYPLAYER_YYSDL_MUTEX_H
#define YYPLAYER_YYSDL_MUTEX_H

#include <pthread.h>
#include "yysdl_internal.h"

#define SDL_MUTEX_TIMEOUT 1

typedef struct SDL_mutex {
    pthread_mutex_t mutex_id;
} SDL_mutex;

SDL_mutex *SDL_CreateMutex();
void       SDL_DestroyMutex(SDL_mutex *mutex);
void       SDL_DestroyMutexP(SDL_mutex **mutex);
int        SDL_LockMutex(SDL_mutex *mutex);
int        SDL_UnlockMutex(SDL_mutex *mutex);

typedef struct SDL_cond {
    pthread_cond_t cond_id;
} SDL_cond;

SDL_cond *SDL_CreateCond();
void     *SDL_DestroyCond(SDL_cond *cond);
void     *SDL_DestroyCondP(SDL_cond **cond);
int       SDL_CondSignal(SDL_cond *cond);
int       SDL_CondWait(SDL_cond *cond, SDL_mutex *mutex);
int       SDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, uint32_t ms);

#endif //YYPLAYER_YYSDL_MUTEX_H
