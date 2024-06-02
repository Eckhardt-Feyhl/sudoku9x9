/*! \brief Alle moeglichen Zahlen einer Zelle
  \details Zu Anfang sind alle Zahlen 1..9 moeglich.
    Mit den Methoden dieser Klasse werden dann Zahlen geloescht,
    so dass am Ende nur noch eine Zahl in dieser Zelle steht.
  \author Eckhardt Feyhl
  \version 1.0.0.0
  \date 14. Apr. 2024
  \copyright no license
  \file
*/
#include <string.h> // memset()
#include "log.h"
#include "cellValues9.h"
#include "sudoku9x9.h"

cellValues9::cellValues9() {
  initAllPossible();
}

cellValues9::cellValues9( char val ) {
  init( val );
}

//virtual
cellValues9::~cellValues9() {
}

void cellValues9::initAllPossible() {
  for (char val=1; val<=9; ++val) {
    cellValue[val-1] = val;
  }
}

void cellValues9::init( char val ) {
  for (int idx = 0; idx < 9; ++idx) {
    cellValue[idx] = val;
  }
}

int cellValues9::getPossibleCnt(char* pVal/*=NULL*/) const {
  int cnt = 0;
  for (int idx=0; idx < 9; ++idx) {
    if ((0 < cellValue[idx]) && (cellValue[idx] <= 9)) {
      if (pVal != NULL) {
        pVal[cnt] = cellValue[idx];
      }
      ++cnt;
    }
  }
  return cnt;
}

char cellValues9::getPossibleValue( char val ) const {
  if ((val < 1) || (9 < val)) {
    return -1;
  }
  if ((cellValue[val-1] < 1) || (9 < cellValue[val-1])) {
    return 0;
  }
  return cellValue[val-1];
}

int cellValues9::clearPossibleValue( char val ) {
  if ((val < 1) || (9 < val)) {
    return -1;
  }
  int ret = 0;
  if (cellValue[val-1] > 0) {
    ret = 1;
  }
  cellValue[val-1] = 0;
  int posCnt = getPossibleCnt();
  if (posCnt < 1) {
    PRINT_ERR("durch Loeschen von Zahl %d ist die Zelle leer\n", (int)val);
    return -2;
  }
  return ret;
}

int cellValues9::clearPossibleValueBut( char val, char* pCleared/*=NULL*/ ) {
  if ((val < 1) || (9 < val)) {
    return -1;
  }
  int cnt = 0;
  for (int idx=0; idx < 9; ++idx) {
	if (cellValue[idx] > 0) {
      if (cellValue[idx] != val) {
        if (pCleared != NULL) {
          pCleared[cnt] = cellValue[idx];
        }
        cellValue[idx] = 0;
        ++cnt;
      }
	}
  }
  return cnt;
}

int cellValues9::clearPossiblesBut( const char* pVal, char* pCleared/*=NULL*/ ) {
  if (pVal == NULL) {
    return -1;
  }
  int cnt = 0;
  for (int idx=0; idx < 9; ++idx) {
	if (cellValue[idx] > 0) {
      if ( inArray9( pVal, cellValue[idx] ) < 1 ) {
        if (pCleared != NULL) {
          pCleared[cnt] = cellValue[idx];
        }
        cellValue[idx] = 0;
        ++cnt;
      }
	}
  }
  return cnt;
}
