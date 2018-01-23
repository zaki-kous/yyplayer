//
// Created by 朱乾 on 17/12/18.
//

#include "yyplayer.h"
#include "yysdl_log.h"
#include "ff_ffplay.h"

YYPlayer *yyp_create()
{
    YYPlayer *yyPlayer = mallocz(sizeof(YYPlayer));
    if (!yyPlayer)
        goto fail;
    yyPlayer->ffPlayer = ffp_create();
    if (!yyPlayer->ffPlayer)
        goto fail;
    LOGI("yyp_create success %p.", yyPlayer);
    pthread_mutex_init(&yyPlayer->mutex, NULL);

    return yyPlayer;
    fail:
    return NULL;
}

static int set_data_source_internal(YYPlayer *player, const char *url)
{
    assert(player);
    assert(url);

    freep((void **)&player->data_source);
    player->data_source = strdup(url);
    if (!player->data_source)
        return YYP_OUT_OF_MEMORY;

    return 0;
}

static int yyp_prepare_async_internal(YYPlayer *player)
{
    return ffp_prepare_async(player->ffPlayer, player->data_source);
}

int yyp_set_data_source(YYPlayer *player, const char *url)
{
    assert(player);
    PTHREAD_LOCK(&player->mutex);
    int ret = set_data_source_internal(player, url);
    PTHREAD_UNLOCK(&player->mutex);
    return ret;
}

int yyp_prepare_async(YYPlayer *player)
{
    assert(player);
    PTHREAD_LOCK(&player->mutex);
    int ret = yyp_prepare_async_internal(player);
    PTHREAD_UNLOCK(&player->mutex);
    LOGI("yyp_prepare_async ret :%d", ret);
    return ret;
}

void *yyp_set_weak_ref(YYPlayer *player, void *weak_ref)
{
    assert(player);
    void *pre_weak_ref = player->weak_thiz;
    player->weak_thiz = weak_ref;
    return pre_weak_ref;
}

void yyp_global_init()
{
    assert(player);
    ffp_global_init();
}
