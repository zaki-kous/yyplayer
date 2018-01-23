//
// Created by 朱乾 on 17/12/24.
//

#ifndef YYPLAYER_YYPLAYER_INTERNAL_H
#define YYPLAYER_YYPLAYER_INTERNAL_H

#include <pthread.h>
#include <assert.h>
#include "ff_ffplay.h"

struct YYPlayer {
    FFPlayer *ffPlayer;
    pthread_mutex_t mutex;

    jclass weak_thiz;
    char *data_source;
};

#endif //YYPLAYER_YYPLAYER_INTERNAL_H
