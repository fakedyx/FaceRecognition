#include "pch.h"
#include "VideoPlayer.h"

bool videoPlayerInit(video_player_t* player) {
	if (!player) return false;
	player->instance = libvlc_new(0, nullptr);
	return player->instance != NULL;
}

bool videoPlayerPlay(video_player_t* player, char* filePath)
{
	bool bRet = false;

	if (!player || !filePath || strlen(filePath) == 0)
		return false;

	if (player->instance == NULL) {
		videoPlayerInit(player);
		return false;
	}

	libvlc_media_t* pVlcMedia = libvlc_media_new_path(player->instance, filePath);
	if (pVlcMedia == NULL) {
		return false;
	}

	player->player = libvlc_media_player_new_from_media(pVlcMedia);
	if (player->player) {
		// 设置播放窗口
		libvlc_media_player_set_hwnd(player->player, player->hwnd);

		// 播放
		if (libvlc_media_player_play(player->player) != -1)
			bRet = true;

		libvlc_media_release(pVlcMedia);
	}

	return bRet;
}

bool videoPlayerPause(video_player_t* player) {
	if (!player || !player->player) {
		return false;
	}

	if (libvlc_media_player_can_pause(player->player)) {
		libvlc_media_player_pause(player->player);
		return true;
	}

	return false;
}

void videoPlayerStop(video_player_t* player) {
	libvlc_media_player_stop(player->player);
	libvlc_media_player_release(player->player);
	player->player = nullptr;
}