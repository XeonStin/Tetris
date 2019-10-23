#pragma once

#include "variables.h"
#include "Tetris.h"
#include "save.h"

extern int resetGame;

void TetrisInit();

void TetrisStop();
void TetrisStart();

void TetrisPause();
void TetrisResume();

void TetrisReset();

void setDifficulty(int newDifficulty);
void setGameNumber(int newNumber);
int getGameNumber();

int getSave(int index);
int setSave(int index);
void saveScore();
int getScore(int level, int rank);

void drawTetris();