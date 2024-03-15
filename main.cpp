#include <windows.h>

#include "nya_dx11_appbase.h"

#include "musicplayer_defs.h"
#include "musicplayer_state.h"
#include "musicplayer_playlist.h"
#include "musicplayer_saveload.h"
#include "musicplayer_draw.h"

void ProgramLoop() {
	RecalculateAspect();

	mSongListMutex.lock();
	ProcessPlayer();
	DrawPlayerUI();
	mSongListMutex.unlock();

	CommonMain();
}

void ProgramOnExit() {
	SaveAllToDisk();
}

int main()
{
	if (InitAppBase()) {
		NyaAudio::Init(ghWnd);

		ReadAllFromDisk();
		if (aPlaylists.empty()) AddPlaylist("All Songs");

		while (true) {
			AppBaseLoop();
			Sleep(0);
		}
		return 0;
	}
	return 1;
}
