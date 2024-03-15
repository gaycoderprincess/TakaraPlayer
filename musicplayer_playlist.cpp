#include <algorithm>
#include "musicplayer_playlist.h"
#include "musicplayer_state.h"

std::vector<CNyaPlaylist> aPlaylists;

int CNyaPlaylist::GetTotalLength() {
	int len = 0;
	for (auto& song : aSongs) {
		len += song.metadata.length;
	}
	return len;
}

void AddPlaylist(const char* name) {
	CNyaPlaylist playlist;
	playlist.playlistName = name;
	aPlaylists.push_back(playlist);
}

CNyaPlaylist& GetCurrentPlaylistInfo() {
	return aPlaylists[nCurrentPlaylist];
}

std::vector<CNyaSong>& GetCurrentPlaylist() {
	return GetCurrentPlaylistInfo().aSongs;
}

void SwitchToPlaylist(int id) {
	if (nCurrentPlaylist != id) nPlaylistScroll = 0;
	nCurrentPlaylist = id;
}

void ErasePlaylist(int id) {
	if (id <= 0 || id >= aPlaylists.size()) return;

	aPlaylists.erase(aPlaylists.begin() + id);
	if (nCurrentPlaylist == id) SwitchToPlaylist(0);
}

bool ShuffleComp(CNyaSong& a, CNyaSong& b) {
	return a.songId < b.songId;
}

void ShufflePlaylist() {
	bool bIsShuffled = false;

	auto& playlist = GetCurrentPlaylist();
	for (auto& song : playlist) {
		if (song.songId != &song - &playlist[0]) {
			bIsShuffled = true;
			break;
		}
	}

	if (bIsShuffled) {
		// re-sort in songId order
		std::sort(playlist.begin(), playlist.end(), ShuffleComp);
	} else {
		std::vector<CNyaSong> aSongs = playlist;
		playlist.clear();

		while (!aSongs.empty()) {
			int r = rand() % aSongs.size();
			playlist.push_back(aSongs[r]);
			aSongs.erase(aSongs.begin() + r);
		}
	}
}

CNyaSong* GetCurrentSongInPlaylist() {
	for (auto& song : GetCurrentPlaylist()) {
		if (song.filePath == sCurrentSong.songData.filePath) {
			return &song;
		}
	}
	return nullptr;
}