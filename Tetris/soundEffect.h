#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib,"winmm.lib")
#include "cextgraph.h"
#include <Digitalv.h>  
#include "classes.h"
#include <cmath>

extern const int Frequency[15];

extern const int TimeShort;

extern class Tetris;

void up3();

class MIDIPlayer {
private:
	MCIDEVICEID MIDIPlayerID;
public:
	DWORD OriginalTempo;
	DWORD getTempo();
	void setTempo(DWORD newTempo);
	void play();
	void open(const char *);
	void close();
	void pause();
	void updateTempo(const Tetris &game);
};