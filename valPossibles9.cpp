/*
 * valPossibles9.cpp
 *
 *  Created on: 09.04.2024
 *      Author: efeyh
 */
#include <string.h>
#include "valPossibles9.h"

valPossibles9::valPossibles9() {
}

valPossibles9::~valPossibles9() {
}

int valPossibles9::getPossibleCnt(char val) const {
  if (val < 1) {
	return -1;
  }
  if (val > 9) {
	return -2;
  }
  return valPoss[val-1].getPossibleCnt();
}

int valPossibles9::addPossible( char ln, char col, char val ) {
  if (val < 1) {
	return -1;
  }
  if (val > 9) {
	return -2;
  }
  return valPoss[val-1].addPossible(ln, col);
}

int valPossibles9::addPossible( char* pLn, char* pCol, char val, int cnt ) {
  if (pLn == NULL) {
    return -1;
  }
  if (pCol == NULL) {
    return -2;
  }
  if (val < 1) {
	return -3;
  }
  if (val > 9) {
	return -4;
  }
  if (cnt < 1) {
	return -5;
  }
  if (cnt > 9) {
	return -6;
  }
  int ret = 0;
  int idx=0;
  for ( idx=0; idx < cnt; ++idx) {
    ret = valPoss[val-1].addPossible(pLn[idx], pCol[idx]);
    if (ret < 0) {
      return -7;
    }
  }
  return idx; //=cnt
}

int valPossibles9::getPossibleCell( char& ln, char& col, char val, char cnt ) const {
  if (val < 1) {
	return -1;
  }
  if (val > 9) {
    return -2;
  }
  if (cnt < 1) {
    return -3;
  }
  if (cnt > 9) {
    return -4;
  }
  return valPoss[val-1].getPossibleCell( ln, col, cnt );
}

int valPossibles9::check() const {
  int ret = 0;
  for (int idx=0; idx < 9; ++idx) {
    ret = valPoss[idx].check();
    if (ret < 0) {
      return (idx+1);
    }
  }
  return 0;
}
