#include "stdafx.h"
#include "eventProcess.h"
#include "variables.h"

void keyboardEventProcess(int key, int event) {
	if (event == KEY_UP) {
		Beep(Frequency[7], TimeShort);
		return;
	}
	if (currentPosition != 7) {
		return;
	}
	switch (key) {
		case VK_LEFT:
			game[1].tetromino.move(0, game[1].playground);
			break;
		case VK_RIGHT:
			game[1].tetromino.move(1, game[1].playground);
			break;
		case VK_DOWN:
			game[1].tetromino.move(2, game[1].playground);
			break;
		case VK_UP:
			game[1].tetromino.rotate(game[1].playground);
			break;
		case VK_ESCAPE:
			currentPosition = 2;
			saveScore();
			TetrisStop();
			resetGame = 1;
			bgm2.play();
			break;
		case VK_DELETE:
			saveScore();
			exit(-1);
			break;

		default:
			return;
	}
}

void charEventProcess(char key) {
	if (currentPosition != 7) {
		return;
	}
	key = tolower(key);
	switch (key) {
		case 'a':
			game[0].tetromino.move(0, game[0].playground);
			break;
		case 'd':
			game[0].tetromino.move(1, game[0].playground);
			break;
		case 's':
			game[0].tetromino.move(2, game[0].playground);
			break;
		case 'w':
			game[0].tetromino.rotate(game[0].playground);
			break;

		case 'p':
		case ' ':

			if (gamePause) {
				TetrisResume();
			} else {
				TetrisPause();
			}
			gamePause = !gamePause;
			break;
			
		case 'r':
			TetrisReset();
			break;

		case 'g':
			saveScore();
			TetrisStop();
			currentPosition = 11;
			resetGame = 1;
			break;

		default:
			break;
	}
}

/*
	timerID:
	1: refresh
	2: fall
*/
void timerEventProcess(int timerID) {
	if (timerID == 0) {
		display();
		return;
	}
	int i = timerID / Tetris::TimerNumber;
	switch (timerID % Tetris::TimerNumber) {
		case 1:
			//game[i].draw();
			break;
		case 2:
			if (!game[i].tetromino.move(2, game[i].playground)) {
				Beep(Frequency[0], TimeShort * 2);

				const int rowKilled = game[i].playground.add(game[i].tetromino);

				game[i].score += pow(1.1, rowKilled) * game[i].XSize * Tetris::Score2 * 1. * Tetris::MaxPeriod / game[i].curPeriod * rowKilled;

				game[i].tetromino = game[i].nextTetromino;
				game[i].nextTetromino = Tetromino(game[i].XSize, game[i].YSize);

				if (game[i].tetromino.hit(game[i].playground)) {
					game[i].gameover();
					return;
				}

				game[i].score += Tetris::Score1 * 1. * Tetris::MaxPeriod / game[i].curPeriod;
				if (i == 0) {
					bgm.updateTempo(game[0]);
				}
			}
			game[i].updatePeriod();
			break;
	}
}
