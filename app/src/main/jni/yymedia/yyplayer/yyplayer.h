//
// Created by 朱乾 on 17/12/18.
//

#ifndef YYPLAYER_YYPLAYER_H
#define YYPLAYER_YYPLAYER_H

#include "yyplayer_internal.h"
#include "yysdl_internal.h"

typedef struct YYPlayer YYPlayer;

YYPlayer *yyp_create();

int yyp_set_data_source(YYPlayer *player, const char *url);

#endif //YYPLAYER_YYPLAYER_H
