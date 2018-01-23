//
// Created by 朱乾 on 17/12/24.
//

#ifndef YYPLAYER_FF_FFPLAY_H
#define YYPLAYER_FF_FFPLAY_H

#include "ff_ffplay_def.h"

FFPlayer *ffp_create();
void      ffp_global_init();
void      ffp_toggle_buffing(FFPlayer *ff, int start_buffing);
int       ffp_prepare_async(FFPlayer *ff, const char* url);

#endif //YYPLAYER_FF_FFPLAY_H
