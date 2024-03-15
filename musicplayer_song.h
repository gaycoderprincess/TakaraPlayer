#ifndef MUSICPLAYER_SONG_H
#define MUSICPLAYER_SONG_H

#include <string>
#include "nya_dx11_appbase.h"

class CNyaSong {
public:
	std::string filePath;
	std::string fileName;
	int songId = 0;
	struct {
		std::string title;
		std::string artist;
		std::string album;
		int length = 0;
		std::string genre;
		unsigned int year = 0;
		int bitrate = 0;
		int samplerate = 0;
	} metadata;
	struct {
		int numListens = 0;
		int numFullListens = 0;
	} customMetadata;

	std::string& GetTitle();
	void Play();
	void ReadMetadata();
	int GetPlaylistViewOffset() const;
};

void AddNewSong(const char* path, bool readMetadata = true, bool lock = true);
void AddSongFolder(const char* path);
void SkipSong(int add);
void EraseSong(int id);
#endif