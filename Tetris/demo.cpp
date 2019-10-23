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
char WindowName[20] = "����˹����";
double winwidth, winheight;   // ���ڳߴ�
int    currentPosition = 0;
int    gameMode = 0;
int    gamePause = 0;
int difficultyLevel; //�Ѷ�ϵ����1-��ģʽ��2-��ͨģʽ��3-����ģʽ��
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

	// ��ʼ���������ں�ͼ��ϵͳ
	//loop:

	SetWindowTitle(WindowName);

	//SetWindowTitle("Tetris");
	SetWindowSize(14.01, 6.01); // ��λ - Ӣ��
	InitGraphics();

	registerMouseEvent(MouseEventProcess);
	TetrisInit();
	startTimer(0, 33);
	// ��ô��ڳߴ�
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
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	gameMode = 0;
	resetGame = 1;

	if (button(GenUIID(0), x + 0.05, y + 4 * h, w, h, "����Ϸ") && currentPosition == 0) {
		TetrisReset();
		currentPosition = 1;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y + 2 * h, w, h, "��ȡ�浵") && currentPosition == 0) {
		currentPosition = 4;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y, w, h, "��Ϸ����") && currentPosition == 0) {
		currentPosition = 5;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y - 2 * h, w, h, "�������") && currentPosition == 0) {
		currentPosition = 6;
		return;
	}

	if (button(GenUIID(0), x + 0.05, y - 4 * h, w, h, "�˳���Ϸ") && currentPosition == 0)
		exit(-1);

}

void ModeSelection() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����
	
	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "ģʽѡ��");
	DrawTextString(words);

	if (button(GenUIID(0), x, y + 4 * h, w, h, "����ģʽ") && currentPosition == 1) {
		game[0].XSize = 10;
		TetrisReset();
		currentPosition = 2;
		gameMode = 0;
		return;
	}
	if (button(GenUIID(0), x, y + 2 * h, w, h, "˫��ģʽ") && currentPosition == 1) {
		game[0].XSize = 10;
		TetrisReset();
		currentPosition = 2;
		gameMode = 1;
		return;
	}
	if (button(GenUIID(0), x, y + 0, w, h, "��խģʽ") && currentPosition == 1) {
		game[0].XSize = 4;
		TetrisReset();
		currentPosition = 2;
		gameMode = 2;
		return;
	}
	if (button(GenUIID(0), x, y - 2 * h, w, h, "�������˵�") && currentPosition == 1) {
		currentPosition = 0;
		return;
	}
}

void GameIntroduction() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "��Ϸ����");
	DrawTextString(words);
	SetPenColor(PenColors[5]);
	MovePen(winwidth / 4 - 1, winheight / 2 + 1.5);
	DrawLine(winwidth / 2 + 2, 0);
	DrawLine(0, -(1.75+2*h));
	DrawLine(-(winwidth / 2 + 2), 0);
	DrawLine(0, 1.75+2*h);

	SetPenColor(PenColors[0]);
	MovePen(winwidth / 4 - 0.5, winheight / 2 + 1.25);
	DrawTextString("��������");
	SetPenColor(PenColors[9]);
	MovePen(winwidth / 4 + 1, winheight / 2 + 1);
	DrawTextString("����ģʽ�� W��ת  A����  D����  S����");
	MovePen(winwidth / 4 + 1, winheight / 2 + 0.75);
	DrawTextString("˫��ģʽ�����һ��W��ת  A����  D����  S����");
	MovePen(winwidth / 4 + 2.04, winheight / 2 + 0.5);
	DrawTextString("��Ҷ�����  ��ת  ������  ������ ��  ����");
	SetPenColor(PenColors[0]);
	MovePen(winwidth / 4 - 0.5, winheight / 2 + 0.25);
	DrawTextString("������ܣ�");
	SetPenColor(PenColors[9]);
	MovePen(winwidth / 4 + 1, winheight / 2);
	DrawTextString("1��	���з���������������ӷ֣�ÿ��һ�������ò����ӷ֣�");
	MovePen(winwidth / 4 + 1, winheight / 2 - 0.25);
	DrawTextString("2��	���ŵ÷ֱ�ߣ����������ٶȻ�ӿ죻");
	MovePen(winwidth / 4 + 1, winheight / 2 - 0.5);
	DrawTextString("3��	������������ʱ��Ϸ������");
	MovePen(winwidth / 4 + 1, winheight / 2 - 0.75);
	DrawTextString("4��	�û����Դ浵�Լ��鿴���а�");


	if (button(GenUIID(0), x, y - 4 * h, w, h, "�������˵�") && currentPosition == 5) {
		currentPosition = 0;
		return;
	}
}

void SoftwareIntroduction() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.75, winheight / 2 + 2);
	SetPointSize(30);
	sprintf(words, "�������");
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
	DrawTextString("��������ƺ�ʵ���˴���ͼ�ν������Ϸ������˹���顣����˹������һ��������");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 1);
	DrawTextString("���ķ���������Ϸ������Ŀ����ʵ�ֻ���libGraphics��ʵ�ֶ���˹������Ϸ���������С�");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 0.75);
	DrawTextString("��Ϸ��ʼʱ������Ϸ���涥���������һ���������Զ����䣬��ҿ���ѡ���¼�����");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 0.5);
	DrawTextString("WASD���ٿط�����˶������µķ������������Ϸ�����Ϸ���ʼ���£���һ��������");
	MovePen(winwidth / 4 - 0.9, winheight / 2 + 0.25);
	DrawTextString("����ǰԤʾ����Ϸ�����Ҳ��С���������������ĳһ��ʱ�����з���ȫ����ʧ����");
	MovePen(winwidth / 4 - 0.9, winheight / 2);
	DrawTextString("�����Ϸ�����������һ�С����̶��ķ���ѵ��������Ϸ����޷���������ʱ������Ϸ��");
	MovePen(winwidth / 4 - 0.9, winheight / 2 - 0.25);
	DrawTextString("����ÿ�����䷽����ɹ���ɹ̶����鼴�÷�,�ɹ�����һ�з���ʱ����õ÷ּӳɡ�");
	MovePen(winwidth / 4 - 0.5, winheight / 2 - 0.5);
	DrawTextString("���Ȿ��Ϸ��������˫�˶�սģʽ��BGM��ǿ����ҵ���Ϸ����С�");


	if (button(GenUIID(0), x, y - 2 * h, w, h, "�������˵�") && currentPosition == 6) {
		currentPosition = 0;
		return;
	}
}

void ChooseDifficulty() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "�Ѷ�ѡ��");
	DrawTextString(words);

	if (button(GenUIID(0), x, y + 4 * h, w, h, "��ģʽ") && currentPosition == 2) {
		currentPosition = 7;
		difficultyLevel = 0;
		return;
	}
	if (button(GenUIID(0), x, y+2*h, w, h, "��ͨģʽ") && currentPosition == 2) {
		currentPosition = 7;
		difficultyLevel = 1;
		return;
	}
	if (button(GenUIID(0), x, y , w, h, "����ģʽ") && currentPosition == 2) {
		currentPosition = 7;
		difficultyLevel = 2;
		return;
	}
	if (button(GenUIID(0), x, y - 2 * h, w, h, "�۷�ս��") && currentPosition == 2) {
		currentPosition = 3;
		return;
	}
	if (button(GenUIID(0), x, y - 4 * h, w, h, "����ģʽѡ��") && currentPosition == 2){
		currentPosition = 1;
		return;
	}
}

void SummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "�۷�ս��");
	DrawTextString(words);

	if (button(GenUIID(0), x, y + 4 * h, w, h, "��ģʽ") && currentPosition == 3) {
		currentPosition = 8;
		return;
	}
	if (button(GenUIID(0), x, y + 2 * h, w, h, "��ͨģʽ") && currentPosition == 3) {
		currentPosition = 9;
		return;
	}
	if (button(GenUIID(0), x, y, w, h, "����ģʽ") && currentPosition == 3) {
		currentPosition = 10;
		return;
	}
	if (button(GenUIID(0), x, y - 2 * h, w, h, "�����Ѷ�ѡ��") && currentPosition == 3) {
		currentPosition = 2;
		return;
	}
}

void LoadSelection() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "�浵ѡ��");
	DrawTextString(words);

	if (button(GenUIID(0), x / 2, y + 4 * h, w, h, "�浵һ") && currentPosition == 4) {
		if (getSave(1)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x , y + 4 * h, w, h, "�浵��") && currentPosition == 4) {
		if (getSave(2)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), 3*x/2, y + 4 * h, w, h, "�浵��") && currentPosition == 4) {
		if (getSave(3)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x/2, y+2*h, w, h, "�浵��") && currentPosition == 4) {
		if (getSave(4)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x, y+2*h, w, h, "�浵��") && currentPosition == 4) {
		if (getSave(5)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), 3*x/2, y+2*h , w, h, "�浵��") && currentPosition == 4) {
		if (getSave(6)) {
			currentPosition = 7;
		}
		return;
	}
	if (button(GenUIID(0), x, y - 3 * h, w, h, "�������˵�") && currentPosition == 4) {
		currentPosition = 0;
		return;
	}
}

void SimpleSummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[300] = "";
	MovePen(winwidth / 2 -0.70, winheight / 2 + 2);
	sprintf(words, "�۷�ս�����򵥣�");
	DrawTextString(words);
	for (int i = 1; i <= 10; ++i) {
		MovePen(winwidth / 2 - 0.70, winheight / 2 + 2 - GetFontHeight() * i * 2);
		sprintf(words, "��%2d�� %10d��", i, getScore(0, i));
		DrawTextString(words);
	}


	//��ȡ���ݣ�����������������������������������������������������������������������
	if (button(GenUIID(0), x, y - 4 * h, w, h, "�����۷�ս��") && currentPosition == 8) {
		currentPosition = 3;
		return;
	}
}

void NormalSummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.70, winheight / 2 + 2);
	sprintf(words, "�۷�ս������ͨ��");
	DrawTextString(words);
	for (int i = 1; i <= 10; ++i) {
		MovePen(winwidth / 2 - 0.70, winheight / 2 + 2 - GetFontHeight() * i * 2);
		sprintf(words, "��%2d�� %10d��", i, getScore(1, i));
		DrawTextString(words);
	}

	//��ȡ���ݣ�����������������������������������������������������������������������
	if (button(GenUIID(0), x, y - 4 * h, w, h, "�����۷�ս��") && currentPosition == 9) {
		currentPosition = 3;
		return;
	}
}

void HardSummitReport() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[5]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.70, winheight / 2 + 2);
	sprintf(words, "�۷�ս�������ѣ�");
	DrawTextString(words);
	for (int i = 1; i <= 10; ++i) {
		MovePen(winwidth / 2 - 0.70, winheight / 2 + 2 - GetFontHeight() * i * 2);
		sprintf(words, "��%2d�� %10d��", i, getScore(2, i));
		DrawTextString(words);
	}

	//��ȡ���ݣ�����������������������������������������������������������������������
	if (button(GenUIID(0), x, y - 4 * h, w, h, "�����۷�ս��") && currentPosition == 10) {
		currentPosition = 3;
		return;
	}
}

void SaveSelection() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

	DisplayClear();
	SetPenColor(PenColors[0]);
	char words[128] = "";
	MovePen(winwidth / 2 - 0.35, winheight / 2 + 2);
	sprintf(words, "�浵ѡ��");
	DrawTextString(words);

	if (button(GenUIID(0), x / 2, y + 4 * h, w, h, "�浵һ") && currentPosition == 11) {
		if (setSave(1)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x, y + 4 * h, w, h, "�浵��") && currentPosition == 11) {
		if (setSave(2)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), 3 * x / 2, y + 4 * h, w, h, "�浵��") && currentPosition == 11) {
		if (setSave(3)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x / 2, y + 2 * h, w, h, "�浵��") && currentPosition == 11) {
		if (setSave(4)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x, y + 2 * h, w, h, "�浵��") && currentPosition == 11) {
		if (setSave(5)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), 3 * x / 2, y + 2 * h, w, h, "�浵��") && currentPosition == 11) {
		if (setSave(6)) {
			currentPosition = 7;
		}
	}
	if (button(GenUIID(0), x, y - 3 * h, w, h, "����") && currentPosition == 11) {
		currentPosition = 7;
		return;
	}
}

void GameDisplay() {
	double fH = GetFontHeight();
	double h = fH * 2;  // �ؼ��߶�
	double x = winwidth / 2.5;
	double y = winheight / 2 - h;
	double w = winwidth / 5; // �ؼ����

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

	if (button(GenUIID(0), 1, 5, 1.2, 0.5, gamePause ? "����(P)": "��ͣ(P)") && currentPosition == 7) {
		if (gamePause) {
			TetrisResume();
		} else {
			TetrisPause();
		}
		gamePause = !gamePause;
	}

	if (button(GenUIID(0), 1, 4, 1.2, 0.5, "���¿�ʼ(R)") && currentPosition == 7) {
		saveScore();
		TetrisReset();
	}

	if (gameMode==0 && button(GenUIID(0), 1, 3, 1.2, 0.5, "����(G)") && currentPosition == 7) {
		saveScore();
		TetrisStop();
		currentPosition = 11;
		resetGame = 1;
		return;
	}

	if (button(GenUIID(0), 1, 2, 1.2, 0.5, "������һҳ(Esc)") && currentPosition == 7) {
		currentPosition = 2;
		saveScore();
		TetrisReset();
		resetGame = 1;
		bgm2.play();
		return;
	}

	if (button(GenUIID(0), 1, 1, 1.2, 0.5, "�˳���Ϸ(Del)") && currentPosition == 7) {
		saveScore();
		exit(-1);
	}

	setDifficulty(difficultyLevel);
	setGameNumber(gameMode == 1 ? 2 : 1);
	TetrisStart();
	drawTetris();
}

void display() {
	// ����
	DisplayClear();

	//registerKeyboardEvent(KeyboardEventProcess);// ���
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
