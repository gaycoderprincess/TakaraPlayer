#ifndef MUSICPLAYER_STATE_H
#define MUSICPLAYER_STATE_H
#include <mutex>

#include "musicplayer_song.h"

class CActiveSongData {
public:
	CNyaSong songData;
	NyaAudio::NyaSound audioHandle = 0;

	bool IsValid();
	void Play();
	void Stop();
	void Pause();
	void Unpause();
	void UpdateVolume();
};
extern CActiveSongData sCurrentSong;

enum ePlaybackState {
	STATE_STOPPED,
	STATE_PLAYING,
	STATE_PAUSED,
};
extern ePlaybackState nPlaybackState;

extern int nCurrentPlaylist;
extern float fVolume;
extern bool bVolumeMuted;

extern int nPlaylistScroll;

extern std::mutex mSongListMutex;
#endif