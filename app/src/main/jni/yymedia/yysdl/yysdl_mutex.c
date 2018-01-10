//
// Created by 朱乾 on 17/12/24.
//

#include <assert.h>
#include "yysdl_mutex.h"
SDL_mutex *SDL_CreateMutex()
{
    SDL_mutex *mutex = mallocz(sizeof(SDL_mutex));
    if (!mutex)
        return NULL;
    if (pthread_mutex_init(&mutex->mutex_id, NULL) != 0) {
        free(mutex);
        return NULL;
    }
    return mutex;
}

void SDL_DestroyMutex(SDL_mutex *mutex)
{
    if (mutex) {
        pthread_mutex_destroy(&mutex->mutex_id);
        free(mutex);
    }
}

void SDL_DestroyMutexP(SDL_mutex **mutex)
{
    if (mutex) {
        SDL_DestroyMutex(*mutex);
        *mutex = NULL;
    }
}

int SDL_LockMutex(SDL_mutex *mutex)
{
    assert(mutex);
    if (!mutex)
        return -1;
    return pthread_mutex_lock(&mutex->mutex_id);
}

int SDL_UnlockMutex(SDL_mutex *mutex)
{
    assert(mutex);
    if (!mutex)
        return -1;

    return pthread_mutex_unlock(&mutex->mutex_id);
}

SDL_cond *SDL_CreateCond()
{
    SDL_cond *cond = mallocz(sizeof(SDL_cond));
    if (!cond)
        return NULL;

    if (pthread_cond_init(&cond->cond_id, NULL) != 0) {
        free(cond);
        return NULL;
    }
    return cond;
}

void  *SDL_DestroyCond(SDL_cond *cond)
{
    if (cond) {
        pthread_cond_destroy(&cond->cond_id);
        free(cond);
    }
}

void *SDL_DestroyCondP(SDL_cond **cond)
{
    if (cond) {
        SDL_DestroyCond(*cond);
        *cond = NULL;
    }
}

int SDL_CondSignal(SDL_cond *cond)
{
    assert(cond);
    if (!cond)
        return -1;

    return pthread_cond_signal(&cond->cond_id);
}

int SDL_CondWait(SDL_cond *cond, SDL_mutex *mutex)
{
    assert(cond);
    assert(mutex);
    if (!cond || !mutex)
        return -1;

    return pthread_cond_wait(&cond->cond_id, &mutex->mutex_id);
}

int SDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, uint32_t ms)
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
