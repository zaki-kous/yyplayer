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
} SDL_Mutex;

SDL_Mutex *SDL_CreateMutex();
void       SDL_DestroyMutex(SDL_Mutex *mutex);
void       SDL_DestroyMutexP(SDL_Mutex **mutex);
int        SDL_LockMutex(SDL_Mutex *mutex);
int        SDL_UnlockMutex(SDL_Mutex *mutex);

typedef struct SDL_cond {
    pthread_cond_t cond_id;
} SDL_Cond;

SDL_Cond *SDL_CreateCond();
void     *SDL_DestroyCond(SDL_Cond *cond);
void     *SDL_DestroyCondP(SDL_Cond **cond);
int       SDL_CondSignal(SDL_Cond *cond);
int       SDL_CondWait(SDL_Cond *cond, SDL_Mutex *mutex);
int       SDL_CondWaitTimeout(SDL_Cond *cond, SDL_Mutex *mutex, uint32_t ms);

#endif //YYPLAYER_YYSDL_MUTEX_H
