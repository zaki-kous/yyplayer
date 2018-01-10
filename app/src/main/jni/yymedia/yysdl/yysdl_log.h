//
// Created by 朱乾 on 17/12/24.
//

#ifndef YYPLAYER_YYSDL_LOG_H
#define YYPLAYER_YYSDL_LOG_H

#define LOG_TAG "yyplyer"

#define LOGI     SDL_LOGI
#define LOGD     SDL_LOGD
#define LOGW     SDL_LOGW
#define LOGE     SDL_LOGE

void SDL_LOGI(const char *log, ...);
void SDL_LOGD(const char *log, ...);
void SDL_LOGW(const char *log, ...);
void SDL_LOGE(const char *log, ...);

#endif //YYPLAYER_YYSDL_LOG_H
