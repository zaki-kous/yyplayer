//
// Created by 朱乾 on 17/12/17.
//
#include "ff_ffplay.h"
#include "yysdl_log.h"
#include "ff_ffplay_options.h"


static void *ffp_context_child_next(void *obj, void *prev)
{
    return NULL;
}

static const AVClass *ffp_context_child_class_next(const AVClass *prev)
{
    return NULL;
}

static const char *ffp_context_to_name(void *ptr)
{
    return "FFPlayer";
}

const AVClass ff_context_class = {
    .class_name       = "FFPlayer",
    .item_name        = ffp_context_to_name,
    .option           = ffp_context_options,
    .version          = LIBAVUTIL_VERSION_INT,
    .child_next       = ffp_context_child_next,
    .child_class_next = ffp_context_child_class_next
};

FFPlayer *ffp_create() {
    FFPlayer *ffPlayer = mallocz(sizeof(FFPlayer));
    if (!ffPlayer)
        goto fail;
    ffPlayer->af_mutex = SDL_CreateMutex();
    ffPlayer->vf_mutex = SDL_CreateMutex();

    ffPlayer->av_class = &ff_context_class;

    av_opt_set_defaults(ffPlayer);

    return ffPlayer;
    fail:
    return NULL;
}