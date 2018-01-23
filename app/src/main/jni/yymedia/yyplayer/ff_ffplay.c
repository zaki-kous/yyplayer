//
// Created by 朱乾 on 17/12/17.
//
#include "ff_ffplay.h"
#include "libavformat/avformat.h"
#include "yysdl_log.h"
#include "ff_ffplay_options.h"

static AVPacket flush_pkt;

static int packet_queue_put_private(PacketQueue *queue, AVPacket *packet)
{
    if (queue->abort_request) {
        return -1;
    }

    YYPacketList *pktList = queue->recycle_pkt;
    if (pktList) {
        queue->recycle_pkt = pktList->next;
        queue->recycle_count++;
    } else{
        pktList = av_malloc(sizeof(YYPacketList));
        queue->alloc_count++;
    }

    if (!pktList) {
        return -1;
    }

    pktList->pkt = *packet;
    pktList->next = NULL;
    if (packet == &flush_pkt) {
        queue->serial++;
    }
    pktList->serial = queue->serial;

    if (!queue->last_pkt) {
        queue->first_pkt = pktList;
    } else {
        queue->last_pkt->next = pktList;
    }
    queue->last_pkt = pktList;
    queue->size += packet->size + sizeof(AVPacketList);

    queue->duration += FFMAX(packet->duration, MIN_PKT_DURATION);
    SDL_CondSignal(queue->cond);
    return 0;
}

static int packet_queue_put(PacketQueue *queue, AVPacket *packet)
{
    int ret = 0;
    SDL_LockMutex(queue->mutex);
    ret = packet_queue_put_private(queue, packet);
    SDL_UnlockMutex(queue->mutex);
    if (ret < 0)
    {
        av_packet_unref(packet);
    }
    return ret;
}

static int packet_queue_init(PacketQueue *queue)
{
    memset(queue, 0, sizeof(PacketQueue));
    queue->mutex = SDL_CreateMutex();
    if (!queue->mutex)
    {
        LOGE("packet_queue_init fail, create Mutex error.");
        return AVERROR(ENOMEM);
    }
    queue->cond = SDL_CreateCond();
    if (!queue->cond)
    {
        LOGE("packet_queue_init fail, create cond error.");
        return AVERROR(ENOMEM);
    }
    queue->abort_request = 0;
    return 0;
}

static int packet_queue_get(PacketQueue *queue, AVPacket *pkt, int block, int *serial)
{
    YYPacketList *pktList;
    int ret = 0;

    SDL_LockMutex(queue->mutex);

    for (;;) {
        if (queue->abort_request) {
            break;
        }
        pktList = queue->first_pkt;

        if (pktList) {
            queue->first_pkt = pktList->next;
            if (!queue->first_pkt)
                queue->last_pkt = NULL;

            *pkt = pktList->pkt;
            if (serial)
                *serial = queue->serial;

            queue->size -= pktList->pkt.size + sizeof(AVPacketList);
            queue->nb_packets--;
            queue->duration -= FFMAX(pktList->pkt.duration, MIN_PKT_DURATION);

            pktList->next = queue->recycle_pkt;
            queue->recycle_pkt = pktList;
            ret = 1;
            break;
        } else if (block) {
            ret = 0;
            break;
        } else {
            SDL_CondWait(queue->cond, queue->mutex);
        }
    }

    SDL_UnlockMutex(queue->mutex);
    return ret;
}

static int packet_queue_get_buffering(FFPlayer *ff, PacketQueue *queue, AVPacket *pkt, int *serial, int *finished)
{
    while(1) {
        int packet_ret = packet_queue_get(queue, pkt, 0, serial);
        if (packet_ret < 0) {
            return -1;
        } else if (packet_ret == 0) {
            //开始缓冲
            if (!finished) {
                ffp_toggle_buffing(ff, 1);
            }
            packet_ret = packet_queue_get(queue, pkt, 1, serial);
            if (packet_ret < 0) {
                return -1;
            }

            if (*finished == *serial) {
                av_packet_unref(pkt);
                continue;
            } else{
                break;
            }
        }
    }
}

static int stream_component_open(FFPlayer *ff, int stream_index)
{
    VideoState *vs = ff->video;
    AVFormatContext *ic = vs->ic;
    AVCodecContext *avctx = NULL;
    AVCodec *codec;
    int ret;

    if (stream_index < 0 || stream_index >= ic->nb_streams) {
        return -1;
    }
    avctx = avcodec_alloc_context3(NULL);

    ret = avcodec_parameters_to_context(avctx, ic->streams[stream_index]->codecpar);
    if (ret < 0)
        goto fail;
    av_codec_set_pkt_timebase(avctx, ic->streams[stream_index]->time_base);
    codec = avcodec_find_decoder(avctx->codec_id);
    if (!codec) {
        LOGE("avcodec_find_decoder fail codec_id: %d", avctx->codec_id);
        goto fail;
    }

    int lowres = av_codec_get_max_lowres(codec);
    LOGI("codec max lowres :%d", lowres);
    if (lowres > 0) {
        av_codec_set_lowres(avctx, lowres);
    }

    //open codec
    if (avcodec_open2(avctx, codec, NULL) < 0) {
        LOGE("avcodec_open2 fail.");
        goto fail;
    }

    vs->eof = 0;
    switch (avctx->codec_type)
    {
        case AVMEDIA_TYPE_VIDEO:
            vs->video_stream_index = stream_index;
            vs->video_stream = ic->streams[stream_index];
            break;
        case AVMEDIA_TYPE_AUDIO:
            vs->audio_stream = ic->streams[stream_index];
            vs->audio_stream_index = stream_index;
            break;
        default:
            break;
    }

    goto success;
fail:
    if (avctx) {
        avcodec_free_context(&avctx);
        avctx = NULL;
    }
success:
    return ret;
}

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

static int decode_interrupt_callback(void *args)
{
    VideoState *vs = args;
    return vs->abort_request;
}

static int read_thread(void *args)
{
    FFPlayer *ff = args;
    VideoState *vs = ff->video;
    AVFormatContext *ic = NULL;
    int stream_index[AVMEDIA_TYPE_NB];
    int ret;
    char error_str[128] = {0};

    memset(stream_index, -1, sizeof(stream_index));
    vs->video_stream_index = -1;
    vs->audio_stream_index = -1;
    vs->eof = 0;

    ic = avformat_alloc_context();
    if (!ic) {
        LOGW("avformat_alloc_context fail.");
        goto fail;
    }
    ic->interrupt_callback.callback = decode_interrupt_callback;
    ic->interrupt_callback.opaque = vs;

    ret = avformat_open_input(&ic, vs->filename, NULL, NULL);
    if (ret < 0) {
        av_strerror(ret, error_str, sizeof(error_str));
        LOGE("fial open input filename :%s, %s", vs->filename, error_str);
        goto fail;
    }

    ret = avformat_find_stream_info(ic, NULL);
    if (ret < 0) {
        LOGE("find stream info fail ret: %d", ret);
        goto fail;
    }

    LOGI("open file [%s] success", vs->filename);
    vs->ic = ic;

    //find stream index
    if (!ff->video_disable && stream_index[AVMEDIA_TYPE_VIDEO] == -1) {
        stream_index[AVMEDIA_TYPE_VIDEO] = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, stream_index[AVMEDIA_TYPE_VIDEO],
                                                               -1, NULL, 0);
        LOGI("find video stream index :%d", stream_index[AVMEDIA_TYPE_VIDEO]);
    }

    if (!ff->audio_disable && stream_index[AVMEDIA_TYPE_AUDIO] == -1) {
        stream_index[AVMEDIA_TYPE_AUDIO] = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, stream_index[AVMEDIA_TYPE_AUDIO],
                                                               stream_index[AVMEDIA_TYPE_VIDEO], NULL, 0);
        LOGI("find audio stream index :%d", stream_index[AVMEDIA_TYPE_AUDIO]);
    }

    if (stream_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        stream_component_open(ff, stream_index[AVMEDIA_TYPE_VIDEO]);
    }

    if (stream_index[AVMEDIA_TYPE_AUDIO] >= 0) {
        stream_component_open(ff, stream_index[AVMEDIA_TYPE_AUDIO]);
    }
fail:
    if (ic && vs->ic) {
        avformat_close_input(&ic);
    }
    return 0;
}

static VideoState *stream_open(FFPlayer *ffPlayer, const char* url)
{
    assert(!ffPlayer->video);

    VideoState *vs = NULL;
    vs = av_mallocz(sizeof(VideoState));
    if (!vs)
        goto fail;
    vs->filename = av_strdup(url);
    if (!vs->filename)
        goto fail;

    if (packet_queue_init(&vs->video_queue) < 0 ||
            packet_queue_init(&vs->audio_queue) < 0) {
        goto fail;
    }

    vs->play_mutex = SDL_CreateMutex();

    vs->read_thread = SDL_CreateThreadEx(&vs->_read_thread, read_thread, ffPlayer, "ff_read");

    if (!vs->read_thread)
    {
        fail:
        return NULL;
    }
    return vs;
}

int ffp_prepare_async(FFPlayer *ffPlayer, const char* url)
{
    VideoState *videoState = stream_open(ffPlayer, url);
    if (!videoState)
        return YYP_OUT_OF_MEMORY;
    ffPlayer->video = videoState;
    ffPlayer->input_filename = av_strdup(url);
    return 0;
}

void ffp_toggle_buffing_internal(FFPlayer *ff, int start_buffing)
{

}

void ffp_toggle_buffing(FFPlayer *ff, int start_buffing)
{
    SDL_LockMutex(ff->video->play_mutex);
    ffp_toggle_buffing_internal(ff, start_buffing);
    SDL_UnlockMutex(ff->video->play_mutex);
}

void ffp_global_init()
{
    av_register_all();
    av_init_packet(&flush_pkt);
}