/*
 * valCellCollect.cpp
 *
 *  Created on: 09.04.2024
 *      Author: efeyh
 */
#include <string.h>
#include "valCellCollect.h"

void valCellCollect::valCellCollectInit0() {
  memset( values, 0, sizeof(values));
  valCnt = 0;
  memset( lines, 0, sizeof(lines));
  memset( columns, 0, sizeof(columns));
  cellCnt = 0;
}

valCellCollect::valCellCollect() {
  valCellCollectInit0();
}

valCellCollect::~valCellCollect() {
  valCellCollectInit0();
}

int valCellCollect::addValCell( char ln, char col, char val ) {
  if (ln < 1) {
    return -1;
  }
  if (ln > 9) {
    return -2;
  }
  if (col < 1) {
    return -3;
  }
  if (col > 9) {
    return -4;
  }
  if (val < 1) {
    return -5;
  }
  if (val > 9) {
    return -6;
  }
  for (int idx = 0; idx < 9; ++idx) {
    if (values[idx] == val) {
      break; //val ist schon drin
    }
    if (values[idx] == 0) {
      values[idx] = val; //val muss neu rein
      ++valCnt;
      break;
    }
  }
  for (int idx = 0; idx < 9; ++idx) {
    if ((lines[idx] == ln) && (columns[idx] == col)) {
      break; //Zelle ist schon drin
    }
    if ((lines[idx] == 0) && (columns[idx] == 0)) {
      lines[idx] = ln; //Zelle muss neu rein
      columns[idx] = col; //Zelle muss neu rein
      ++cellCnt;
      break;
    }
  }
  if (valCnt == cellCnt) {
    return 0;
  }
  return cellCnt;
}

bool valCellCollect::isValCellCntEqual() const {
  if (valCnt == cellCnt) {
    return true;
  }
  return false;
}

char valCellCollect::getCellCnt() const {
  return cellCnt;
}

char valCellCollect::getValCnt() const {
  return valCnt;
}

const char* valCellCollect::getPossibleLnsPtr() const {
  return lines;
}

const char* valCellCollect::getPossibleColsPtr() const {
  return columns;
}

const char* valCellCollect::getPossibleValsPtr() const {
  return values;
}
