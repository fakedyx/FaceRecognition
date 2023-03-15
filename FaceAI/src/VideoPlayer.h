#pragma once

#include "pch.h"

#ifdef _WIN32
#include <basetsd.h>
typedef SSIZE_T ssize_t;
#endif

#include "vlc/vlc.h"

typedef struct VideoPlayer {
	libvlc_media_player_t* player;
	libvlc_instance_t* instance;
	HWND hwnd; //播放窗口
}video_player_t;

bool videoPlayerInit(video_player_t* player);                 // 视频播放器的初始化
bool videoPlayerPlay(video_player_t* player, char* filePath);   // 播放指定视频
bool videoPlayerPause(video_player_t* player);				// 暂停
void videoPlayerStop(video_player_t* player);
