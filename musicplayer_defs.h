#ifndef MUSICPLAYER_DEFS_H
#define MUSICPLAYER_DEFS_H
#include "nya_dx11_appbase.h"

static const char* aSupportedFormats[] = {
		".mp3",
		".wav",
		".flac",
		".ogg"
};
const float fMaxVolume = 1.5;

enum PlayerColor
{
	COLOR_BLACK,
	COLOR_GRAY,
	COLOR_BROWN,
	COLOR_LIGHT_PINK,
	COLOR_DARK_PINK,
	COLOR_LAVENDER,
	COLOR_WHITE,
	COLOR_PURE_WHITE,
	NUM_PLAYER_COLORS
};

const NyaDrawing::CNyaRGBA32 aColors[NUM_PLAYER_COLORS] = {
	{37, 25, 38, 255}, // COLOR_BLACK
	{127, 127, 127, 255}, // COLOR_GRAY
	{66, 33, 11, 255}, // COLOR_BROWN
	{243, 204, 205, 255}, // COLOR_LIGHT_PINK
	{245, 113, 163, 255}, // COLOR_DARK_PINK
	{211, 179, 223, 255}, // COLOR_LAVENDER
	{229, 229, 229, 255}, // COLOR_WHITE
	{255, 255, 255, 255}, // COLOR_PURE_WHITE
};

const int nSongListNumDisplayed = 19;
const int nPlaylistNumDisplayed = 26;
const float fBorderDistY = 0.01;
const float fPlaylistSelectBottom = 0.92;
const float fSongDisplayTop = fPlaylistSelectBottom + fBorderDistY;
const float fSongDisplayYSpacing = 0.02;
const float fSongArtSizeY = 0.05;
const float fCurrentPlaylistTop = 0.25;
const float fPlaylistTextInitialSpacing = 0.032;
const float fPlaylistTextSpacing = 0.032;
const float fScrollbarSizeX = 0.02;
const float fScrollbarOffsetX = 0.01;
const float fPlaylistTextXSpacing = 0.3 * 0.5625; // base spacing of 16:9, extend/squish so there's no empty space
const float fProgressBarPosY = fPlaylistSelectBottom + fBorderDistY + 0.05;
const float fProgressBarSizeY = 0.0066;
const float fSelectionRectSize = fPlaylistTextSpacing * 0.5;
const float fScrollbarTop = fCurrentPlaylistTop + fPlaylistTextInitialSpacing - fSelectionRectSize;
const float fScrollbarBottom = fCurrentPlaylistTop + fPlaylistTextInitialSpacing + fPlaylistTextSpacing * nSongListNumDisplayed + fSelectionRectSize;
const float fScrollbarYSize = 0.032;
const float fVolumeBarPosY = fPlaylistSelectBottom + fBorderDistY + 0.025;
const float fVolumeBarSizeY = 0.0066;
const float fTopBarTop = fBorderDistY;
const float fTopBarBottom = fCurrentPlaylistTop - fBorderDistY;
const float fAddSongSizeY = 0.02;
const float fAddSongSpacingY = 0.025;
const float fAddSongY = fTopBarBottom - fAddSongSpacingY;
const float fAddPlaylistSizeY = 0.02;
const float fAddPlaylistSpacingY = 0.025;
const float fAddPlaylistY = fPlaylistSelectBottom - fAddSongSpacingY;
const float fLogoSizeY = 0.2;
const float fLogoTop = fBorderDistY + 0.0133;
const float fPlayerControlsMidY = ((fProgressBarPosY - fProgressBarSizeY) + fPlaylistSelectBottom + fBorderDistY) * 0.5;
const float fPlayerControlsSizeY = 0.01;
const float fPlaylistSelectTop = fBorderDistY;
const float fPlaylistSelectTextSpacing = 0.032;
const float fPlaylistSelectTextHeaderY = fPlaylistSelectTop + 0.032;
const float fPlaylistSelectTextTop = fPlaylistSelectTextHeaderY + 0.05;
const float fPlaylistSelectHighlightSpacing = fPlaylistSelectTextSpacing * 0.5;
const float fDialogSizeY = 0.1;
const float fDialogTop = 0.5 - fDialogSizeY;
const float fDialogBottom = 0.5 + fDialogSizeY;
const float fDialogCloseSizeY = 0.01;
const float fDialogCloseY = fDialogTop + fAddSongSpacingY;
const float fDialogTextSpacingY = 0.04;
const float fDialogButtonTextSpacingY = 0.01;
const float fDialogPlaylistNameTextY = fDialogTop + 0.06;
const float fDialogInputSpacingY = 0.024;
const float fDialogInputSizeY = 0.03;
const float fDialogInputTop = fDialogPlaylistNameTextY + fDialogInputSpacingY;
const float fDialogInputBottom = fDialogPlaylistNameTextY + fDialogInputSpacingY + fDialogInputSizeY;

// dynamic values by aspect ratio
extern float fBorderDistX;
extern float fPlaylistSelectLeft;
extern float fPlaylistSelectRight;
extern float fPlaylistTextInitialXSpacing;
extern float fFinalScrollbarLeft;
extern float fFinalScrollbarRight;
extern float fProgressBarLeft;
extern float fProgressBarRight;
extern float fVolumeBarRight;
extern float fVolumeBarLeft;
extern float fTopBarLeft;
extern float fTopBarRight;
extern float fAddSongSizeX;
extern float fAddSongSpacingX;
extern float fAddSongX;
extern float fAddPlaylistSizeX;
extern float fAddPlaylistSpacingX;
extern float fAddPlaylistX;
extern float fLogoSizeX;
extern float fLogoLeft;
extern float fSongArtSizeX;
extern float fSongArtRight;
extern float fPlayerControlsMidX;
extern float fPlayerControlsSizeX;
extern float fPlayerControlsSpacingX;
extern float fPlaylistSelectTextMidX;
extern float fPlaylistSelectTextLeft;
extern float fDialogCloseSizeX;
extern float fDialogSizeX;
extern float fDialogLeft;
extern float fDialogRight;
extern float fDialogCloseX;
extern float fDialogTextSpacingX;
extern float fDialogButtonTextSpacingX;
extern float fDialogPlaylistNameTextX;
extern float fDialogInputSpacingX;
extern float fDialogInputLeft;
extern float fDialogInputRight;
void RecalculateAspect();
#endif