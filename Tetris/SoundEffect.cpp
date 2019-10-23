#include "stdafx.h"
#include "soundEffect.h"
#include "variables.h"

const int Frequency[15] = {150, 523, 578, 659, 698, 784, 880, 988, 1098, 1221, 1358, 1510, 1679, 1866, 2075};

const int TimeShort = 30;

void up3() {
	for (int i = 1; i <= 3; ++i) {
		Beep(Frequency[i*4+1], TimeShort*2);
	}
}

DWORD MIDIPlayer::getTempo() {
	MCI_STATUS_PARMS StatusParms;
	StatusParms.dwItem = MCI_SEQ_STATUS_TEMPO;
	mciSendCommand(MIDIPlayerID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD)(LPMCI_STATUS_PARMS)&StatusParms);
	return StatusParms.dwReturn;
}

void MIDIPlayer::setTempo(DWORD newTempo) {
	MCI_SEQ_SET_PARMS SetParms;
	SetParms.dwTempo = newTempo;
	mciSendCommand(MIDIPlayerID, MCI_SET, MCI_SEQ_SET_TEMPO, (DWORD)(LPVOID)&SetParms);
}

void MIDIPlayer::play() {
	MCI_PLAY_PARMS  mciPlayParms;
	mciSendCommand(MIDIPlayerID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD_PTR)&mciPlayParms);
}

void MIDIPlayer::open(const char * fileName) {
	MCI_OPEN_PARMS mciOpenParms;
	mciOpenParms.lpstrDeviceType = TEXT("sequencer");
	mciOpenParms.lpstrElementName = TEXT(fileName);
	if (mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpenParms) == 0) {
		MIDIPlayerID = mciOpenParms.wDeviceID;
	}
	OriginalTempo = getTempo() * 0.6;
	setTempo(OriginalTempo);
	play();
	pause();
}

void MIDIPlayer::close() {
	mciSendCommand(MIDIPlayerID, MCI_CLOSE, 0, NULL);
}

void MIDIPlayer::pause() {
	MCI_PLAY_PARMS PlayParms;
	mciSendCommand(MIDIPlayerID, MCI_PAUSE, 0, (DWORD)(LPVOID)&PlayParms);
}

void MIDIPlayer::updateTempo(const Tetris &game) {

	auto getLevel = [&]() -> int {
		for (int j = game.YSize; j >= 1; --j) {
			for (int i = 1; i <= game.XSize; ++i) {
				if (game.playground.state[j][i]) {
					return j;
				}
			}
		}
		return 0;
	};

	double speed = pow(1.5, 1.0 * getLevel() / game.YSize);
	setTempo(DWORD(OriginalTempo * speed));
}
