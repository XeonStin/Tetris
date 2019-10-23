#pragma once

#include "cextgraph.h"
#include "soundEffect.h"
#include "classes.h"
#include "variables.h"

void keyboardEventProcess(int key, int event);
void charEventProcess(char key);
void timerEventProcess(int timerID);

extern MIDIPlayer bgm2;
extern void display();
extern void TetrisPause();
extern void TetrisResume();
extern void TetrisReset();
extern void saveScore();
extern void TetrisStop();
extern int  currentPosition;
extern int  resetGame;
