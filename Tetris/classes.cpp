#include "stdafx.h"
#include "classes.h"
#include "variables.h"

const Coordinate TetrominoShape[7][4] = {
	{Coordinate(0, 2), Coordinate(0, 1), Coordinate(0, 0), Coordinate(0, -1)},	// I
	{Coordinate(-1, 1), Coordinate(-1, 0), Coordinate(0, 0), Coordinate(1, 0)},	// J
	{Coordinate(1, 1), Coordinate(-1, 0), Coordinate(0, 0), Coordinate(1, 0)},	// L
	{Coordinate(0, 1), Coordinate(1, 1), Coordinate(0, 0), Coordinate(1, 0)},	// O
	{Coordinate(-1, 0), Coordinate(0, 1), Coordinate(0, 0), Coordinate(1, 1)},	// S
	{Coordinate(-1, 0), Coordinate(0, -1), Coordinate(0, 0), Coordinate(1, -1)},// Z
	{Coordinate(0, 1), Coordinate(1, 0), Coordinate(0, 0), Coordinate(-1, 0)},	// T
};

/*
	Direction:
	0: left
	1: right
	2: down
*/
const Coordinate delta[4] = {
	Coordinate(-1, 0), Coordinate(1, 0), Coordinate(0, -1), Coordinate(0, 1)
};

Coordinate::Coordinate(const int &_x = 0, const int &_y = 0): x(_x), y(_y) {
}

Coordinate Coordinate::rotate() {
	return Coordinate(y, -x);
}

Coordinate Coordinate::operator + (const Coordinate &other) {
	return Coordinate(x + other.x, y + other.y);
}

Coordinate Coordinate::operator - (const Coordinate &other) {
	return Coordinate(x - other.x, y - other.y);
}

Tetromino::Tetromino(int XSize, int YSize) {
	std::uniform_int_distribution<> typeDistribution(0, 6);
	std::default_random_engine e(time(0));
	type = typeDistribution(e);
	//type = 3;
	//color = colorDistribution(e);
	color = 11 + type;
	pos = Coordinate(XSize / 2, YSize);

	for (int i = 0; i < 4; ++i) {
		hitBox[i] = TetrominoShape[type][i];
	}
}

Bool Tetromino::hit(Playground &playground) {
	for (auto &block : hitBox) {
		if (!playground.isEmpty(pos + block)) {
			return true;
		}
	}
	return false;
}

/*
	Direction:
	0: left
	1: right
	2: down
	3: up
*/
Bool Tetromino::move(const int &Direction, Playground &playground) {
	pos = pos + delta[Direction];
	if (hit(playground)) {
		pos = pos - delta[Direction];
		Beep(Frequency[0], TimeShort * 2);
		return false;
	}
	return true;
}

Bool Tetromino::rotate(Playground &playground) {
	Tetromino tmp(*this);
	for (auto &i : tmp.hitBox) {
		i = i.rotate();
	}
	if (!tmp.hit(playground)) {
		*this = tmp;
		return true;
	}
	Beep(Frequency[0], TimeShort * 2);
	return false;
}

/*****************************************************/
/*
	state:
	0:	empty
	1-10: color
*/
Playground::Playground(int _XSize, int _YSize) {
	XSize = _XSize;
	YSize = _YSize;
	score = 0;
	for (int i = 0; i <= YSize + 2; ++i) {
		for (int j = 0; j <= XSize + 2; ++j) {
			state[i][j] = 0;
		}
	}
	for (int i = 0; i <= YSize; ++i) {
		state[i][0] = state[i][XSize + 1] = 1;
	}
	for (int i = 0; i < XSize + 2; ++i) {
		state[0][i] = 1;
	}
}

Bool Playground::isEmpty(const Coordinate &pos) {
	return state[pos.y][pos.x] == 0;
}

int Playground::add(const Tetromino &other) {
	for (const auto &block : other.hitBox) {
		state[other.pos.y + block.y][other.pos.x + block.x] = other.color;
	}
	return killRow();
}

int Playground::killRow() {
	auto check = [&](const int &d) -> Bool {
		for (int i = 1; i <= XSize; ++i) {
			if (!state[d][i]) {
				return false;
			}
		}
		return true;
	};

	auto kill = [&](const int &d) -> void {
		for (int i = d; i < YSize; ++i) {
			for (int j = 1; j <= XSize; ++j) {
				state[i][j] = state[i + 1][j];
			}
		}
		for (int j = 1; j <= XSize; ++j) {
			state[YSize][j] = 0;
		}
	};

	int res = 0;

	for (int i = 1; i <= YSize; ++i) {
		while (check(i)) {
			up3();
			kill(i);
			++res;
		}
	}
	return res;
}

char PenColors[18][12] = {
	"Black", "Dark Gray", "Gray", "Light Gray", "White",	// 0 ~ 4
	"Red", "Yellow", "Green", "Cyan", "Blue", "Magenta",	// 5 ~ 10
	"myOrange", "myBlue", "myRed", "myGreen", "myBrown", "myPurple", "myYellow"	// 11 ~ 17
};

const double Tetris::BlockSize = 0.3;
const double Tetris::KWidth = 0.015;
const double Tetris::KPeriod = 2;
int Tetris::MinPeriod = 300;
int Tetris::MaxPeriod = 500;

void Tetris::drawBlock(int x, int y, int color) {
	SetPenColor(PenColors[color]);
	SetPenSize(EdgeSize);

	MovePen(y * BlockSize + KWidth, x * BlockSize + KWidth);
	DrawLine(0, BlockSize - KWidth * 2);
	DrawLine(BlockSize - KWidth * 2, 0);
	DrawLine(0, -(BlockSize - KWidth * 2));
	DrawLine(-(BlockSize - KWidth * 2), 0);

	MovePen(y * BlockSize + KWidth * 2, x * BlockSize + KWidth * 2);
	StartFilledRegion(.7);
	DrawLine(0, BlockSize - KWidth * 2);
	DrawLine(BlockSize - KWidth * 2, 0);
	DrawLine(0, -(BlockSize - KWidth * 2));
	DrawLine(-(BlockSize - KWidth * 2), 0);
	EndFilledRegion();
}


void Tetris::defineColors() {
	DefineColor(PenColors[11], 1, .5, 0);
	DefineColor(PenColors[12], 0, .5, 1);
	DefineColor(PenColors[13], .78125, 0, 0);
	DefineColor(PenColors[14], 0, .5, 0);
	DefineColor(PenColors[15], .5, .25, 0);
	DefineColor(PenColors[16], .390625, 0, .78125);
	DefineColor(PenColors[17], 1, .78125, 0);
}

/******************************************************************************/

void Tetris::pause() {
	if (!pauseState) {
		cancelTimer(index * TimerNumber + 2);
		if (index == 0) {
			bgm.pause();
		}
		pauseState = true;
	}
}

void Tetris::resume() {
	if (pauseState) {
		setTimer();
		if (index == 0) {
			bgm.play();
		}
		pauseState = false;
	}
}

void Tetris::setTimer() {
	startTimer(index * TimerNumber + 1, RefreshPeriod);
	startTimer(index * TimerNumber + 2, curPeriod);
}

void Tetris::killTimer() {
	cancelTimer(index * TimerNumber + 1);
	cancelTimer(index * TimerNumber + 2);
}

void Tetris::updatePeriod() {
	curPeriod = max(MinPeriod, MaxPeriod - (time(0) - startTime) * KPeriod);
	cancelTimer(index * TimerNumber + 2);
	startTimer(index * TimerNumber + 2, curPeriod);
}

Tetris::Tetris(int _index, int _XSize, int _YSize, int _XPos) {
	index = _index;
	XSize = _XSize;
	YSize = _YSize;
	XPos = _XPos;
	tetromino = Tetromino(XSize, YSize);
	nextTetromino = Tetromino(XSize, YSize);
	playground = Playground(XSize, YSize);
	startTime = time(0);
	curPeriod = MaxPeriod;
	score = 0;

	cancelTimer(index * TimerNumber + 1);
	cancelTimer(index * TimerNumber + 2);
	overState = pauseState = visible = false;
	if (index == 0) {
		bgm.close();
		bgm.open("./bgm.mid");
	}
}

void Tetris::reset() {
	*this = Tetris(index, XSize, YSize, XPos);
}

void Tetris::gameover() {
	overState = true;
	cancelTimer(index * TimerNumber + 2);
	drawGameover();
	if (index == 0) {
		bgm.setTempo(bgm.OriginalTempo);
	}
	rankList[difficultyLevel].add(score);
	saveRankList(rankList);
	return;
}

void Tetris::clear() {
	char whiteColor[10] = "White";
	SetPenColor(whiteColor);
	MovePen(XPos * BlockSize, 0);

	StartFilledRegion(1);
	DrawLine(0							, (YSize + 2) * BlockSize);
	DrawLine((XSize + 5) * BlockSize	, 0);
	DrawLine(0							, -(YSize + 2) * BlockSize);
	DrawLine(-(XSize + 5) * BlockSize	, 0);
	EndFilledRegion();
}

void Tetris::drawFrame() {
	SetPenColor(PenColors[0]);
	SetPenSize(EdgeSize);

	MovePen (BlockSize * (XPos)	, 0);
	DrawLine(0						, BlockSize * YSize);
	DrawLine(BlockSize * XSize		, 0);
	DrawLine(0						, -(BlockSize * YSize));
	DrawLine(BlockSize * ( - XSize)	, 0);

	MovePen (BlockSize * (XSize + XPos)	, BlockSize * (YSize - 5));
	DrawLine(0					, BlockSize * 5);
	DrawLine(BlockSize * (5)	, 0);
	DrawLine(0					, -(BlockSize * 5));
	DrawLine(BlockSize * ( - 5)	, 0);
}

void Tetris::draw() {
	clear();
	drawFrame();

	for (int i = 1; i <= YSize; ++i) {
		for (int j = 1; j <= XSize; ++j) {
			if (playground.state[i][j] > 0) {
				drawBlock(i - 1, j + XPos - 1, playground.state[i][j]);
			}
		}
	}
	for (const auto &block : tetromino.hitBox) {
		if (tetromino.pos.x + block.x >= 1 && tetromino.pos.x + block.x <= XSize && tetromino.pos.y + block.y >= 1 && tetromino.pos.y + block.y <= YSize) {
			drawBlock(tetromino.pos.y + block.y - 1, tetromino.pos.x + block.x + XPos - 1, tetromino.color);
		}
	}

	/*
	extern int curPeriod;
	char tempoStr[10];
	sprintf(tempoStr, "%d", tetrisInstanceCount);
	DrawTextString(tempoStr);
	*/

	for (auto i : nextTetromino.hitBox) {
		drawBlock(YSize - 4 + i.y, 2 + XSize + i.x + XPos, nextTetromino.color);
	}

	MovePen(BlockSize * (XSize + XPos + 1), BlockSize * (YSize - 7));

	int pointSize = 40;
	SetPointSize(pointSize);

	char text[20];
	sprintf(text, "%d", score);
	DrawTextString(text);

	if (overState) {
		drawGameover();
	}

	SetPenSize(1);
	SetPointSize(1);
}

void Tetris::drawGameover() {
	int pointSize = 40;
	SetPointSize(pointSize);

	char text[20];
	sprintf(text, "Game Over");

	double	textWidth = TextStringWidth(text),
			textHeight = GetFontHeight();

	while (textWidth > XSize * BlockSize) {
		SetPointSize(--pointSize);
		textWidth = TextStringWidth(text),
		textHeight = GetFontHeight();
	}

	SetPenColor(PenColors[0]);
	SetPenSize(EdgeSize);

	MovePen(BlockSize * (XPos)	, BlockSize * (YSize / 2.));

	StartFilledRegion(.2);
	DrawLine(0						, 2 * textHeight);
	DrawLine(BlockSize * (XSize)	, 0);
	DrawLine(0						, -(2 * textHeight));
	DrawLine(BlockSize * (- XSize)	, 0);
	EndFilledRegion();
	 

	MovePen(BlockSize * (XPos + XSize / 2.) - textWidth / 2., BlockSize * (YSize / 2.) + textHeight * 0.6);

	DrawTextString(text);

	SetPenSize(1);
	SetPointSize(1);
}

RankList::RankList() {
	for (int i = 0; i < 10; ++i) {
		scoreList[i] = 0;
	}
}

void RankList::add(int newScore) {
	for (int i = 0; i < 10; ++i) {
		if (newScore > scoreList[i]) {
			for (int j = 9; j > i; --j) {
				scoreList[j] = scoreList[j - 1];
			}
			scoreList[i] = newScore;
			break;
		}
	}
}