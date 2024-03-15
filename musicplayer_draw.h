#ifndef MUSICPLAYER_DRAW_H
#define MUSICPLAYER_DRAW_H
#include <string>

bool IsAnySongActive();
void OpenAddSongDialog();

namespace AddPlaylistDialog {
	extern bool bActive;
	extern bool bTextBoxHighlighted;
	extern bool bFirstFrame;
	extern std::string sInputText;
	void Open();
}

bool IsMainWindowInputBlocked();
void ProcessPlayer();
void DrawMouseCursor();
bool IsScrollbarRequired();
bool DrawSongInfo(double id, float size, bool isCurrent, const char* title, const char* artist, const char* year, const char* duration, const char* trackId, const char* numListens);
void DrawCurrentPlaylist();
void CapScrollbar();
void DrawScrollbar();
bool DrawPlaylistInfo(int id, const char* name);
void DrawPlaylistSelect();
void DrawPlayerUI();
#endif