//
// Created by 朱乾 on 17/12/24.
//

#include <assert.h>
#include "yysdl_mutex.h"
SDL_Mutex *SDL_CreateMutex()
{
    SDL_Mutex *mutex = mallocz(sizeof(SDL_Mutex));
    if (!mutex)
        return NULL;
    if (pthread_mutex_init(&mutex->mutex_id, NULL) != 0) {
        free(mutex);
        return NULL;
    }
    return mutex;
}

void SDL_DestroyMutex(SDL_Mutex *mutex)
{
    if (mutex) {
        pthread_mutex_destroy(&mutex->mutex_id);
        free(mutex);
    }
}

void SDL_DestroyMutexP(SDL_Mutex **mutex)
{
    if (mutex) {
        SDL_DestroyMutex(*mutex);
        *mutex = NULL;
    }
}

int SDL_LockMutex(SDL_Mutex *mutex)
{
    assert(mutex);
    if (!mutex)
        return -1;
    return pthread_mutex_lock(&mutex->mutex_id);
}

int SDL_UnlockMutex(SDL_Mutex *mutex)
{
    assert(mutex);
    if (!mutex)
        return -1;

    return pthread_mutex_unlock(&mutex->mutex_id);
}

SDL_Cond *SDL_CreateCond()
{
    SDL_Cond *cond = mallocz(sizeof(SDL_Cond));
    if (!cond)
        return NULL;

    if (pthread_cond_init(&cond->cond_id, NULL) != 0) {
        free(cond);
        return NULL;
    }
    return cond;
}

void  *SDL_DestroyCond(SDL_Cond *cond)
{
    if (cond) {
        pthread_cond_destroy(&cond->cond_id);
        free(cond);
    }
}

void *SDL_DestroyCondP(SDL_Cond **cond)
{
    if (cond) {
        SDL_DestroyCond(*cond);
        *cond = NULL;
    }
}

int SDL_CondSignal(SDL_Cond *cond)
{
    assert(cond);
    if (!cond)
        return -1;

    return pthread_cond_signal(&cond->cond_id);
}

int SDL_CondWait(SDL_Cond *cond, SDL_Mutex *mutex)
{
    assert(cond);
    assert(mutex);
    if (!cond || !mutex)
        return -1;

    return pthread_cond_wait(&cond->cond_id, &mutex->mutex_id);
}

int SDL_CondWaitTimeout(SDL_Cond *cond, SDL_Mutex *mutex, uint32_t ms)
{
    int retval;
    struct timeval datatime;
    struct timespec abstime;

    assert(cond);
    assert(mutex);
    if (!cond || !mutex)
        return -1;

    gettimeofday(&datatime, NULL);
    abstime.tv_sec = datatime.tv_sec + ms / 1000;
    abstime.tv_nsec = (datatime.tv_usec + (ms % 1000) * 1000) * 1000;

    if (abstime.tv_nsec > 1000000000) {
        abstime.tv_sec += 1;
        abstime.tv_nsec -= 1000000000;
    }

    while (1) {
        retval = pthread_cond_timedwait(&cond->cond_id, &mutex->mutex_id, &abstime);
        if (retval == 0)
            return 0;
        else if (retval == EINTR)
            continue;
        else if (retval == ETIMEDOUT)
            return SDL_MUTEX_TIMEOUT;
        else
            break;
    }

    return -1;
}
