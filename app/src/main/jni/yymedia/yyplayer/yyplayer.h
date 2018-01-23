//
// Created by 朱乾 on 17/12/18.
//

#ifndef YYPLAYER_YYPLAYER_H
#define YYPLAYER_YYPLAYER_H

#include "yyplayer_internal.h"
#include "yysdl_internal.h"

typedef struct YYPlayer YYPlayer;

YYPlayer *yyp_create();

void  yyp_global_init();
int   yyp_set_data_source(YYPlayer *player, const char *url);
int   yyp_prepare_async(YYPlayer *player);
void *yyp_set_weak_ref(YYPlayer *player, void *weak_ref);

#endif //YYPLAYER_YYPLAYER_H
