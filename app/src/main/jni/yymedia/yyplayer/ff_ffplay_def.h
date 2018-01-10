//
// Created by 朱乾 on 17/12/24.
//

#ifndef YYPLAYER_FF_FFPLAY_DEF_H
#define YYPLAYER_FF_FFPLAY_DEF_H

#include "libavutil/opt.h"
#include "ff_ffplay_inc.h"

typedef struct FFPlayer {
    const AVClass *av_class;

    int audio_disable;
    int video_disable;
    int subtitle_disable;

    int64_t startTime;

    SDL_mutex *af_mutex;
    SDL_mutex *vf_mutex;

} FFPlayer;

#endif //YYPLAYER_FF_FFPLAY_DEF_H
