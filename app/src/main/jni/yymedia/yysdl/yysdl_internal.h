//
// Created by 朱乾 on 17/12/18.
//

#ifndef YYPLAYER_YYSDL_INTERNAL_H
#define YYPLAYER_YYSDL_INTERNAL_H

#include <stdlib.h>
#include <memory.h>
#include <jni.h>

#ifndef ARRAY_SIZ_ELEN
#define ARRAY_SIZ_ELEN(x) ((int) (sizeof(x) / sizeof(x[0])))
#endif

#ifndef PTHREAD_LOCK
#define PTHREAD_LOCK(x) pthread_mutex_lock(x)
#endif

#ifndef PTHREAD_UNLOCK
#define PTHREAD_UNLOCK(x) pthread_mutex_unlock(x)
#endif

inline static void *mallocz(size_t size) {
    void *mem = malloc(size);
    if (!mem) {
        return NULL;
    }
    memset(mem, 0, size);
    return mem;
}

inline static void freep(void **mmp)
{
    if (mmp && *mmp) {
        free(*mmp);
        *mmp = NULL;
    }
}


#endif //YYPLAYER_YYSDL_INTERNAL_H
