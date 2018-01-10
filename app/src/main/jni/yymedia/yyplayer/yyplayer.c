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

int yyp_set_data_source(YYPlayer *player, const char *url) {
    PTHREAD_LOCK(&player->mutex);
    int ret = set_data_source_internal(player, url);
    PTHREAD_UNLOCK(&player->mutex);
    LOGI("yyp_set_data_source ret :%d", ret);
    return ret;
}
