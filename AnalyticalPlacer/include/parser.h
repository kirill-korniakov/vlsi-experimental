/* 
 * parser.h
 * this is a part of itlAnalyticalPlacer
 * Copyright (C) 2005, ITLab, Aleksey Bader, Zhivoderov
 * email: bader@newmail.ru
 * email: zhivoderov.a@gmail.com
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

using namespace DataStructures;

struct strExtend
{
  char* name;      /// Name of cell.
  int  cellIdx;

  strExtend() {name = new char[DEFAULT_NAME_LENGTH]; name[0]=0;}
  strExtend(const char* _name)
  {
    delete[]name;
    name = new char[strlen(_name)+1];
    strcpy(name, _name);
  }
  ~strExtend(){delete[]name;}
  
  void operator=(const char* right)
  {
    if (strlen(right) < DEFAULT_NAME_LENGTH)
    {
      strcpy(name, right);
    }
    else
    {
      delete[]name;
      name = new char[strlen(right)+1];
      strcpy(name, right);
    }
  }

  void operator=(const strExtend& right)
  {
    *this = right.name;
    cellIdx = right.cellIdx;
  }
};

int findName(str* table, char* name);
int findNameBS(strExtend* A, int Lb, int Ub, char* Key);
int findNameBS(str* A, int Lb, int Ub, char* Key);

void QSortStrExtend(strExtend* a, long N);

MULTIPLACER_ERROR CMDParse(int argc, char* argv[]);
void CfgParse(char* fileName);

MULTIPLACER_ERROR ValidateKeys();
void SetDefaultKeysValues();

int ParseAux(const char* fileName, Circuit& circuit);
int ParseNodes(const char* fileName, Circuit& circuit);
int ParseScl(const char* fileName, Circuit& circuit);
int ParsePl(const char* fileName, Circuit& circuit);
int ParseWts(const char* fileName, Circuit& circuit);
int ParseNets(const char* fileName, Circuit& circuit);
int ParseNetWeights(char* fileName, Circuit& circuit);
int ParseDEF(const char* filename, Circuit& circuit);
//int ReparsePl(const char* fileName, str* table, int numOfNT, Place* placement,
//               int nBinRows, int nBinCols);

int ParseLEFDEF(const char* lefname, const char* defname, Circuit& circuit);
int ExportDEF(const char* defname, Circuit& circuit);
#endif