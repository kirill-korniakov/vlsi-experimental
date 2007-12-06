/* 
 * parser.h
 * this is a part of itlDragon
 * Copyright (C) 2005, ITLab, Aleksey Bader, Zhivoderov
 * email: bader@newmail.ru
 * email: woody-quaker@yandex.ru
 * last modified 30 March 2006
 */

/*
  Parsing functions
*/

#ifndef _PARSER_H_
#define _PARSER_H_

#include <fstream>
#include "..\include\data_structures.h"
#include "..\include\global.h"
#include "..\include\errors.h"
#include <time.h>
using namespace std;

struct strExtend
{
  char name[20];
  int  cellIdx;
};

int findName(str* table, char* name);
int findNameBS(strExtend* A, int Lb, int Ub, char* Key);
int findNameBS(str* A, int Lb, int Ub, char* Key);

void QSortStrExtend(strExtend* a, long N);

ITLDRAGON_ERROR CMDParse(int argc, char* argv[]);
void CfgParse(char* fileName);

ITLDRAGON_ERROR ValidateKeys();
void SetDefaultKeysValues();

int ParseAux(const char* fileName, Circuit& circuit);
int ParseNodes(const char* fileName, Circuit& circuit);
int ParseScl(const char* fileName, Circuit& circuit);
int ParsePl(const char* fileName, Circuit& circuit);
int ParseWts(const char* fileName, Circuit& circuit);
int ParseNets(const char* fileName, Circuit& circuit);

#endif