#include "cgraphics.h"
#include "cextgraph.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "cimgui.h"
#include <math.h>
#include "soundEffect.h"
#include "variables.h"
#include "c++_to_c.h"

MIDIPlayer bgm2;
char WindowName[20] = "俄罗斯方块";
double winwidth, winheight;   // 窗口尺寸
int    currentPosition = 0;
int    gameMode = 0;
int    gamePause = 0;
int difficultyLevel; //难度系数（1-简单模式；2-普通模式；3-困难模式）
void display();
void Main();
void MainMenu();
void ModeSelection();
void GameIntroduction();
void SoftwareIntroduction();
void ChooseDifficulty();
void SummitReport();
void LoadSelection();
void SimpleSummitReport();
void NormalSummitReport();
void HardSummitReport();
void GameDisplay();

void MouseEventProcess(int x, int y, int button, int event)
{
	uiGetMouse(x, y, button, event);
	if (event == BUTTON_UP) {
		Beep(Frequency[9], TimeShort);
	}
}

void Main() {

	// 初始化交互窗口和图形系统
	//loop:

	SetWindowTitle(WindowName);

	//SetWindowTitle("Tetris");
	SetWindowSize(14.01, 6.01); // 单位 - 英寸
	InitGraphics();

	registerMouseEvent(MouseEventProcess);
	TetrisInit();
	startTimer(0, 33);
	// 获得窗口尺寸
	winwidth = GetWindowWidth();
	winheight = GetWindowHeight();

	bgm2.open("./bgm2.mid");
	bgm2.setTempo(50);
	bgm2.play();
}

void MainMenu() {
	DisplayClear();

	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);

	sprintf(words, WindowName);
	DrawTextString(words);

	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	gameMode = 0;
	resetGame = 1;

	if (button(GenUIID(0), x + 0.05, y + 4 * h, w, h, "新游戏") && currentPosition == 0) {
		TetrisReset();
		currentPosition = 1;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y + 2 * h, w, h, "读取存档") && currentPosition == 0) {
		currentPosition = 4;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y, w, h, "游戏介绍") && currentPosition == 0) {
		currentPosition = 5;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y - 2 * h, w, h, "软件介绍") && currentPosition == 0) {
		currentPosition = 6;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y - 4 * h, w, h, "退出游戏") && currentPosition == 0)
		exit(-1);

}

void ModeSelection() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度
	
	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "模式选择");
	DrawTextString(words);

	if (button(GenUIID(0), x, y + 4 * h, w, h, "单人模式") && currentPosition == 1) {
		game[0].XSize = 10;
		TetrisReset();
		currentPosition = 2;
		gameMode = 0;
		return;
	}
	if (button(GenUIID(0), x, y + 2 * h, w, h, "双人模式") && currentPosition == 1) {
		game[0].XSize = 10;
		TetrisReset();
		currentPosition = 2;
		gameMode = 1;
		return;
	}
	if (button(GenUIID(0), x, y + 0, w, h, "狭窄模式") && currentPosition == 1) {
		game[0].XSize = 4;
		TetrisReset();
		currentPosition = 2;
		gameMode = 2;
		return;
	}
	if (button(GenUIID(0), x, y - 2 * h, w, h, "返回主菜单") && currentPosition == 1) {
		currentPosition = 0;
		return;
	}
}

void GameIntroduction() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "游戏介绍");
	DrawTextString(words);
	SetPenColor(PenColors[5]);
	MovePen(winwidth / 4 - 1, winheight / 2 + 1.5);
	DrawLine(winwidth / 2 + 2, 0);
	DrawLine(0, -(1.75+2*h));
	DrawLine(-(winwidth / 2 + 2), 0);
	DrawLine(0, 1.75+2*h);

	SetPenColor(PenColors[0]);
	MovePen(winwidth / 4 - 0.5, winheight / 2 + 1.25);
	DrawTextString("操作介绍");
	SetPenColor(PenColors[9]);
	MovePen(winwidth / 4 + 1, winheight / 2 + 1);
	DrawTextString("单人模式： W旋转  A向左  D向右  S向下");
	MovePen(winwidth / 4 + 1, winheight / 2 + 0.75);
	DrawTextString("双人模式：玩家一：W旋转  A向左  D向右  S向下");
	MovePen(winwidth / 4 + 2.04, winheight / 2 + 0.5);
	DrawTextString("玩家二：↑  旋转  ←向左  →向右 ↓  向下");
	SetPenColor(PenColors[0]);
	MovePen(winwidth / 4 - 0.5, winheight / 2 + 0.25);
	DrawTextString("规则介绍：");
	SetPenColor(PenColors[9]);
	MovePen(winwidth / 4 + 1, winheight / 2);
	DrawTextString("1、	整行方块铺满获得消除加分，每多一个方块获得操作加分；");
	MovePen(winwidth / 4 + 1, winheight / 2 - 0.25);
	DrawTextString("2、	随着得分变高，方块下落速度会加快；");
	MovePen(winwidth / 4 + 1, winheight / 2 - 0.5);
	DrawTextString("3、	当方块叠至最高时游戏结束；");
	MovePen(winwidth / 4 + 1, winheight / 2 - 0.75);
	DrawTextString("4、	用户可以存档以及查看排行榜。");


	if (button(GenUIID(0), x, y - 4 * h, w, h, "返回主菜单") && currentPosition == 5) {
		currentPosition = 0;
		return;
	}
}

void SoftwareIntroduction() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.75, winheight / 2 + 2);
	SetPointSize(30);
	sprintf(words, "软件介绍");
	DrawTextString(words);
	SetPointSize(20);
	SetPenColor(PenColors[5]);
	MovePen(winwidth / 4 - 1, winheight / 2 + 1.5);
	DrawLine(winwidth / 2 + 2, 0);
	DrawLine(0, -2.1);
	DrawLine(-(winwidth / 2 + 2), 0);
	DrawLine(0, 2.1);

	SetPenColor(PenColors[9]);
	MovePen(winwidth / 4 - 0.5, winheight / 2 + 1.25);
	DrawTextString("本程序设计和实现了带有图形界面的游戏：俄罗斯方块。俄罗斯方块是一个操作简");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 1);
	DrawTextString("单的方块消除游戏。本项目程序实现基于libGraphics，实现俄罗斯方块游戏的正常运行。");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 0.75);
	DrawTextString("游戏开始时，从游戏界面顶端随机生成一个方块组自动下落，玩家可以选择按下键盘上");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 0.5);
	DrawTextString("WASD来操控方块的运动。而新的方块组出现在游戏界面上方开始落下，下一个方块组");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 0.25);
	DrawTextString("会提前预示在游戏界面右侧的小窗口里。当方块填满某一行时，该行方块全部消失，该");
	MovePen(winwidth / 4 - 0.9, winheight / 2);
	DrawTextString("行以上方块整体下落一行。当固定的方块堆到区域最上方而无法消除层数时，则游戏结");
	MovePen(winwidth / 4 - 0.9, winheight / 2 - 0.25);
	DrawTextString("束。每个下落方块组成功变成固定方块即得分,成功消除一行方块时，获得得分加成。");
	MovePen(winwidth / 4 - 0.5, winheight / 2 - 0.5);
	DrawTextString("此外本游戏还增设了双人对战模式和BGM增强了玩家的游戏体验感。");


	if (button(GenUIID(0), x, y - 2 * h, w, h, "返回主菜单") && currentPosition == 6) {
		currentPosition = 0;
		return;
	}
}

void ChooseDifficulty() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "难度选择");
	DrawTextString(words);

	if (button(GenUIID(0), x, y + 4 * h, w, h, "简单模式") && currentPosition == 2) {
		currentPosition = 7;
		difficultyLevel = 0;
		return;
	}
	if (button(GenUIID(0), x, y+2*h, w, h, "普通模式") && currentPosition == 2) {
		currentPosition = 7;
		difficultyLevel = 1;
		return;
	}
	if (button(GenUIID(0), x, y , w, h, "困难模式") && currentPosition == 2) {
		currentPosition = 7;
		difficultyLevel = 2;
		return;
	}
	if (button(GenUIID(0), x, y - 2 * h, w, h, "巅峰战报") && currentPosition == 2) {
		currentPosition = 3;
		return;
	}
	if (button(GenUIID(0), x, y - 4 * h, w, h, "返回模式选择") && currentPosition == 2){
		currentPosition = 1;
		return;
	}
}

void SummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "巅峰战报");
	DrawTextString(words);

	if (button(GenUIID(0), x, y + 4 * h, w, h, "简单模式") && currentPosition == 3) {
		currentPosition = 8;
		return;
	}
	if (button(GenUIID(0), x, y + 2 * h, w, h, "普通模式") && currentPosition == 3) {
		currentPosition = 9;
		return;
	}
	if (button(GenUIID(0), x, y, w, h, "困难模式") && currentPosition == 3) {
		currentPosition = 10;
		return;
	}
	if (button(GenUIID(0), x, y - 2 * h, w, h, "返回难度选择") && currentPosition == 3) {
		currentPosition = 2;
		return;
	}
}

void LoadSelection() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "存档选择");
	DrawTextString(words);

	if (button(GenUIID(0), x / 2, y + 4 * h, w, h, "存档一") && currentPosition == 4) {
		if (getSave(1)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x , y + 4 * h, w, h, "存档二") && currentPosition == 4) {
		if (getSave(2)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), 3*x/2, y + 4 * h, w, h, "存档三") && currentPosition == 4) {
		if (getSave(3)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x/2, y+2*h, w, h, "存档四") && currentPosition == 4) {
		if (getSave(4)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x, y+2*h, w, h, "存档五") && currentPosition == 4) {
		if (getSave(5)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), 3*x/2, y+2*h , w, h, "存档六") && currentPosition == 4) {
		if (getSave(6)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x, y - 3 * h, w, h, "返回主菜单") && currentPosition == 4) {
		currentPosition = 0;
		return;
	}
}

void SimpleSummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[300] = "";
	MovePen(winwidth / 2 -0.70, winheight / 2 + 2);
	sprintf(words, "巅峰战报（简单）");
	DrawTextString(words);
	for (int i = 1; i <= 10; ++i) {
		MovePen(winwidth / 2 - 0.70, winheight / 2 + 2 - GetFontHeight() * i * 2);
		sprintf(words, "第%2d名 %10d分", i, getScore(0, i));
		DrawTextString(words);
	}


	//读取数据！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	if (button(GenUIID(0), x, y - 4 * h, w, h, "返回巅峰战报") && currentPosition == 8) {
		currentPosition = 3;
		return;
	}
}

void NormalSummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.70, winheight / 2 + 2);
	sprintf(words, "巅峰战报（普通）");
	DrawTextString(words);
	for (int i = 1; i <= 10; ++i) {
		MovePen(winwidth / 2 - 0.70, winheight / 2 + 2 - GetFontHeight() * i * 2);
		sprintf(words, "第%2d名 %10d分", i, getScore(1, i));
		DrawTextString(words);
	}

	//读取数据！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	if (button(GenUIID(0), x, y - 4 * h, w, h, "返回巅峰战报") && currentPosition == 9) {
		currentPosition = 3;
		return;
	}
}

void HardSummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.70, winheight / 2 + 2);
	sprintf(words, "巅峰战报（困难）");
	DrawTextString(words);
	for (int i = 1; i <= 10; ++i) {
		MovePen(winwidth / 2 - 0.70, winheight / 2 + 2 - GetFontHeight() * i * 2);
		sprintf(words, "第%2d名 %10d分", i, getScore(2, i));
		DrawTextString(words);
	}

	//读取数据！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	if (button(GenUIID(0), x, y - 4 * h, w, h, "返回巅峰战报") && currentPosition == 10) {
		currentPosition = 3;
		return;
	}
}

void SaveSelection() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "存档选择");
	DrawTextString(words);

	if (button(GenUIID(0), x / 2, y + 4 * h, w, h, "存档一") && currentPosition == 11) {
		if (setSave(1)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x, y + 4 * h, w, h, "存档二") && currentPosition == 11) {
		if (setSave(2)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), 3 * x / 2, y + 4 * h, w, h, "存档三") && currentPosition == 11) {
		if (setSave(3)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x / 2, y + 2 * h, w, h, "存档四") && currentPosition == 11) {
		if (setSave(4)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x, y + 2 * h, w, h, "存档五") && currentPosition == 11) {
		if (setSave(5)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), 3 * x / 2, y + 2 * h, w, h, "存档六") && currentPosition == 11) {
		if (setSave(6)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x, y - 3 * h, w, h, "返回") && currentPosition == 11) {
		currentPosition = 7;
		return;
	}
}

void GameDisplay() {
	double fH = GetFontHeight();
	double h = fH * 2;  // 控件高度
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // 控件宽度

	bgm2.pause();

	DisplayClear();

	MovePen(0, 0.01);
	SetPenColor(PenColors[5]);
	DrawLine(12, 0);
	DrawLine(0, 6);
	DrawLine(-12, 0);
	DrawLine(0, -6);
	MovePen(3, 0.01);
	DrawLine(0, 6);

	if (button(GenUIID(0), 1, 5, 1.2, 0.5, gamePause ? "继续(P)": "暂停(P)") && currentPosition == 7) {
		if (gamePause) {
			TetrisResume();
		} else {
			TetrisPause();
		}
		gamePause = !gamePause;
	}

	if (button(GenUIID(0), 1, 4, 1.2, 0.5, "重新开始(R)") && currentPosition == 7) {
		saveScore();
		TetrisReset();
	}

	if (gameMode==0 && button(GenUIID(0), 1, 3, 1.2, 0.5, "保存(G)") && currentPosition == 7) {
		saveScore();
		TetrisStop();
		currentPosition = 11;
		resetGame = 1;
		return;
	}

	if (button(GenUIID(0), 1, 2, 1.2, 0.5, "返回上一页(Esc)") && currentPosition == 7) {
		currentPosition = 2;
		saveScore();
		TetrisReset();
		resetGame = 1;
		bgm2.play();
		return;
	}

	if (button(GenUIID(0), 1, 1, 1.2, 0.5, "退出游戏(Del)") && currentPosition == 7) {
		saveScore();
		exit(-1);
	}

	setDifficulty(difficultyLevel);
	setGameNumber(gameMode == 1 ? 2 : 1);
	TetrisStart();
	drawTetris();
}

void display() {
	// 清屏
	DisplayClear();

	//registerKeyboardEvent(KeyboardEventProcess);// 鼠标
	switch (currentPosition) {
		case 0:
			MainMenu();
			break;

		case 1:
			ModeSelection();
			break;

		case 2:
			ChooseDifficulty();
			break;

		case 3:
			SummitReport();
			break;

		case 4:
			LoadSelection();
			break;

		case 5:
			GameIntroduction();
			break;

		case 6:
			SoftwareIntroduction();
			break;

		case 7:
			GameDisplay();
			break;

		case 8:
			SimpleSummitReport();
			break;

		case 9:
			NormalSummitReport();
			break;

		case 10:
			HardSummitReport();
			break;

		case 11:
			SaveSelection();
			break;

		default:
			currentPosition = 0;
			MainMenu();
			break;
	}
}
