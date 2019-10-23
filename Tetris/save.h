#pragma once

#include "classes.h"
#include <cstdio>
#include <cstring>

extern class RankList;
extern class Tetris;

int saveSaveFile(Tetris&, int);
int loadSaveFile(Tetris&, int);
int loadRankList(RankList *);
int saveRankList(RankList *);
