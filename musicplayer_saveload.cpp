#include <fstream>

#include "musicplayer_state.h"
#include "musicplayer_playlist.h"
#include "musicplayer_saveload.h"

void SaveAllToDisk() {
	std::ofstream file;
	file.open(sProgramDir + "/save.dat", std::ios::binary);
	if (!file.is_open()) return;

	file.write("nya~", 4);
	file.write((char*)&nSaveVersion, sizeof(nSaveVersion));

	file.write((char*)&nCurrentPlaylist, sizeof(nCurrentPlaylist));
	WriteStringToFile(file, sCurrentSong.songData.filePath.c_str());

	int num = aPlaylists.size();
	file.write((char*)&num, sizeof(num));

	for (auto& playlist : aPlaylists) {
		WriteStringToFile(file, playlist.playlistName.c_str());
		num = playlist.aSongs.size();
		file.write((char*)&num, sizeof(num));
		for (auto& song : playlist.aSongs) {
			WriteStringToFile(file, song.filePath.c_str());
			WriteStringToFile(file, song.fileName.c_str());
			file.write((char*)&song.songId, sizeof(song.songId));
			WriteStringToFile(file, song.metadata.title.c_str());
			WriteStringToFile(file, song.metadata.artist.c_str());
			WriteStringToFile(file, song.metadata.album.c_str());
			file.write((char*)&song.metadata.length, sizeof(song.metadata.length));
			WriteStringToFile(file, song.metadata.genre.c_str());
			file.write((char*)&song.metadata.year, sizeof(song.metadata.year));
			file.write((char*)&song.metadata.bitrate, sizeof(song.metadata.bitrate));
			file.write((char*)&song.metadata.samplerate, sizeof(song.metadata.samplerate));
			file.write((char*)&song.customMetadata.numListens, sizeof(song.customMetadata.numListens));
			file.write((char*)&song.customMetadata.numFullListens, sizeof(song.customMetadata.numFullListens));
		}
	}
	file.close();
}

bool ReadAllFromDisk() {
	std::ifstream file;
	file.open(sProgramDir + "/save.dat", std::ios::binary);
	if (!file.is_open()) return false;

	char header[4];
	file.read(header, sizeof(header));
	if (memcmp(header, "nya~", sizeof(header)) != 0) return false;

	int nVersion = 0;
	file.read((char*)&nVersion, sizeof(nVersion));
	if (nVersion > nSaveVersion) return false;

	file.read((char*)&nCurrentPlaylist, sizeof(nCurrentPlaylist));
	auto currSongPath = ReadStringFromFile(file);

	int numPlaylists = 0;
	file.read((char*)&numPlaylists, sizeof(numPlaylists));
	if (numPlaylists <= 0) return false;

	for (int i = 0; i < numPlaylists; i++) {
		CNyaPlaylist playlist;
		playlist.playlistName = ReadStringFromFile(file);

		int numSongs = 0;
		file.read((char*)&numSongs, sizeof(numSongs));

		for (int j = 0; j < numSongs; j++) {
			CNyaSong song;
			song.filePath = ReadStringFromFile(file);
			song.fileName = ReadStringFromFile(file);
			file.read((char*)&song.songId, sizeof(song.songId));
			song.metadata.title = ReadStringFromFile(file);
			song.metadata.artist = ReadStringFromFile(file);
			song.metadata.album = ReadStringFromFile(file);
			file.read((char*)&song.metadata.length, sizeof(song.metadata.length));
			song.metadata.genre = ReadStringFromFile(file);
			file.read((char*)&song.metadata.year, sizeof(song.metadata.year));
			file.read((char*)&song.metadata.bitrate, sizeof(song.metadata.bitrate));
			file.read((char*)&song.metadata.samplerate, sizeof(song.metadata.samplerate));
			file.read((char*)&song.customMetadata.numListens, sizeof(song.customMetadata.numListens));
			file.read((char*)&song.customMetadata.numFullListens, sizeof(song.customMetadata.numFullListens));
			playlist.aSongs.push_back(song);
		}

		aPlaylists.push_back(playlist);
	}

	if (currSongPath[0]) {
		for (auto& song : GetCurrentPlaylist()) {
			if (song.filePath == currSongPath) {
				sCurrentSong.songData = song;
				break;
			}
		}
	}

	return true;
}