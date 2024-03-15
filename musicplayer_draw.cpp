#include <format>
#include <filesystem>
#include "musicplayer_defs.h"
#include "musicplayer_song.h"
#include "musicplayer_state.h"
#include "musicplayer_playlist.h"
#include "musicplayer_saveload.h"
#include "musicplayer_draw.h"

bool IsAnySongActive() {
	return sCurrentSong.audioHandle && nPlaybackState != STATE_STOPPED;
}

void OpenAddSongDialog() {
	const size_t maxFile = 32767;

	OPENFILENAME ofn={0};
	char szFile[maxFile + 1] = {0};
	char szDirect[maxFile + 1] = {0};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = ghWnd;
	ofn.lpstrFile = szDirect;
	*(ofn.lpstrFile) = 0;
	ofn.nMaxFile = maxFile;

	ofn.lpstrFilter = "All Files\0*.*\0Audio\0*.mp3;*.wav;*.flac;*.ogg\0";

	ofn.nFilterIndex = 2;
	ofn.lpstrFileTitle = szFile;
	*(ofn.lpstrFileTitle) = 0;
	ofn.nMaxFileTitle = sizeof(szFile);
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;

	if (GetOpenFileName(&ofn) == TRUE) {
		char *ptr = ofn.lpstrFile;
		ptr[ofn.nFileOffset - 1] = 0;

		std::string dirPath = ptr;

		ptr += ofn.nFileOffset;

		while (*ptr) {
			auto filePath = dirPath + "\\" + ptr;
			if (std::filesystem::is_directory(filePath)) {
				AddSongFolder(filePath.c_str());
			}
			else {
				AddNewSong(filePath.c_str(), true, false);
			}
			ptr += (lstrlen(ptr) + 1);
		}

		SaveAllToDisk();
	}

	ShowCursor(0);
}

namespace AddPlaylistDialog {
	bool bActive = false;
	bool bFirstFrame = false;
	bool bTextBoxHighlighted = false;
	std::string sInputText;

	void Open() {
		bActive = true;
		bTextBoxHighlighted = true;
		bFirstFrame = true;
		sInputText = "";
	}

	void Draw() {
		if (AddPlaylistDialog::bActive && !AddPlaylistDialog::bFirstFrame) {
			// darken background
			DrawRectangle(0, 1, 0, 1, {0, 0, 0, 127}, 0, nullptr);

			DrawRectangle(fDialogLeft, fDialogRight, fDialogTop, fDialogBottom, aColors[COLOR_LIGHT_PINK], 0.02, nullptr);

			bool bHover = false;
			if (DrawRectangle(fDialogCloseX - fDialogCloseSizeX, fDialogCloseX + fDialogCloseSizeX, fDialogCloseY - fDialogCloseSizeY,
							  fDialogCloseY + fDialogCloseSizeY, {255, 255, 255, 0})) {
				bHover = true;
				if (IsKeyJustPressed(VK_LBUTTON)) {
					bActive = false;
				}
			}

			tNyaStringData text;
			text.x = fDialogCloseX;
			text.y = fDialogCloseY;
			text.size = fDialogCloseSizeY * 4;
			text.XCenterAlign = true;
			text.SetColor(&aColors[bHover ? COLOR_GRAY : COLOR_BROWN]);
			DrawString(text, "X");

			text.size = 0.025;
			text.x = fDialogPlaylistNameTextX;
			text.y = fDialogPlaylistNameTextY;
			text.XCenterAlign = false;
			text.SetColor(&aColors[COLOR_BROWN]);
			DrawString(text, "Playlist name:");

			bool bLMBPressed = IsKeyJustPressed(VK_LBUTTON);
			if (DrawRectangle(fDialogInputLeft, fDialogInputRight, fDialogInputTop, fDialogInputBottom, aColors[COLOR_PURE_WHITE]) && bLMBPressed) {
				bTextBoxHighlighted = true;
			}
			else if (bLMBPressed) bTextBoxHighlighted = false;

			if (bTextBoxHighlighted) {
				sInputText += sKeyboardInput;
				if (IsKeyJustPressed(VK_BACK)) sInputText.pop_back();
			}

			text.x = fDialogInputLeft + 0.005;
			text.y = (fDialogInputTop + fDialogInputBottom) * 0.5;
			text.clipMinX = fDialogInputLeft;
			text.clipMinY = fDialogInputTop;
			text.clipMaxX = fDialogInputRight;
			text.clipMaxY = fDialogInputBottom;
			std::string inputStr = sInputText;
			if (bTextBoxHighlighted) inputStr += "|";
			DrawString(text, inputStr.c_str());

			text.x = 0.5;
			text.y = fDialogBottom - fDialogTextSpacingY;
			text.size = 0.03;
			text.XCenterAlign = true;
			text.clipMinX = 0;
			text.clipMinY = 0;
			text.clipMaxX = 1;
			text.clipMaxY = 1;

			auto width = GetStringWidth(text.size, "Add Playlist") * 0.5;
			auto height = GetStringHeight(text.size, "Add Playlist") * 0.5;
			bHover = DrawRectangle(0.5 - width - fDialogButtonTextSpacingX, 0.5 + width + fDialogButtonTextSpacingX, text.y - height - fDialogButtonTextSpacingY, text.y + height + fDialogButtonTextSpacingY, aColors[COLOR_BROWN], 0.02);

			if (((bHover && bLMBPressed) || (bTextBoxHighlighted && IsKeyJustPressed(VK_RETURN))) && sInputText[0]) {
				AddPlaylist(sInputText.c_str());
				bActive = false;
			}

			text.SetColor(&aColors[bHover ? COLOR_GRAY : COLOR_PURE_WHITE]);
			DrawString(text, "Add Playlist");

		}
		bFirstFrame = false;
	}
}

bool IsMainWindowInputBlocked() {
	if (AddPlaylistDialog::bActive) return true;
	return false;
}

void ProcessPlayer() {
	if (!IsMainWindowInputBlocked() && IsKeyJustPressed(VK_SPACE)) {
		switch (nPlaybackState) {
			case STATE_PLAYING:
				sCurrentSong.Pause();
				break;
			case STATE_PAUSED:
				sCurrentSong.Unpause();
				break;
			case STATE_STOPPED:
				if (sCurrentSong.IsValid()) sCurrentSong.Play();
				else GetCurrentPlaylist()[0].Play();
				break;
			default:
				break;
		}
	}

	if (!IsMainWindowInputBlocked() && IsKeyJustPressed(VK_UP)) {
		fVolume += 0.05;
		if (fVolume >= fMaxVolume) fVolume = fMaxVolume;
		sCurrentSong.UpdateVolume();
	}
	if (!IsMainWindowInputBlocked() && IsKeyJustPressed(VK_DOWN)) {
		fVolume -= 0.05;
		if (fVolume <= 0) fVolume = 0;
		sCurrentSong.UpdateVolume();
	}

	if (sCurrentSong.audioHandle && nPlaybackState == STATE_PLAYING) {
		if (NyaAudio::IsFinishedPlaying(sCurrentSong.audioHandle)) {
			if (auto song = GetCurrentSongInPlaylist()) {
				song->customMetadata.numFullListens++;
			}
			SkipSong(1);
		}
	}
}

void DrawMouseCursor() {
	auto x = GetMousePosX();
	auto y = GetMousePosY();
	float sizeX = 32 / (double)nResX;
	float sizeY = 32 / (double)nResY;

	static auto pTexture = LoadTexture("cursor.png");

	DrawRectangle(x, x + sizeX, y, y + sizeY, aColors[COLOR_PURE_WHITE], 0, pTexture);
}

bool IsScrollbarRequired() {
	return GetCurrentPlaylist().size() > nSongListNumDisplayed;
}

bool DrawSongInfo(double id, float size, bool isCurrent, const char* title, const char* artist, const char* year, const char* duration, const char* trackId, const char* numListens) {
	float y = fCurrentPlaylistTop + fPlaylistTextInitialSpacing;
	y += fPlaylistTextSpacing * id;

	if (id > nSongListNumDisplayed) return false;
	//if (y >= fPlaylistSelectBottom) return false;

	float rectRight = 1 - fBorderDistX;
	if (IsScrollbarRequired()) rectRight -= (fScrollbarSizeX + fScrollbarOffsetX) * GetAspectRatioInv();

	tNyaStringData text;
	text.x = fPlaylistSelectRight + fBorderDistX + fPlaylistTextInitialXSpacing;
	text.y = y;
	text.size = 0.02 * size;
	text.YCenterAlign = true;
	text.SetColor(&aColors[isCurrent ? COLOR_GRAY : COLOR_PURE_WHITE]);
	text.clipMaxX = rectRight;
	text.XCenterAlign = true;

	DrawString(text, CapStringToWidth(trackId, text.size, fPlaylistTextXSpacing * 0.1).c_str());
	text.x += fPlaylistTextXSpacing * 0.1;

	text.XCenterAlign = false;
	DrawString(text, CapStringToWidth(title, text.size, fPlaylistTextXSpacing * 1.5).c_str());

	text.x += fPlaylistTextXSpacing * 1.4;
	DrawString(text, CapStringToWidth(artist, text.size, fPlaylistTextXSpacing * 1.4).c_str());
	text.x += fPlaylistTextXSpacing * 1.35;
	DrawString(text, year);
	text.x += fPlaylistTextXSpacing * 0.5;
	DrawString(text, duration);
	text.x += fPlaylistTextXSpacing * 0.5;
	DrawString(text, numListens);

	if (id >= 0 && DrawRectangle(fPlaylistSelectRight + fBorderDistX, rectRight, y - fSelectionRectSize, y + fSelectionRectSize, {0, 0, 0, 0})) {
		if (!IsMainWindowInputBlocked()) {
			DrawRectangle(fPlaylistSelectRight + fBorderDistX, rectRight, y - fSelectionRectSize, y + fSelectionRectSize, {127, 127, 127, 127});
			return true;
		}
	}
	return false;
}

void DrawCurrentPlaylist() {
	DrawSongInfo(-0.15, 2, false, "Title", "Artist", "Year", "Length", "#", "Listens");

	bool bSongChanged = false;

	auto& songList = GetCurrentPlaylist();

	int i = 1;
	int scrollId = 0;
	for (auto& song : songList) {
		if (scrollId < nPlaylistScroll) {
			scrollId++;
			continue;
		}

		if (i > nSongListNumDisplayed) return;

		if (DrawSongInfo(i, 1, song.filePath == sCurrentSong.songData.filePath, song.GetTitle().c_str(), song.metadata.artist.c_str(), song.metadata.year ? std::to_string(song.metadata.year).c_str() : "", GetTimeFromSeconds(song.metadata.length).c_str(), std::to_string(song.songId + 1).c_str(), std::to_string(song.customMetadata.numListens).c_str())) {
			if (!bSongChanged && IsKeyJustPressed(VK_LBUTTON)) {
				song.Play();
				bSongChanged = true;
			}
			if (IsKeyJustPressed(VK_MBUTTON)) {
				EraseSong(scrollId);
				break;
			}
		}

		i++;
		scrollId++;
	}
}

void CapScrollbar() {
	auto max = GetCurrentPlaylist().size();

	if (nPlaylistScroll < 0) nPlaylistScroll = 0;
	if (nPlaylistScroll + nSongListNumDisplayed >= max) nPlaylistScroll = max - nSongListNumDisplayed;
}

void DrawScrollbar() {
	static bool bScrollbarHeld = false;

	float fMaxScroll = GetCurrentPlaylist().size() - nSongListNumDisplayed;

	float fScrollbarPos = nPlaylistScroll / fMaxScroll;
	float fFinalScrollbarPos = std::lerp(fScrollbarTop + fScrollbarYSize, fScrollbarBottom - fScrollbarYSize, fScrollbarPos);

	// background
	if ((DrawRectangle(fFinalScrollbarLeft, fFinalScrollbarRight, fScrollbarTop, fScrollbarBottom, aColors[COLOR_WHITE], 0.02) && IsKeyPressed(VK_LBUTTON)) || bScrollbarHeld) {
		bScrollbarHeld = true;

		auto y = GetMousePosY();
		y -= fScrollbarTop + fScrollbarYSize;
		y /= (fScrollbarBottom - fScrollbarTop) - fScrollbarYSize;
		y *= fMaxScroll;
		nPlaylistScroll = y + 1;
		CapScrollbar();
	}

	if (!IsKeyPressed(VK_LBUTTON)) bScrollbarHeld = false;

	// scrollbar
	DrawRectangle(fFinalScrollbarLeft, fFinalScrollbarRight, fFinalScrollbarPos - fScrollbarYSize, fFinalScrollbarPos + fScrollbarYSize, aColors[COLOR_GRAY], 0.02);
}

bool DrawPlaylistInfo(int id, const char* name) {
	tNyaStringData text;
	text.x = fPlaylistSelectTextLeft;
	text.y = fPlaylistSelectTextTop + id * fPlaylistSelectTextSpacing;
	text.size = 0.03;
	text.XCenterAlign = false;
	text.YCenterAlign = true;
	text.SetColor(&aColors[COLOR_PURE_WHITE]);
	DrawString(text, name);
	if (DrawRectangle(fPlaylistSelectLeft, fPlaylistSelectRight, text.y - fPlaylistSelectHighlightSpacing,
					  text.y + fPlaylistSelectHighlightSpacing, {0, 0, 0, 0})) {
		if (!IsMainWindowInputBlocked()) {
			DrawRectangle(fPlaylistSelectLeft, fPlaylistSelectRight, text.y - fPlaylistSelectHighlightSpacing,
						  text.y + fPlaylistSelectHighlightSpacing, {127, 127, 127, 127});
			return true;
		}
	}
	return false;
}

void DrawPlaylistSelect() {
	tNyaStringData text;
	text.x = fPlaylistSelectTextMidX;
	text.y = fPlaylistSelectTextHeaderY;
	text.size = 0.05;
	text.XCenterAlign = true;
	text.YCenterAlign = true;
	text.SetColor(&aColors[COLOR_PURE_WHITE]);

	DrawString(text, "Playlists");

	int i = 0;
	for (auto& playlist : aPlaylists) {
		if (DrawPlaylistInfo(i, playlist.playlistName.c_str())) {
			if (IsKeyJustPressed(VK_LBUTTON)) {
				SwitchToPlaylist(i);
			}
			if (IsKeyJustPressed(VK_MBUTTON)) {
				ErasePlaylist(i);
				break;
			}
		}
		i++;
	}
}

void DrawPlayerUI() {
	// base background
	DrawRectangle(0,1,0,1,aColors[COLOR_BLACK]);

	// playlists display on the left
	static auto pPlaylistsTex = LoadTexture("nyaTex.png");
	DrawRectangle(fBorderDistX, fPlaylistSelectRight, fBorderDistY, fPlaylistSelectBottom, aColors[COLOR_PURE_WHITE], 0.02, pPlaylistsTex);

	// song display on the bottom
	DrawRectangle(0, 1, fSongDisplayTop, 1, aColors[COLOR_LAVENDER]);

	// song display
	if (IsAnySongActive()) {
		// space for album art, TODO
		DrawRectangle(fBorderDistX, fSongArtRight, fSongDisplayTop + fBorderDistY, fSongDisplayTop + fBorderDistY + fSongArtSizeY, {0, 0, 0, 255});

		tNyaStringData text;
		text.x = fSongArtRight + fBorderDistX;
		text.y = fSongDisplayTop + fBorderDistY;
		text.size = 0.025;
		//text.XCenterAlign = true;
		text.YCenterAlign = false;
		text.SetColor(&aColors[COLOR_BLACK]);

		DrawString(text, sCurrentSong.songData.GetTitle().c_str());

		text.y += fSongDisplayYSpacing;
		text.size = 0.02;
		DrawString(text, CapStringToWidth(sCurrentSong.songData.metadata.artist.c_str(), text.size, fProgressBarLeft - text.x).c_str());
	}

	// progress bar
	{
		// click to skip
		if (DrawRectangle(fProgressBarLeft, fProgressBarRight, fProgressBarPosY - fProgressBarSizeY, fProgressBarPosY + fProgressBarSizeY, aColors[COLOR_GRAY], 0.02) && IsKeyJustPressed(VK_LBUTTON) && IsAnySongActive()) {
			if (!IsMainWindowInputBlocked()) {
				auto x = GetMousePosX();
				x -= fProgressBarLeft;
				x /= fProgressBarRight - fProgressBarLeft;
				x *= NyaAudio::GetLength(sCurrentSong.audioHandle);
				NyaAudio::SkipTo(sCurrentSong.audioHandle, x);
			}
		}

		if (IsAnySongActive()) {
			float progress = (double) NyaAudio::GetPosition(sCurrentSong.audioHandle) /
							 (double) NyaAudio::GetLength(sCurrentSong.audioHandle);
			DrawRectangle(fProgressBarLeft, std::lerp(fProgressBarLeft, fProgressBarRight, progress), fProgressBarPosY - fProgressBarSizeY, fProgressBarPosY + fProgressBarSizeY, aColors[COLOR_WHITE], 0.02);
		}
	}

	// volume bar
	{
		// click to skip
		if (DrawRectangle(fVolumeBarLeft, fVolumeBarRight, fVolumeBarPosY - fVolumeBarSizeY, fVolumeBarPosY + fVolumeBarSizeY, aColors[COLOR_GRAY], 0.02) && IsKeyPressed(VK_LBUTTON) && IsAnySongActive()) {
			auto x = GetMousePosX();
			x -= fVolumeBarLeft;
			x /= fVolumeBarRight - fVolumeBarLeft;
			fVolume = x * fMaxVolume;
			bVolumeMuted = false;
			sCurrentSong.UpdateVolume();
		}

		if (!bVolumeMuted && fVolume > 0) DrawRectangle(fVolumeBarLeft, std::lerp(fVolumeBarLeft, fVolumeBarRight, fVolume / fMaxVolume), fVolumeBarPosY - fVolumeBarSizeY, fVolumeBarPosY + fVolumeBarSizeY, aColors[COLOR_WHITE], 0.02);

		// volume/mute button
		static auto pVolumeTexture = LoadTexture("volume.png");
		static auto pVolumeMuteTexture = LoadTexture("volumeMute.png");
		float x = fVolumeBarLeft - 0.015 * GetAspectRatioInv();
		if (DrawRectangle(x - fPlayerControlsSizeX, x + fPlayerControlsSizeX, fVolumeBarPosY - fPlayerControlsSizeY, fVolumeBarPosY + fPlayerControlsSizeY, aColors[COLOR_WHITE], 0, bVolumeMuted ? pVolumeMuteTexture : pVolumeTexture) && IsKeyJustPressed(VK_LBUTTON)) {
			bVolumeMuted = !bVolumeMuted;
			sCurrentSong.UpdateVolume();
		}
	}

	// player controls
	{
		float fPlayerControlsBackgroundLeft = fPlayerControlsMidX - fPlayerControlsSpacingX * 3;
		float fPlayerControlsBackgroundRight = fPlayerControlsMidX + fPlayerControlsSpacingX * 2;
		float fPlayerControlsBackgroundSizeY = fPlayerControlsSizeY * 1.5;
		DrawRectangle(fPlayerControlsBackgroundLeft, fPlayerControlsBackgroundRight,  fPlayerControlsMidY - fPlayerControlsBackgroundSizeY,  fPlayerControlsMidY + fPlayerControlsBackgroundSizeY, {0,0,0,127}, 0.02);

		static auto pPlayTexture = LoadTexture("play.png");
		static auto pPauseTexture = LoadTexture("pause.png");
		static auto pShuffleTexture = LoadTexture("shuffle.png");
		static auto pSkipTexture = LoadTexture("skip.png");

		// pause/unpause/play
		if (nPlaybackState == STATE_PAUSED) {
			if (!IsMainWindowInputBlocked() && DrawRectangle(fPlayerControlsMidX - fPlayerControlsSizeX, fPlayerControlsMidX + fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, {255, 255, 255, 0}) && IsKeyJustPressed(VK_LBUTTON)) {
				sCurrentSong.Unpause();
			}
			DrawRectangle(fPlayerControlsMidX - fPlayerControlsSizeX, fPlayerControlsMidX + fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, aColors[COLOR_WHITE], 0, pPlayTexture);
		}
		else if (nPlaybackState == STATE_PLAYING) {
			if (!IsMainWindowInputBlocked() && DrawRectangle(fPlayerControlsMidX - fPlayerControlsSizeX, fPlayerControlsMidX + fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, {255, 255, 255, 0}) && IsKeyJustPressed(VK_LBUTTON)) {
				sCurrentSong.Pause();
			}
			DrawRectangle(fPlayerControlsMidX - fPlayerControlsSizeX, fPlayerControlsMidX + fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, aColors[COLOR_WHITE], 0, pPauseTexture);
		}
		else if (nPlaybackState == STATE_STOPPED) {
			if (!IsMainWindowInputBlocked() && DrawRectangle(fPlayerControlsMidX - fPlayerControlsSizeX, fPlayerControlsMidX + fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, {255, 255, 255, 0}) && IsKeyJustPressed(VK_LBUTTON)) {
				if (sCurrentSong.IsValid()) sCurrentSong.Play();
				else GetCurrentPlaylist()[0].Play();
			}
			DrawRectangle(fPlayerControlsMidX - fPlayerControlsSizeX, fPlayerControlsMidX + fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, aColors[COLOR_WHITE], 0, pPlayTexture);
		}

		// skip buttons
		float x = fPlayerControlsMidX - fPlayerControlsSpacingX;
		if (DrawRectangle(x + fPlayerControlsSizeX, x - fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, aColors[COLOR_WHITE], 0, pSkipTexture) && IsKeyJustPressed(VK_LBUTTON)) {
			SkipSong(-1);
		}
		x = fPlayerControlsMidX + fPlayerControlsSpacingX;
		if (DrawRectangle(x - fPlayerControlsSizeX, x + fPlayerControlsSizeX, fPlayerControlsMidY - fPlayerControlsSizeY, fPlayerControlsMidY + fPlayerControlsSizeY, aColors[COLOR_WHITE], 0, pSkipTexture) && IsKeyJustPressed(VK_LBUTTON)) {
			SkipSong(1);
		}

		// shuffle button
		x = fPlayerControlsMidX - fPlayerControlsSpacingX * 2;
		float shuffleSize = fPlayerControlsSizeY * 1.5 * GetAspectRatioInv();
		float sizeY = fPlayerControlsSizeY * 1.05;
		if (DrawRectangle(x - shuffleSize, x + shuffleSize, fPlayerControlsMidY - sizeY, fPlayerControlsMidY + sizeY, aColors[COLOR_WHITE], 0, pShuffleTexture) && IsKeyJustPressed(VK_LBUTTON)) {
			ShufflePlaylist();
		}
	}

	// current playlist, right extending
	if (DrawRectangle(fPlaylistSelectRight + fBorderDistX, 1 - fBorderDistX, fCurrentPlaylistTop, fPlaylistSelectBottom, aColors[COLOR_BROWN], 0.02)) {
		// scrollable if hovered over
		auto mWheel = GetMouseWheelState();
		if (mWheel && GetCurrentPlaylist().size() > nSongListNumDisplayed) {
			nPlaylistScroll -= mWheel;
			CapScrollbar();
		}
	}

	// top
	DrawRectangle(fTopBarLeft, fTopBarRight, fTopBarTop, fTopBarBottom, aColors[COLOR_LIGHT_PINK], 0.02);

	// add songs button
	{
		bool bHover = false;
		if (DrawRectangle(fAddSongX - fAddSongSizeX, fAddSongX + fAddSongSizeX, fAddSongY - fAddSongSizeY,
						  fAddSongY + fAddSongSizeY, {255, 255, 255, 0})) {
			bHover = true;
			if (!IsMainWindowInputBlocked() && IsKeyJustPressed(VK_LBUTTON)) {
				OpenAddSongDialog();
			}
		}

		tNyaStringData text;
		text.x = fAddSongX;
		text.y = fAddSongY;
		text.size = fAddSongSizeY * 4;
		text.XCenterAlign = true;
		text.SetColor(&aColors[bHover ? COLOR_GRAY : COLOR_BROWN]);
		DrawString(text, "+");
	}

	// add playlists button
	if (aPlaylists.size() < nPlaylistNumDisplayed)
	{
		bool bHover = false;
		if (DrawRectangle(fAddPlaylistX - fAddPlaylistSizeX, fAddPlaylistX + fAddPlaylistSizeX, fAddPlaylistY - fAddPlaylistSizeY,
						  fAddPlaylistY + fAddPlaylistSizeY, {255, 255, 255, 0})) {
			bHover = true;
			if (!IsMainWindowInputBlocked() && IsKeyJustPressed(VK_LBUTTON)) {
				AddPlaylistDialog::Open();
			}
		}

		tNyaStringData text;
		text.x = fAddPlaylistX;
		text.y = fAddPlaylistY;
		text.size = fAddPlaylistSizeY * 4;
		text.XCenterAlign = true;
		text.SetColor(&aColors[bHover ? COLOR_GRAY : COLOR_BROWN]);
		DrawString(text, "+");
	}

	static auto pTexture = LoadTexture("logo.png");
	if (pTexture) DrawRectangle(fLogoLeft, fLogoLeft + fLogoSizeX, fLogoTop, fLogoTop + fLogoSizeY, aColors[COLOR_PURE_WHITE], 0, pTexture);

	// playlist info
	{
		//float fPlaylistStatsX = fLogoLeft + fLogoSizeX + 0.03 * GetAspectRatioInv();
		float fPlaylistStatsX = (fLogoLeft + fLogoSizeX + fTopBarRight) * 0.5;
		float fPlaylistStatsY = fTopBarBottom + 0.02;

		tNyaStringData text;
		text.x = fPlaylistStatsX;
		text.y = fPlaylistStatsY;
		text.size = 0.05;
		text.XCenterAlign = true;
		text.YBottomAlign = true;
		text.SetColor(&aColors[COLOR_BROWN]);

		auto playlist = GetCurrentPlaylistInfo();
		DrawString(text,std::format("{} Songs, {}", playlist.aSongs.size(), GetTimeFromSeconds(playlist.GetTotalLength())).c_str());
	}

	DrawCurrentPlaylist();

	DrawPlaylistSelect();

	if (IsScrollbarRequired()) DrawScrollbar();

	AddPlaylistDialog::Draw();

	DrawMouseCursor();
}