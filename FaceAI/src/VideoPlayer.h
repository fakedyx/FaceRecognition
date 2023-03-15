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
	HWND hwnd; //���Ŵ���
}video_player_t;

bool videoPlayerInit(video_player_t* player);                 // ��Ƶ�������ĳ�ʼ��
bool videoPlayerPlay(video_player_t* player, char* filePath);   // ����ָ����Ƶ
bool videoPlayerPause(video_player_t* player);				// ��ͣ
void videoPlayerStop(video_player_t* player);
