#pragma once

#include "soundEffect.h"
#include "save.h"
#include <vector>
#include <random>
#include <ctime>

using std::vector;

class Coordinate {
private:
public:
	int x, y;
	Coordinate(const int &_x, const int &_y);
	Coordinate rotate();
	Coordinate operator + (const Coordinate &other);
	Coordinate operator - (const Coordinate &other);
};

class Playground;

class Tetromino {
private:
public:
	Coordinate hitBox[4];
	int type, color;
	Coordinate pos;

	Tetromino(int _XSize = 10, int _YSize = 20);
	Bool hit(Playground &playground);
	Bool move(const int &Direction, Playground &playground);
	Bool rotate(Playground &playground);
};

class Playground {
private:

public:
	int XSize, YSize;
	int score;
	int state[50][50];

	Playground(int _XSize = 10, int _YSize = 20);
	Bool isEmpty(const Coordinate &pos);
	int add(const Tetromino &other);
	int killRow();
};

class Tetris {
public:
	static const int RefreshPeriod = 33, TimerNumber = 3, EdgeSize = 3, Score1 = 100, Score2 = 100;
	static int MinPeriod, MaxPeriod;
	static const double BlockSize, KWidth, KPeriod;

	static void drawBlock(int x, int y, int color);
	static void defineColors();

	/******************************************************************************/


	int startTime, curPeriod;
	int index, XSize, YSize, XPos, score;
	Playground playground;
	Tetromino tetromino, nextTetromino;
	Bool overState, pauseState, visible;

	void pause();
	void resume();
	void setTimer();
	void killTimer();
	void updatePeriod();
	Tetris(int _index = 0, int _XSize = 10, int _YSize = 20, int _XPos = 0);
	void reset();
	void gameover();
	void clear();
	void drawFrame();
	void draw();
	void drawGameover();
};

class RankList {
public:
	int scoreList[10];
	RankList();
	void add(int newScore);
};
