#include "nya_dx11_appbase.h"
#include "musicplayer_defs.h"

float fBorderDistX;
float fPlaylistSelectLeft;
float fPlaylistSelectRight;
float fPlaylistTextInitialXSpacing;
float fFinalScrollbarLeft;
float fFinalScrollbarRight;
float fProgressBarLeft;
float fProgressBarRight;
float fVolumeBarRight;
float fVolumeBarLeft;
float fTopBarLeft;
float fTopBarRight;
float fAddSongSizeX;
float fAddSongSpacingX;
float fAddSongX;
float fAddPlaylistSizeX;
float fAddPlaylistSpacingX;
float fAddPlaylistX;
float fLogoSizeX;
float fLogoLeft;
float fSongArtSizeX;
float fSongArtRight;
float fPlayerControlsMidX;
float fPlayerControlsSizeX;
float fPlayerControlsSpacingX;
float fPlaylistSelectTextMidX;
float fPlaylistSelectTextLeft;
float fDialogCloseSizeX;
float fDialogSizeX;
float fDialogLeft;
float fDialogRight;
float fDialogCloseX;
float fDialogTextSpacingX;
float fDialogButtonTextSpacingX;
float fDialogPlaylistNameTextX;
float fDialogInputSpacingX;
float fDialogInputLeft;
float fDialogInputRight;

void RecalculateAspect() {
	fBorderDistX = fBorderDistY * GetAspectRatioInv();
	fPlaylistSelectLeft = fBorderDistX;
	fPlaylistSelectRight = 0.3 * GetAspectRatioInv();
	fPlaylistTextInitialXSpacing = 0.032 * GetAspectRatioInv();
	fFinalScrollbarLeft = 1 - fBorderDistX - (fScrollbarSizeX + fScrollbarOffsetX) * GetAspectRatioInv();
	fFinalScrollbarRight = 1 - fBorderDistX - fScrollbarOffsetX * GetAspectRatioInv();
	fProgressBarLeft = fPlaylistSelectRight + fBorderDistX;
	fProgressBarRight = 1 - fBorderDistX;
	fVolumeBarRight = 1 - fBorderDistX;
	fVolumeBarLeft = fVolumeBarRight - (0.2 * GetAspectRatioInv());
	fTopBarLeft = fPlaylistSelectRight + fBorderDistX;
	fTopBarRight = 1 - fBorderDistX;
	fAddSongSizeX = fAddSongSizeY * GetAspectRatioInv();
	fAddSongSpacingX = fAddSongSpacingY * GetAspectRatioInv();
	fAddSongX = fTopBarRight - fAddSongSpacingX;
	fAddPlaylistSizeX = fAddPlaylistSizeY * GetAspectRatioInv();
	fAddPlaylistSpacingX = fAddPlaylistSpacingY * GetAspectRatioInv();
	fAddPlaylistX = fPlaylistSelectRight - fAddPlaylistSpacingX;
	fLogoSizeX = fLogoSizeY * GetAspectRatioInv() * 2.99408284024;
	fLogoLeft = fPlaylistSelectRight + fBorderDistX + (0.0133 * GetAspectRatioInv());
	fSongArtSizeX = fSongArtSizeY * GetAspectRatioInv();
	fSongArtRight = fBorderDistX + fSongArtSizeX;
	fPlayerControlsMidX = (fVolumeBarLeft + fPlaylistSelectRight + fBorderDistX) * 0.5;
	fPlayerControlsSizeX = fPlayerControlsSizeY * GetAspectRatioInv();
	fPlayerControlsSpacingX = 0.03 * GetAspectRatioInv();
	fPlaylistSelectTextMidX = (fPlaylistSelectLeft + fPlaylistSelectRight) * 0.5;
	fPlaylistSelectTextLeft = fPlaylistSelectLeft + (0.025 * GetAspectRatioInv());
	fDialogCloseSizeX = fDialogCloseSizeY * GetAspectRatioInv();
	fDialogSizeX = 0.2 * GetAspectRatioInv();
	fDialogLeft = 0.5 - fDialogSizeX;
	fDialogRight = 0.5 + fDialogSizeX;
	fDialogCloseX = fDialogRight - fAddSongSpacingX;
	fDialogTextSpacingX = 0.04 * GetAspectRatioInv();
	fDialogButtonTextSpacingX = 0.01 * GetAspectRatioInv();
	fDialogPlaylistNameTextX = fDialogLeft + fDialogTextSpacingX;
	fDialogInputSpacingX = 0.02 * GetAspectRatioInv();
	fDialogInputLeft = fDialogLeft + fDialogInputSpacingX;
	fDialogInputRight = fDialogRight - fDialogInputSpacingX;
}