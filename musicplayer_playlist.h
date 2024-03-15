#ifndef MUSICPLAYER_PLAYLIST_H
#define MUSICPLAYER_PLAYLIST_H

#include "musicplayer_song.h"

class CNyaPlaylist {
public:
	std::string playlistName;
	std::vector<CNyaSong> aSongs;

	int GetTotalLength();
};
extern std::vector<CNyaPlaylist> aPlaylists;

void AddPlaylist(const char* name);
std::vector<CNyaSong>& GetCurrentPlaylist();
CNyaPlaylist& GetCurrentPlaylistInfo();
void SwitchToPlaylist(int id);
void ErasePlaylist(int id);
void ShufflePlaylist();
CNyaSong* GetCurrentSongInPlaylist();
#endif