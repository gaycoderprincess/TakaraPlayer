#ifndef MUSICPLAYER_SAVELOAD_H
#define MUSICPLAYER_SAVELOAD_H
const int nSaveVersion = 1;
void SaveAllToDisk();
bool ReadAllFromDisk();
#endif