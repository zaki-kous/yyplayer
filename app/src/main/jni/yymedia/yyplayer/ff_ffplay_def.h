//
// Created by 朱乾 on 17/12/24.
//

#ifndef YYPLAYER_FF_FFPLAY_DEF_H
#define YYPLAYER_FF_FFPLAY_DEF_H

#include "libavutil/opt.h"
#include "libavformat/avformat.h"
#include "ff_ffplay_inc.h"

#define MIN_PKT_DURATION 15

typedef struct YYPacketList {
    AVPacket pkt;
    struct YYPacketList *next;
    int serial;
} YYPacketList;

typedef struct PacketQueue {
    YYPacketList *first_pkt, *last_pkt;
    YYPacketList *recycle_pkt;

    SDL_Cond *cond;
    SDL_Mutex *mutex;

    int serial;

    int size;
    int nb_packets;
    int abort_request;

    int recycle_count;
    int alloc_count;

    int64_t duration;
} PacketQueue;

typedef struct VideoState {
    AVFormatContext *ic;
    AVInputFormat *input;

    int video_stream_index;
    int audio_stream_index;
    AVStream *video_stream;
    AVStream *audio_stream;
    int eof;

    int abort_request;

    SDL_Thread *read_thread;
    SDL_Thread _read_thread;

    PacketQueue video_queue;
    PacketQueue audio_queue;

    SDL_Mutex *play_mutex;

    char *filename;
} VideoState;

typedef struct FFPlayer {
    const AVClass *av_class;

    VideoState *video;

    int audio_disable;
    int video_disable;
    int subtitle_disable;

    int64_t startTime;

    char *input_filename;

    SDL_Mutex *af_mutex;
    SDL_Mutex *vf_mutex;

} FFPlayer;

#endif //YYPLAYER_FF_FFPLAY_DEF_H
