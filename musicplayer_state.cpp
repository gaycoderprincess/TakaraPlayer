#include "musicplayer_state.h"
#include "musicplayer_playlist.h"

CActiveSongData sCurrentSong;
ePlaybackState nPlaybackState = STATE_STOPPED;
int nCurrentPlaylist = 0;
float fVolume = 1.0;
bool bVolumeMuted = false;

int nPlaylistScroll = 0;

std::mutex mSongListMutex;

bool CActiveSongData::IsValid() {
	return songData.filePath[0];
}

void CActiveSongData::Play() {
	if (audioHandle = NyaAudio::LoadFile(songData.filePath.c_str())) {
		UpdateVolume();
		NyaAudio::Play(audioHandle);
		nPlaybackState = STATE_PLAYING;

		if (auto song = GetCurrentSongInPlaylist()) {
			song->customMetadata.numListens++;
		}

		while (auto scroll = songData.GetPlaylistViewOffset()) {
			nPlaylistScroll += scroll;
		}
	}
}

void CActiveSongData::Stop() {
	nPlaybackState = STATE_STOPPED;

	if (!audioHandle) return;
	NyaAudio::Stop(audioHandle);
	NyaAudio::Delete(&audioHandle);
}

void CActiveSongData::Pause() {
	nPlaybackState = STATE_PAUSED;
	NyaAudio::Stop(audioHandle);
}

void CActiveSongData::Unpause() {
	nPlaybackState = STATE_PLAYING;
	NyaAudio::Play(audioHandle);
}

void CActiveSongData::UpdateVolume() {
	if (!audioHandle) return;
	if (nPlaybackState != STATE_PLAYING) return;
	NyaAudio::SetVolume(audioHandle, bVolumeMuted ? 0 : fVolume);
}