// Tetris.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "c++_to_c.h"

MIDIPlayer bgm;

int GameNumber = 1;
vector<Tetris> game;
const Coordinate SizeList[] = {Coordinate(10, 20), Coordinate(10, 20), Coordinate(5, 10), Coordinate(5, 5)};
RankList rankList[3]; // 0-easy
extern int gameMode;
int resetGame = 0;

void setDifficulty(int newDifficulty) {
	switch (newDifficulty) {
		case 0:
			Tetris::MinPeriod = 400;
			Tetris::MaxPeriod = 500;
			break;
		case 1:
			Tetris::MinPeriod = 300;
			Tetris::MaxPeriod = 400;
			break;
		case 2:
			Tetris::MinPeriod = 100;
			Tetris::MaxPeriod = 200;
			break;
	}
}

void setGameNumber(int newNumber) {
	GameNumber = newNumber;
}

int getGameNumber() {
	return GameNumber;
}

void TetrisStop() {
	bgm.pause();
	for (int i = 0; i < GameNumber; ++i) {
		game[i].killTimer();
	}
}

void TetrisInit() {
	Tetris::defineColors();
	registerKeyboardEvent(keyboardEventProcess);
	registerCharEvent(charEventProcess);
	registerTimerEvent(timerEventProcess);

	game.push_back(Tetris(0, SizeList[0].x, SizeList[0].y, 10));

	int maxYSize = game[0].YSize;

	for (int i = 1; i < 2; ++i) {
		game.push_back(Tetris(i, SizeList[i].x, SizeList[i].y, game[i-1].XPos + game[i-1].XSize + 7));
		maxYSize = max(maxYSize, game[i].YSize);
	}

	loadRankList(&rankList[0]);

	TetrisStop();
}

void saveScore() {
	if (gameMode == 0 || gameMode == 1) {
		for (int i = 0; i < GameNumber; ++i) {
			rankList[difficultyLevel].add(game[i].score);
		}
		saveRankList(rankList);
	}
}

void TetrisReset() {
	for (int i = 0; i < GameNumber; ++i) {
		game[i].reset();
	}
	resetGame = 1;
}

void TetrisStart() {
	if (resetGame) {
		bgm.play();
		for (int i = 0; i < GameNumber; ++i) {
			game[i].setTimer();
			game[i].visible = true;
		}
		resetGame = 0;
	}
}

void TetrisPause() {
	for (int i = 0; i < GameNumber; ++i) {
		game[i].pause();
	}
}

void TetrisResume() {
	for (int i = 0; i < GameNumber; ++i) {
		game[i].resume();
	}
}

int getSave(int index) {
	return !loadSaveFile(game[0], index);
}

int setSave(int index) {
	return !saveSaveFile(game[0], index);
}

int getScore(int level, int rank) {
	return rankList[level].scoreList[rank - 1];
}

void drawTetris() {
	for (int i = 0; i < GameNumber; ++i) {
		if (game[i].visible) {
			game[i].draw();
		}
	}
}
