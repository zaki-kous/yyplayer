//
// Created by 朱乾 on 17/12/24.
//

#ifndef YYPLAYER_FF_FFPLAY_OPTIONS_H
#define YYPLAYER_FF_FFPLAY_OPTIONS_H

#define FFP_OPTION_OFFSET(x) offsetof(FFPlayer, x)

#define OPTION_INT(_default, _min, _max) \
    .type = AV_OPT_TYPE_INT, \
    {.i64   =   _default},\
    .min = _min, \
    .max = _max, \
    .flags = AV_OPT_FLAG_DECODING_PARAM \

#define OPTION_INT64(_default, _min, _max) \
    .type = AV_OPT_TYPE_INT64, \
    {.i64   =   _default},\
    .min = _min, \
    .max = _max, \
    .flags = AV_OPT_FLAG_DECODING_PARAM \

static const AVOption ffp_context_options[] = {
        {
                "audio_disable",   "disable audio",
                FFP_OPTION_OFFSET(audio_disable), OPTION_INT(0, 0, 1)
        },
        {
                "video_disable",   "disable video",
                FFP_OPTION_OFFSET(video_disable), OPTION_INT(0, 0, 1)
        },
        {
                "subtitle_disable",   "disable subtitle",
                FFP_OPTION_OFFSET(subtitle_disable), OPTION_INT(1, 0, 1)
        },
        {
                "startTime",   "set media start time",
                FFP_OPTION_OFFSET(startTime), OPTION_INT64(150, 0, INT64_MAX)
        },
        {NULL}
};
#endif //YYPLAYER_FF_FFPLAY_OPTIONS_H
