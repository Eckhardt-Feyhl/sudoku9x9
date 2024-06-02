/*
 * possibles9.cpp
 *
 *  Created on: 09.04.2024
 *      Author: efeyh
 */
#include <string.h>
#include "possibles9.h"

void possibles9::possibleInit0() {
  possibleCnt = 0;
  memset( possibleLn, 0, sizeof(possibleLn) );
  memset( possibleCol, 0, sizeof(possibleCol) );
}

possibles9::possibles9() {
  possibleInit0();
}

possibles9::~possibles9() {
  possibleInit0();
}

int possibles9::getPossibleCnt() const {
  if (possibleCnt < 0) {
	return -1;
  }
  if (possibleCnt > 9) {
	return -2;
  }
  return (int)possibleCnt;
}

int possibles9::addPossible( char ln, char col ) {
  if (possibleCnt < 0) {
    return -1;
  }
  if (possibleCnt >= 9) {
    return -2;
  }
  if (ln <= 0) {
    return -3;
  }
  if (ln > 9) {
    return -4;
  }
  if (col <= 0) {
    return -5;
  }
  if (col > 9) {
    return -6;
  }
  for (int idx = 0; idx < 9; ++idx) {
    if ((possibleLn[idx]==0) && (possibleCol[idx]==0)) {
      possibleLn[idx] = ln;
      possibleCol[idx] = col;
      ++possibleCnt;
      return possibleCnt;
    }
    if ((possibleLn[idx]==ln) && (possibleCol[idx]==col)) {
      return 0;
    }
  }
  return -7;
}

int possibles9::getPossibleCell( char& ln, char& col, char cnt ) const {
  if (cnt <= 0) {
    return -1;
  }
  if (cnt > 9) {
    return -2;
  }
  ln = possibleLn[cnt-1];
  col = possibleCol[cnt-1];
  if (ln < 0) {
    return -3;
  }
  if (ln == 0) {
    return 0;
  }
  if (ln > 9) {
    return -4;
  }
  if (col < 0) {
    return -5;
  }
  if (col == 0) {
    return 0;
  }
  if (col > 9) {
    return -6;
  }
  return 1;
}

int possibles9::check() const {
  if ((possibleCnt < 1) || (9 < possibleCnt)) {
    return -1;
  }
  for (int idx=0; idx < (int)possibleCnt; ++idx) {
    if ((possibleLn[idx] < 1) || (9 < possibleLn[idx])) {
      return -2;
    }
    if ((possibleCol[idx] < 1) || (9 < possibleCol[idx])) {
      return -3;
    }
  }
  return 0;
}
