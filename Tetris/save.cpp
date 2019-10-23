#include "stdafx.h"
#include "save.h"

/*�������Tetris���е����ݱ������ļ��У�saveNumberΪ�浵���*/
int saveSaveFile(Tetris &tetris, int saveNumber)
{
	FILE *fp;
	errno_t err;

	char name[20] = "save0.save";
	name[4] = saveNumber + '0';

	if ((err = fopen_s(&fp, name, "wb")) != 0) {
		return -1;
	}
	
	fwrite(&tetris, sizeof(Tetris), 1, fp);

	if (fclose(fp)) {
		return -1;
	}
	return 0;
}

/*���浵�е����ݶ�ȡ����һ��������Tetris����*/
int loadSaveFile(Tetris &tetris, int saveNumber)
{
	FILE *fp;
	errno_t err;
	Tetris tmp;

	char name[20] = "save0.save";
	name[4] = saveNumber + '0';

	if ((err = fopen_s(&fp, name, "rb")) != 0) {
		return -1;
	}

	fread(&tmp, sizeof(Tetris), 1, fp);

	if (fclose(fp)) {
		return -1;
	}

	tetris = tmp;
	return 0;
}

int loadRankList(RankList *rankList) {
	FILE *fp;
	errno_t err;
	char name[20] = "rankList.save";

	if ((err = fopen_s(&fp, name, "rb")) != 0) {
		return -1;
	}

	fread(rankList, sizeof(RankList), 3, fp);

	if (fclose(fp)) {
		return -1;
	}

	return 0;
}

int saveRankList(RankList *rankList) {
	FILE *fp;
	errno_t err;

	char name[20] = "rankList.save";

	if ((err = fopen_s(&fp, name, "wb")) != 0) {
		return -1;
	}
	
	fwrite(rankList, sizeof(RankList), 3, fp);

	if (fclose(fp)) {
		return -1;
	}
	return 0;
}