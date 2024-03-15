#include <fileref.h>
#include <filesystem>

#include "musicplayer_defs.h"
#include "musicplayer_song.h"
#include "musicplayer_state.h"
#include "musicplayer_playlist.h"

std::string& CNyaSong::GetTitle() {
	if (metadata.title[0]) return metadata.title;
	return fileName;
}

void CNyaSong::Play() {
	if (this == &GetCurrentPlaylist()[0] && GetCurrentPlaylist().empty()) return;

	sCurrentSong.Stop();
	if (this != &sCurrentSong.songData) sCurrentSong.songData = *this;
	sCurrentSong.Play();
}

void CNyaSong::ReadMetadata() {
	TagLib::FileRef f(filePath.c_str());
	if (f.isNull()) return;

	metadata.length = f.audioProperties()->lengthInSeconds();
	metadata.bitrate = f.audioProperties()->bitrate();
	metadata.samplerate = f.audioProperties()->sampleRate();

	auto tag = f.tag();
	if (tag->isEmpty()) return;

	if (!tag->title().isEmpty()) metadata.title = tag->title().to8Bit(true);
	if (!tag->artist().isEmpty()) metadata.artist = tag->artist().to8Bit(true);
	if (!tag->album().isEmpty()) metadata.album = tag->album().to8Bit(true);
	if (!tag->genre().isEmpty()) metadata.genre = tag->genre().to8Bit(true);
	metadata.year = tag->year();
}

// -1 - above the viewable area
// 0 - in the viewable area
// 1 - below the viewable area
int CNyaSong::GetPlaylistViewOffset() const {
	auto& songList = GetCurrentPlaylist();

	int i = 1;
	int scrollId = 0;
	for (auto& song : songList) {
		if (scrollId < nPlaylistScroll) {
			if (song.filePath == filePath) return -1;

			scrollId++;
			continue;
		}

		if (i > nSongListNumDisplayed) return 1;
		//if (y >= fPlaylistSelectBottom) return 1;

		if (song.filePath == filePath) return 0;

		i++;
		scrollId++;
	}
	return 0;
}

void SkipSong(int add) {
	bool bShouldPlay = nPlaybackState != STATE_STOPPED;

	auto& songList = GetCurrentPlaylist();

	if (songList.empty()) return;

	sCurrentSong.Stop();

	int i = 0;
	for (auto& song : songList) {
		if (song.filePath == sCurrentSong.songData.filePath) {
			i += add;
			if (i < 0) i = songList.size() - 1;
			if (i >= songList.size()) i = 0;
			if (bShouldPlay) {
				songList[i].Play();
				if (nPlaybackState == STATE_PLAYING) return;
			} else {
				sCurrentSong.songData = songList[i];
				return;
			}
		}
		i++;
	}
}

void EraseSong(int id) {
	auto& songList = GetCurrentPlaylist();
	if (id < 0 || id >= songList.size()) return;

	songList.erase(songList.begin() + id);

	for (int i = id + 1; i < songList.size(); i++) {
		songList[i].songId--;
	}
}

void AddNewSong(const char* path, bool readMetadata, bool lock) {
	if (!std::filesystem::exists(path)) return;

	// dupe check
	for (auto& song : GetCurrentPlaylist())
	{
		if (song.filePath == path) return;
	}

	CNyaSong data;
	data.filePath = path;

	data.fileName = path;
	while (auto id = data.fileName.find_first_of("\\/") != std::string::npos) {
		data.fileName = &data.fileName.c_str()[id];
	}

	if (readMetadata) data.ReadMetadata();

	if (lock) mSongListMutex.lock();
	data.songId = GetCurrentPlaylist().size();
	GetCurrentPlaylist().push_back(data);
	if (lock) mSongListMutex.unlock();
}

// recursive but singlethreaded - need this thing to add songs in a predictable order
void AddSongFolder(const char* path) {
	for (const auto & entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) {
			AddSongFolder(entry.path().string().c_str());
		}
		else if (entry.is_regular_file()) {
			auto filePath = entry.path().string();
			for (auto& extension : aSupportedFormats) {
				if (StringHasEnding(filePath, extension)) {
					AddNewSong(filePath.c_str(), true);
				}
			}
		}
	}
}