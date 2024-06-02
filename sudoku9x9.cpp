//============================================================================
// Name        : sudoku9x9.cpp
// Author      : Eckhardt Feyhl
// Version     :
// Copyright   : no copyright
// Description : solves 9x9 Sudokus
//============================================================================

//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <unistd.h>
#include <sys/types.h>
//#include <sys/time.h>
#include <conio.h>
#include "log.h"
#include "valPossibles9.h"
#include "valCellCollect.h"
#include "sudoku9x9.h"
#include "in_out.h"

std::list<sudoku9x9> sdList; /*!< Liste fuer probierte Zahlen */

/*! \brief liefert die Anzahl Vorkommnisse von val in pArr9
  \returns <0 bei Fehler \n
    0..9 die Anzahl Vorkommnisse von val in pArr9 */
int inArray9(
  const char* pArr9, //!< [in] Pointer auf die 9 Zahlen
  char val, //!< 0..9 die Zahl deren Vorkommnisse in pArr9 gezaehlt werden soll
  char* pOut9/*=NULL*/ //!< [out] optional die Positionen 1..9 von val in pArr9
  ) {
  if (pArr9 == NULL ) {
    PRINT_ERR( "pArr9==NULL\n" );
    return -1;
  }
  if ((val < 0) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 0..9)\n", (int)val);
    return -2;
  }
  if (pOut9 != NULL) {
    memset( pOut9, 0, 9*sizeof(char));
  }
  int ret = 0;
  for (int idx=0; idx < 9; ++idx) {
    if ( pArr9[idx] == val ) {
      if (pOut9 != NULL) {
        pOut9[ret] = idx+1;
      }
      ++ret;
    }
  }
  return ret;
}

bool isSameLnCol( char* pLnCols ) {
  if (pLnCols == NULL) {
	PRINT_ERR("pLnCols==NULL\n");
    return false;
  }
  char fst = 0;
  for (int idx=0; idx < 9; ++idx) {
    if (pLnCols[idx] < 1) {
      continue; //ungueltig - ignore
    }
    if (9 < pLnCols[idx]) {
      continue; //ungueltig - ignore
    }
    if (fst <= 0) {
      fst = pLnCols[idx];
    } else if (pLnCols[idx] != fst) {
      return false;
    }
  }
  if (fst <= 0) {
	PRINT_ERR("kein gueltiger Wert in lnCols gefunden\n");
    return false;
  }
  return true;
}

bool isSameQuad( char* pLnCol ) {
  if (pLnCol == NULL) {
	PRINT_ERR("pLnCol==NULL\n");
    return false;
  }
  char fstIdx = -1;
  for (int idx=0; idx < 9; ++idx) {
    if ((pLnCol[idx] < 1) || (9 < pLnCol[idx])) {
      continue; //ungueltig - ignore
    }
    if (fstIdx < 0) {
      fstIdx = ((pLnCol[idx] - 1) / 3) * 3;
    } else if ((((pLnCol[idx]-1) / 3) * 3) != fstIdx) {
      return false;
    }
  }
  if (fstIdx < 0) {
	PRINT_ERR("kein gueltiger Wert in pLnCol gefunden\n");
    return false;
  }
  return true;
}

sudoku9x9::sudoku9x9() {
  errorCell.init( -1 );
}

sudoku9x9::~sudoku9x9() {
}

int sudoku9x9::clearPossibleVal(int ln, int col, int val, bool show/*=true*/) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  int ret = allCells[ln-1][col-1].clearPossibleValue( val );
  if (ret < 0) {
    PRINT_ERR("allCells[ln=%d][col=%d].clearPossibleValue( val=%d )returned %d\n",
      ln, col, (int)val, ret);
    return -4;
  }
  if (ret == 0) {
    return 0;
  }
  if ( show ) {
    printf("sudoku9x9::%s(): ln=%d col=%d val=%d geloescht\n", __func__, ln, col, val);
  }
  char posVals[9];
  memset( &posVals[0], 0, sizeof(posVals));
  ret = getPossibleCnt(ln, col, posVals);
  if (ret < 1) {
    PRINT_ERR("getPossibleCnt(ln=%d, col=%d) returned %d\n", ln, col, ret);
    return -5;
  }
  if (ret == 1) {
    lnColValue lcv(ln, col, posVals[0]);
    onlyOne.push_back( lcv );
    printf("%s(): Zahl %d jetzt alleine in (ln=%d, col=%d)\n",
      __func__, (int)posVals[0], (int)ln, (int)col);
  }
  return 1;
}

int sudoku9x9::clearPossibleValsBut(int ln, int col, const char* pVal) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if (pVal == NULL) {
    PRINT_ERR("pVal == NULL\n");
    return -3;
  }
  int rmCnt = 0;
  for (char val = 1; val <= 9; ++val) {
    if ( getPossibleVal( ln, col, val ) > 0 ) {
      if ( inArray9(pVal, val) < 1 ) { // val ist nicht in pVal-array enthalten
        rmCnt += clearPossibleVal(ln, col, val);
	  }
	}
  }
  return rmCnt;
}

int sudoku9x9::clearOtherValues(const char* ln, const char* col, const char* vals, int maxNums) {
  //yes - N Zahlen in N Zellen
  int ret = 0;
  int rmCnt = 0;
  for (int idx=0; idx < maxNums; ++idx) {
    ret = clearPossibleValsBut( ln[idx], col[idx], vals);
    if (ret < 0) {
      PRINT_ERR("clearPossibleValsBut(ln=%d col=%d) returned %d\n",
        (int)ln[idx], (int)col[idx], ret);
      return -1;
    }
    rmCnt += ret;
  }
  if (rmCnt > 0) {
    printf("%d Zellen, %d Zahlen:\n", maxNums, maxNums);
    for (int idx=0; idx < maxNums; ++idx) {
      printf("ln=%d col=%d val=%d\n", (int)ln[idx], (int)col[idx], (int)vals[idx]);
    }
    int ret = print();
    if (ret < 0) {
      PRINT_ERR("print() returned %d\n", ret);
      return -2;
    }
  }
  return rmCnt;
}

int sudoku9x9::clearOtherValues(const valCellCollect& vcc) {
  if ( !vcc.isValCellCntEqual() ) {
	return 0;
  }
  //yes - N Zahlen in N Zellen
  int maxNums = vcc.getValCnt();
  const char* pLns = vcc.getPossibleLnsPtr();
  const char* pCols = vcc.getPossibleColsPtr();
  const char* pVals = vcc.getPossibleValsPtr();
  int ret = clearOtherValues(pLns, pCols, pVals, maxNums);
  if (ret < 0) {
    PRINT_ERR("clearOtherValues() returned %d\n", ret);
    return -1;
  }
  return ret;
}

int sudoku9x9::insertValCells(valCellCollect& vcc, const valPossibles9& vp, char val) {
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d - (expected 1..9)\n", (int)val);
    return -1;
  }
  int valCnt = vp.getPossibleCnt( val );
  if ((valCnt < 2) || (8 < valCnt)) {
    return 0;
  }
  int ret = 0;
  int insertCnt = 0;
  char ln = 0;
  char col = 0;
  for (int idx=0; idx < valCnt; ++idx) {
	ret = vp.getPossibleCell(ln, col, val, idx+1);
    if (ret < 0) {
      printf("%s(): vp.getPossibleCell(val=%d cnt=%d) returned %d\n",
        __func__, (int)val, idx+1, ret);
      return -2;
    }
    ret = vcc.addValCell( ln, col, val);
    if (ret < 0) {
      printf("%s(): vcc.addValCell(ln=%d, col=%d, val=%d) returned %d\n",
        __func__, (int)ln, (int)col, (int)val, ret);
      return -3;
    }
    ++insertCnt;
  }
  return insertCnt;
}

int sudoku9x9::insertNextValIntoLnCol(valCellCollect& collect, const valPossibles9& valPos, char startVal) {
  if ((startVal < 1) || (9 < startVal)) {
    PRINT_ERR("startVal=%d - (expected 1..9)\n", startVal);
    return -1;
  }
  valCellCollect newCollect;
  int valCnt = 0;
  char ln = 0;
  char col = 0;
  int ret = 0;
  int rmCnt = 0;
  for (char val = startVal; val <= 9; ++val ) {
	newCollect = collect;
	valCnt = valPos.getPossibleCnt( val );
	if ((valCnt < 1) || (9 < valCnt)) {
      PRINT_ERR("val=%d valCnt=%d (expected 1..9)\n", val, valCnt);
      return -10;
	}
	if (valCnt == 1) {
	  valPos.getPossibleCell( ln, col, val, 1);
	  if ((ln < 1) || (9 < ln)) {
	    PRINT_ERR("valCnt for val %d == 1 but pValLn=%d - (expected 1..9)\n",
          (int)val, (int)ln);
	    return -11;
      }
	  if ((col < 1) || (9 < col)) {
        PRINT_ERR("valCnt for val %d == 1 but pValCol=%d - (expected 1..9)\n",
          (int)val, (int)col);
	    return -12;
      }
	  ret = setKnownValue(ln, col, val, false);
      if (ret < 0) {
        PRINT_ERR("setNum(ln=%d, col=%d, val=%d) returned %d\n",
          (int)ln, (int)col, (int)val, ret);
        return -13;
      }
      if (ret == 0) { //keine Veraenderung
        continue; // for val 1..9
      }
      rmCnt += ret;
      printf("%s(): Zahl %d in Zeile %d und Spalte %d gesetzt\n",
        __func__, val, ln, col);
      ret = print();
      if (ret < 0) {
        PRINT_ERR("print() returned %d\n", ret);
        return -14;
      }
      return rmCnt; //wir haben mindestens eine Zelle veraendert
      //die Liste valPos muss erst wieder aktualisiert werden
	}
	ret = insertValCells( newCollect, valPos, val);
	if (ret < 0) {
      PRINT_ERR("insertValCells(val=%d) returned %d\n",
        (int)val, ret);
      return -15;
	}
	if ( newCollect.isValCellCntEqual() ) {
      ret = clearOtherValues( newCollect );
      if (ret < 0) {
        PRINT_ERR("clearOtherValues(newCollect) returned %d\n", ret);
    	return -14;
      }
      return ret;
	}
	//passt noch nicht - also naechste Zahl
    if (val >= 9){ //alle Zahlen durch - fertig
      return 0;
	}
    ret = insertNextValIntoLnCol( newCollect, valPos, val+1);
    if (ret < 0) {
      PRINT_ERR("insertNextValIntoLnCol(val=%d) returned %d\n",
        val+1, ret);
      return -15;
    }
  } //for val=startVal .. 9
  return rmCnt;
}

int sudoku9x9::clearCellN_v3(const valPossibles9& vp9) {
  int ret = vp9.check();
  if (ret < 0) {
    PRINT_ERR("vp9.check() returned %d\n", ret);
    return -1;
  }
  valCellCollect vcc;
  ret = insertNextValIntoLnCol( vcc, vp9, 1);
  if (ret < 0) {
    PRINT_ERR("insertNextValIntoLnCol(val=1) returned %d\n", ret);
    return -10;
  }
  return ret;
}

int sudoku9x9::clearLineNum(int ln, int col, char val, bool show) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  int cnt = 0;
  char num = 0; //Inhalt einer Zelle
  int ret = 0;
  //val aus der Zeile loeschen
  for (int column=1; column <= 9; ++column) {
    if (column != col) {
      num = getPossibleVal(ln, column, val);
      if (num == val) {
        ++cnt;
        ret = clearPossibleVal(ln, column, (int)num, show);
        if (ret < 0) {
          PRINT_ERR("clearPossibleVal() returned %d\n", ret);
          return -4;
        }
      }
    }
  }
  return cnt;
}

int sudoku9x9::clearLineButCols(int ln, const char* pCol, char val) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if (pCol == NULL) {
    PRINT_ERR("pCol==NULL\n");
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  int colIdx = 0;
  if ((pCol[0] < 1) || (9 < pCol[0])) {
    PRINT_ERR("pCol[0]=%d (allowed 1..9)\n", (int)pCol[0]);
    return -4;
  }
  int cnt = 0;
  char num = 0; //Inhalt einer Zelle
  int ret = 0;
  //val aus der Zeile loeschen
  for (int column=1; column <= 9; ++column) {
    if (column != pCol[colIdx]) {
      num = getPossibleVal(ln, column, val);
      if (num == val) {
        ++cnt;
        ret = clearPossibleVal(ln, column, num);
        if (ret < 0) {
          PRINT_ERR("clearPossibleVal() returned %d\n", ret);
          return -5;
        }
      }
    } else {
      ++colIdx;
    }
  }
  return cnt;
}

int sudoku9x9::valCntLn( char* pCol, int ln, char val) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (expected 1..9)\n", ln);
    return -1;
  }
  if ((val < 0) || (9 < val)) {
    PRINT_ERR("val=%d (expected 0..9)\n", (int)val);
    return -2;
  }
  if (pCol != NULL) {
    memset( pCol, 0, 9*sizeof(char) );
  }
  int ret = 0;
  for (int col=1; col <= 9; ++col) {
	if (val == 0) {
      if ( getPossibleCnt(ln, col) > 1) {
        if (pCol != NULL) {
          pCol[ret] = (char)col;
        }
    	++ret;
      }
	} else if ( getPossibleVal(ln, col, val) > 0) {
      if (pCol != NULL) {
        pCol[ret] = (char)col;
      }
      ++ret;
    }
  }
  if (ret < 1) {
	if (val > 0) {
      PRINT_ERR("Zahl %d nicht mehr in Zeile %d enthalten\n", (int)val, ln);
      return -3;
	}
  }
  return ret;
}

int sudoku9x9::solveLine( int ln ) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (expected 1..9)\n", ln);
    return -1;
  }
  int ret = 0;
  int rm = 0;
  int cnt = 0;
  char aLn[9];
  memset(aLn, ln, sizeof(aLn)); //alle Zellen sind in dieser Zeile
  char aCol[9];
  valPossibles9 vp9;
  for (char val=1; val<=9; ++val) {
    memset(aCol, 0, sizeof(aCol));
    cnt = valCntLn( aCol, ln, val);
    if (cnt < 1) {
      PRINT_ERR("Zahl %d nicht mehr in Zeile %d enthalten\n", (int)val, ln);
      return -3;
    }
    vp9.addPossible( aLn, aCol, val, cnt);
    if (cnt == 1) {
      rm = setKnownValue( ln, aCol[0], val, false );
      if (rm < 0) {
        PRINT_ERR("setKnownValue(ln=%d col=%d val=%d) returned %d\n",
          ln, (int)aCol[0], (int)val, rm);
    	return -4;
      }
      if (rm > 0) {
        printf("Zahl %d nur noch einmal in Zeile %d enthalten\n",
          (int)val, ln);
        ret = print();
        if (ret < 0) {
          PRINT_ERR("print() returned %d\n", ret);
          return -5;
        }
        return rm;
      }
    } else if ( cnt <= 3 ) {
      if ( isSameQuad( aCol ) ) {
        rm = clearQuadNumLn(ln, aCol[0], val);
        if (rm < 0) {
          PRINT_ERR("clearQuadNumLn(ln=%d col=%d val=%d) returned %d\n",
            ln, (int)aCol[0], (int)val, rm);
          return -6;
        }
        if (rm > 0) {
          printf("Zahl %d in Zeile %d ist %d mal im selben Quadrant\n",
            (int)val, ln, cnt);
          ret = print();
          if (ret < 0) {
            PRINT_ERR("print() returned %d\n", ret);
            return -7;
          }
          return rm;
        }
      }
    }
  }
  rm = clearCellN_v3( vp9 );
  if (rm < 0) {
    PRINT_ERR("clearCellN_v3() returned %d\n", rm);
    return -8;
  }
  return rm;
}

int sudoku9x9::solveLines() {
  int ret = 0;
  for (int ln=1; ln<=9; ++ln) {
    ret = solveLine( ln );
    if (ret < 0) {
      PRINT_ERR("solveLine(ln=%d) returned %d\n", ln, ret);
      return -1;
    }
    if (ret > 0) {
      return ret;
    }
  } //for ln
  return 0;
}

int sudoku9x9::clearColNum(int ln, int col, char val, bool show/*=true*/) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  int ret = 0;
  char num = 0; //Inhalt einer Zelle
  int cnt = 0;
  //val aus der Spalte loeschen
  for (int line=1; line <= 9; ++line) {
    if (line != ln) {
      num = getPossibleVal(line, col, val);
      if (num == val) {
        ++cnt;
        ret = clearPossibleVal(line, col, num, show);
        if (ret < 0) {
          PRINT_ERR("clearPossibleVal() returned %d\n", ret);
          return -4;
        }
      }
    }
  }
  return cnt;
}

int sudoku9x9::clearColButLns(int col, char* pLn, char val) {
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -1;
  }
  if (pLn == NULL) {
    PRINT_ERR("pLn==NULL\n");
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  if ((pLn[0] < 1) || (9 < pLn[0])) {
    PRINT_ERR("pLn[0]=%d (allowed 1..9)\n", (int)pLn[0]);
    return -4;
  }
  int lnIdx = 0;
  int ret = 0;
  int cnt = 0;
  char num = 0; //Inhalt einer Zelle
  //val aus der Zeile loeschen
  for (int line=1; line <= 9; ++line) {
    if (line != pLn[lnIdx]) {
      num = getPossibleVal(line, col, val);
      if (num == val) {
        ++cnt;
        ret = clearPossibleVal(line, col, num);
        if (ret < 0) {
          PRINT_ERR("clearPossibleVal() returned %d\n", ret);
          return -5;
        }
      }
    } else {
      ++lnIdx;
    }
  }
  return cnt;
}

int sudoku9x9::valCntCol( char* pLn, int col, char val) {
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (expected 1..9)\n", col);
    return -1;
  }
  if ((val < 0) || (9 < val)) {
    PRINT_ERR("val=%d (expected 0..9)\n", (int)val);
    return -2;
  }
  if (pLn != NULL) {
    memset( pLn, 0, 9*sizeof(char) );
  }
  int ret = 0;
  for (int ln=1; ln <= 9; ++ln) {
	if (val == 0) {
      if ( getPossibleCnt(ln, col) > 1) {
        if (pLn != NULL) {
          pLn[ret] = (char)ln;
        }
    	++ret;
      }
	} else if ( getPossibleVal(ln, col, val) > 0) {
      if (pLn != NULL) {
        pLn[ret] = (char)ln;
      }
      ++ret;
    }
  }
  if (ret < 1) {
	if (val > 0) {
      PRINT_ERR("Zahl %d nicht mehr in Spalte %d enthalten\n", (int)val, col);
      return -3;
	}
  }
  return ret;
}

int sudoku9x9::solveCol( int col ) {
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (expected 1..9)\n", col);
    return -2;
  }
  int ret = 0;
  int rm = 0;
  int cnt = 0;
  char aLn[9]; //die Liste der Zeilen in der die Zahl vorkommt
  char aCol[9]; //die Liste der Spalten in der die Zahl vorkommt
  memset( aCol, col, sizeof(aCol)); //hier immer die Spalte col
  valPossibles9 vp9;
  for (char val=1; val<=9; ++val) {
    memset( aLn, 0, sizeof(aLn));
    cnt = valCntCol( aLn, col, val);
    if (cnt < 1) {
      PRINT_ERR("Zahl %d nicht mehr in Spalte %d enthalten\n", (int)val, col);
      return -3;
    }
    vp9.addPossible( aLn, aCol, val, cnt);
    if ((cnt == 1)) {
      rm = setKnownValue( (int)aLn[0], col, val, false );
      if (rm < 0) {
        PRINT_ERR("setNum(ln=%d col=%d val=%d) returned %d\n",
          (int)aLn[0], col, (int)val, rm);
    	return -4;
      }
      if (rm > 0) {
        printf("Zahl %d nur noch einmal in Spalte %d enthalten\n", (int)val, col);
        ret = print();
        if (ret < 0) {
          PRINT_ERR("print() returned %d\n", ret);
          return -5;
        }
        return rm;
      }
    } else if ( cnt <= 3 ) {
      if ( isSameQuad( &aLn[0] ) ) {
        rm = clearQuadNumCol(aLn[0], col, val);
        if (rm < 0) {
          PRINT_ERR("clearQuadNumCol(ln=%d col=%d val=%d) returned %d\n",
            (int)aLn[0], col, (int)val, rm);
          return -5;
        }
        if (rm > 0) {
          printf("Zahl %d ist in Spalte %d %d mal im selben Quadrant\n",
        		  (int)val, col, cnt);
          ret = print();
          if (ret < 0) {
            PRINT_ERR("print() returned %d\n", ret);
            return -5;
          }
          return rm;
        }
      }
    }
  }
  rm = clearCellN_v3( vp9 );
  if (rm < 0) {
    PRINT_ERR("clearCellN_v3() returned %d\n", rm);
    return -6;
  }
  return rm;
}

int sudoku9x9::solveCols() {
  int ret = 0;
  for (int col=1; col<=9; ++col) {
    ret = solveCol( col);
    if (ret < 0) {
      PRINT_ERR("solveCol(col=%d) returned %d\n", col, ret);
      return -1;
    }
    if (ret > 0) {
      return ret;
    }
  } //for col
  return 0;
}

int sudoku9x9::getQuadIdx( int lnIdx, int colIdx ) {
  if ((lnIdx < 0) || (8 < lnIdx)) {
    PRINT_ERR("lnIdx=%d (allowed 0..8)\n", (int)lnIdx);
    return -1;
  }
  if ((colIdx < 0) || (8 < colIdx)) {
    PRINT_ERR("colIdx=%d (allowed 0..8)\n", (int)colIdx);
    return -2;
  }
  char idx = (lnIdx / 3)*3; //0,3,6
  idx += (colIdx / 3); //0,1,2
  return idx;
}

int sudoku9x9::getQuadCellArrIdx( int lnIdx, int colIdx ) {
  if ((lnIdx < 0) || (8 < lnIdx)) {
    PRINT_ERR("lnIdx=%d (allowed 0..8)\n", (int)lnIdx);
    return -1;
  }
  if ((colIdx < 0) || (8 < colIdx)) {
    PRINT_ERR("colIdx=%d (allowed 0..8)\n", (int)colIdx);
    return -2;
  }
  char lnStart = (lnIdx / 3)*3; //0,3,6
  char colStart = (colIdx / 3)*3; //0,3,6
  char lidx = lnIdx - lnStart; //0,1,2
  char cidx = colIdx - colStart; //0,1,2
  char idx = lidx * 3 + cidx;
  return idx;
}

char sudoku9x9::getPossibleVal(int ln, int col, char val) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  cellValues9& cell = getCell( ln, col );
  return cell.getPossibleValue( val );
}

int sudoku9x9::getPossibleCnt(int ln, int col, char* pVals/*=NULL*/) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  int cnt = 0;
  int pvIdx = 0;
  for (char val=1; val <= 9; ++val) {
	if ( getPossibleVal(ln, col, val) > 0) {
	  if (pVals != NULL) {
		pVals[pvIdx++] = val;
	  }
	  ++cnt;
	}
  }
  if (cnt < 1) {
	PRINT_ERR("In Zelle ln=%d, col=%d ist keine Zahl mehr eingetragen\n", ln, col);
	return -3;
  }
  return cnt;
}

/*! \brief Gibt das Sudoku auf dem Bildschirm aus */
int sudoku9x9::print() {
  char allPossibles[9*9*9];
  for (int lnIdx=0; lnIdx < 9; ++lnIdx) {
    for (int colIdx=0; colIdx < 9; ++colIdx) {
      for (int valIdx=0; valIdx < 9; ++valIdx) {
        allPossibles[lnIdx*81+colIdx*9+valIdx] = getPossibleVal(lnIdx+1, colIdx+1, valIdx+1);
      }
    }
  }
  printSudoku( &allPossibles[0] );
  int ret = 0;
  while ( !onlyOne.empty() ) {
    lnColValue lcv = onlyOne.front();
    onlyOne.pop_front();
    // ACHTUNG Rekursion!!!!
    ret = setKnownValue( lcv.line, lcv.column, lcv.value, true);
    if (ret < 0) {
      PRINT_ERR("setKnownValue(ln=%d, col=%d, val=%d returned %d\n",
        (int)lcv.line, (int)lcv.column, (int)lcv.value, ret);
      return -1;
    }
  }
  return 0;
}

cellValues9& sudoku9x9::getCell( int ln, int col ) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return errorCell;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return errorCell;
  }
  return allCells[ln-1][col-1];
}

int sudoku9x9::clearQuadNumLn(int ln, int col, char val) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  int firstLn = (((ln-1) / 3) * 3) + 1;
  int firstCol = (((col-1) / 3) * 3) + 1;
  int ret = 0;
  int cnt = 0;
  char num = 0; //Inhalt einer Zelle
  for (int li=0; li < 3; ++li) {
    for (int ci=0; ci < 3; ++ci) {
      if ((firstLn+li) == ln) {
        continue;
      }
      num = getPossibleVal(firstLn+li, firstCol+ci, val);
      if (num == val) {
        ++cnt;
        ret = clearPossibleVal(firstLn+li, firstCol+ci, num);
        if (ret < 0) {
          PRINT_ERR("clearPossibleVal() returned %d\n", ret);
          return -5;
        }
      }
    }
  }
  return cnt;
}

int sudoku9x9::clearQuadNumCol(int ln, int col, char val) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  int firstLn = (((ln-1) / 3) * 3) + 1;
  int firstCol = (((col-1) / 3) * 3) + 1;
  int ret = 0;
  int cnt = 0;
  char num = 0; //Inhalt einer Zelle
  for (int li=0; li < 3; ++li) {
    for (int ci=0; ci < 3; ++ci) {
      if ((firstCol+ci) == col) {
        continue;
      }
      num = getPossibleVal(firstLn+li, firstCol+ci, val);
      if (num == val) {
        ++cnt;
        ret = clearPossibleVal(firstLn+li, firstCol+ci, num);
        if (ret < 0) {
          PRINT_ERR("clearPossibleVal() returned %d\n", ret);
          return -4;
        }
      }
    }
  }
  return cnt;
}

int sudoku9x9::clearQuadNum(int ln, int col, char val, bool show/*=true*/) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", val);
    return -3;
  }
  int firstLn = (((ln-1) / 3) * 3) + 1;
  int firstCol = (((col-1) / 3) * 3) + 1;
  int ret = 0;
  int cnt = 0;
  char num = 0; //Inhalt einer Zelle
  for (int li=0; li < 3; ++li) {
    for (int ci=0; ci < 3; ++ci) {
      if ((firstLn+li) == ln) {
        if ((firstCol+ci) == col) {
          continue;
        }
      }
      num = getPossibleVal(firstLn+li, firstCol+ci, val);
      if (num == val) {
        ++cnt;
        ret = clearPossibleVal(firstLn+li, firstCol+ci, val, show);
        if (ret < 0) {
          PRINT_ERR("clearPossibleVal() returned %d\n", ret);
          return -4;
        }
      }
    }
  }
  return cnt;
}

int sudoku9x9::setKnownValue( int ln, int col, char val, bool show/*=true*/ ) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (allowed 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (allowed 1..9)\n", col);
    return -2;
  }
  if ((val < 1) || (9 < val)) {
    PRINT_ERR("val=%d (allowed 1..9)\n", (int)val);
    return -3;
  }
  cellValues9& cell = getCell(ln, col);
  int ret = 0;
  int rmCnt = 0;
  ret = cell.clearPossibleValueBut( val );
  if (ret < 0) {
    PRINT_ERR("cell.clearPossibleValueBut(val=%d) returned %d\n", (int)val, ret);
    return -4;
  }
  rmCnt += ret;
  ret = clearLineNum( ln, col, val, show);
  if (ret < 0) {
    PRINT_ERR("clearLineNum(ln=%d, col=%d, val=%d) returned %d\n", ln, col, (int)val, ret);
    return -5;
  }
  rmCnt += ret;
  ret = clearColNum(ln, col, val, show);
  if (ret < 0) {
    PRINT_ERR("clearColNum(ln=%d, col=%d, val=%d) returned %d\n", ln, col, (int)val, ret);
    return -6;
  }
  rmCnt += ret;
  ret = clearQuadNum( ln, col, val, show );
  if (ret < 0) {
    PRINT_ERR("clearQuadNum(ln=%d, col=%d, val=%d) returned %d\n", ln, col, (int)val, ret);
    return -7;
  }
  rmCnt += ret;
  if (rmCnt > 0) {
    if ( show ) {
      ret = print();
      if (ret < 0) {
        PRINT_ERR("print() returned %d\n", ret);
        return -8;
      }
    }
  }
  return rmCnt;
}

int sudoku9x9::setKnownValues(const char* pVal81) {
  if (pVal81 == NULL) {
    PRINT_ERR("pVal81==NULL\n");
    return -1;
  }
  int ret = 0;
  int rmCnt = 0;
  char val = 0;
  for (int ln=1; ln<=9; ++ln) {
	for (int col=1; col<=9; ++col) {
      val = pVal81[(ln-1)*9+col-1];
      if ((1 <= val) && (val <= 9)) {
        ret = setKnownValue( ln, col, val, false);
        if (ret < 0) {
   	      PRINT_ERR("setKnownValue(ln=%d, col=%d, val=%d) returned %d\n",
            ln, col, (int)val, ret);
   	      return -2;
        }
        rmCnt += ret;
      }
	}
  }
  return rmCnt;
}

int sudoku9x9::valCntQuad( char* pLn, char* pCol, int ln, int col, char val) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (expected 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (expected 1..9)\n", col);
    return -2;
  }
  if ((val < 0) || (9 < val)) {
    PRINT_ERR("val=%d (expected 0..9)\n", (int)val);
    return -3;
  }
  if (pLn != NULL) {
    memset( pLn, 0, 9*sizeof(char) );
  }
  if (pCol != NULL) {
    memset( pCol, 0, 9*sizeof(char) );
  }
  int ret = 0;
  int fstLn = (((ln-1) / 3) * 3) + 1;
  if ((fstLn < 1) || (7 < fstLn)) {
	PRINT_ERR("fstLn = %d (ln=%d) - expected 1..7\n", fstLn, ln);
	return -4;
  }
  int fstCol = (((col-1) / 3) * 3) + 1;
  if ((fstCol < 1) || (7 < fstCol)) {
	PRINT_ERR("fstCol = %d (col=%d) - expected 1..7\n", fstCol, col);
	return -5;
  }
  for (int cellIdx=0; cellIdx < 9; ++cellIdx) {
    ln = fstLn + (cellIdx / 3);
    if ((ln < 1) || (9 < ln)) {
      PRINT_ERR("ln = %d (fstLn=%d, cellIdx=%d) - expected 1..9\n", ln, fstLn, cellIdx);
      return -6;
    }
    col = fstCol + (cellIdx % 3);
    if ((col < 1) || (9 < col)) {
      PRINT_ERR("col = %d (fstCol=%d, cellIdx=%d) - expected 1..9\n", col, fstCol, cellIdx);
      return -7;
    }
	if (val == 0) {
      if ( getPossibleCnt(ln, col) > 1) {
        if (pLn != NULL) {
          pLn[ret] = (char)ln;
        }
        if (pCol != NULL) {
          pCol[ret] = (char)col;
        }
    	++ret;
      }
	} else if ( getPossibleVal(ln, col, val) > 0) {
      if (pLn != NULL) {
        pLn[ret] = (char)ln;
      }
      if (pCol != NULL) {
        pCol[ret] = (char)col;
      }
      ++ret;
    }
  }
  if (ret < 1) {
	if (val > 0) {
      PRINT_ERR("Zahl %d nicht mehr im Quadrant enthalten (ln=%d, col=%d)\n",
        (int)val, fstLn, fstCol);
      return -4;
	}
  }
  return ret;
}

int sudoku9x9::solveQuad( int ln, int col) {
  if ((ln < 1) || (9 < ln)) {
    PRINT_ERR("ln=%d (expected 1..9)\n", ln);
    return -1;
  }
  if ((col < 1) || (9 < col)) {
    PRINT_ERR("col=%d (expected 1..9)\n", col);
    return -2;
  }
  int ret = 0;
  int rm = 0;
  int cnt = 0;
  char aLn[9]; //die Liste der Zeilen in der die Zahl vorkommt
  char aCol[9]; //die Liste der Spalten in der die Zahl vorkommt
  valPossibles9 vp9;
  for (char val=1; val<=9; ++val) {
    memset( aLn, 0, sizeof(aLn));
    memset( aCol, 0, sizeof(aCol));
    cnt = valCntQuad( aLn, aCol, ln, col, val);
    if (cnt < 1) {
      PRINT_ERR("Zahl %d nicht mehr im Quadrant ln=%d col=%d enthalten\n",
        (int)val, ln, col);
      return -3;
    }
    vp9.addPossible( aLn, aCol, val, cnt);
    if (cnt == 1) {
      rm = setKnownValue( aLn[0], aCol[0], val, false );
      if (rm < 0) {
        PRINT_ERR("setNum(ln=%d col=%d val=%d) returned %d\n",
          (int)aLn[0], (int)aCol[0], (int)val, rm);
        return -4;
      }
      if (rm > 0) {
        printf("Zahl %d nur noch einmal im Quadrant ln=%d col=%d enthalten\n",
          (int)val, (int)aLn[0], (int)aCol[0]);
        ret = print();
        if (ret < 0) {
          PRINT_ERR("print() returned %d\n", ret);
          return -5;
        }
        return rm;
      }
    } else if ( cnt <= 3 ) {
      if ( isSameLnCol( &aLn[0] ) ) {
        rm = clearLineButCols( aLn[0], aCol, val);
        if (rm < 0) {
          PRINT_ERR("clearLineButCols(ln=%d val=%d) returned %d\n",
            (int)aLn[0], (int)val, rm);
          return -6;
        }
        if (rm > 0) {
          printf("Zahl %d im Quadrant ln=%d col=%d liegen alle in der Zeile %d\n",
        		  (int)val, ln, col, (int)aLn[0]);
          ret = print();
          if (ret < 0) {
            PRINT_ERR("print() returned %d\n", ret);
            return -7;
          }
          return rm;
        }
      } else if ( isSameLnCol( aCol ) ) {
        rm = clearColButLns( aCol[0], aLn, val);
        if (rm < 0) {
          PRINT_ERR("clearLineButLns(col=%d val=%d) returned %d\n",
            (int)aCol[0], (int)val, rm);
          return -8;
        }
        if (rm > 0) {
          printf("Zahl %d im Quadrant ln=%d col=%d liegen alle in der Spalte %d\n",
        		  (int)val, ln, col, (int)aCol[0]);
          ret = print();
          if (ret < 0) {
            PRINT_ERR("print() returned %d\n", ret);
            return -9;
          }
          return rm;
        }
      }
    }
  }
  rm = clearCellN_v3(vp9);
  if (rm < 0) {
    PRINT_ERR("clearCellN_v3() returned %d\n", rm);
    return -8;
  }
  return rm;
}

int sudoku9x9::solveQuads() {
  int ret = 0;
  for (int col=1; col<=9; col+=3) {
    for (int ln=1; ln<=9; ln+=3) {
      ret = solveQuad( ln, col );
      if (ret < 0) {
        PRINT_ERR("solveQuad(ln=%d col=%d) returned %d\n", ln, col, ret);
        return -1;
      }
      if (ret > 0) {
        return ret;
      }
    } //for ln
  } //for col
  return 0;
}

/*! \brief Loest das Sudoku
  \returns 0 wenn es geloest wurde
           ansonsten die Anzahl ungeloester Zellen */
int sudoku9x9::solve() {
  int ret = 0;
  int rmCnt = 0; //Summe aller geloeschter moeglichen Zahlen in einem Durchlauf
  do {
	ret = solveQuads();
	if (ret < 0) {
	  PRINT_ERR("solveQuads() returned %d\n", ret);
	  return -1;
	}
	rmCnt = ret;
	ret = solveLines();
	if (ret < 0) {
	  PRINT_ERR("solveLines() returned %d\n", ret);
	  return -2;
	}
	rmCnt += ret;
    ret = solveCols();
	if (ret < 0) {
	  PRINT_ERR("solveCols() returned %d\n", ret);
	  return -3;
	}
	rmCnt += ret;
  }while (rmCnt > 0);
  int unsolvedCnt = 0;
  int pc = 0;
  for (int ln=1; ln<=9; ++ln) {
    for (int col=1; col<=9; ++col) {
      pc = getPossibleCnt(ln, col);
      if ( pc > 1 ) {
    	++unsolvedCnt;
      }
    }
  }
  return unsolvedCnt;
}

/*! \brief Einsprungfunktion
  \details usage: \n
    sudoku9x9.exe [filename] \n
    Aufruf ohne filename: die bekannten Zahlen der Zellen werden ueber die Console eingelesen \n
    Aufruf mit filename: die bekannten Zahlen der Zellen werden von filename eingelesen \n
    filename Format: 9 Zeilen mit je 9 Zahlen 0..9. \n
      Die 0 steht fuer eine noch nicht bekannte Zelle. */
int main(
  int argc, //!< Anzahl Argumente in argv. 1=keine Argumente - in argv[0] steht der Programmname
  char* argv[] //!< Die Aufruf-Argumente. argc=2: in argv[1] steht [filename]
  ) {
  char kvs[81]; //known values
  memset(kvs, 0, sizeof(kvs)); //alle Zellen sind erstmal unbekannt
  int ret = readKnownValues(kvs, argc, argv); //von Console bzw. Datei
  if (ret < 0) {
    PRINT_ERR("readKnownValues() returned %d\n", ret);
    return -1;
  }
  sudoku9x9 sudoku;
  ret = sudoku.setKnownValues( kvs ); //die bekannten Zahlen einsetzen
  if (ret < 0) {
	PRINT_ERR("sudoku.setKnownValues() returned %d\n", ret);
    return -2;
  }
  do {
    ret = sudoku.print(); //auf Console ausgeben
    if (ret < 0) {
      PRINT_ERR("sudoku.print() returned %d\n", ret);
      return -3;
    }
    ret = sudoku.solve(); //liefert die Anzahl ungeloester Zellen zurueck
    if (ret < 0) {
	  PRINT_ERR("sudoku.solve() returned %d\n", ret);
	  if ( !rdRollBack( !sdList.empty() ) ) {
	    return 2;
	  }
	  sudoku = sdList.back();
	  sdList.pop_back();
    }
    if (ret == 0) {
	  printf("%s(): J U B E L - geloest\n", __func__);
      PAUSE;
	  return 0;
    }
    if (ret > 0) {
      printf("%s(): sudoku.solve() returned %d unsolved cells\n", __func__, ret);
    }
    char ln = 0;
    char col = 0;
    char val = 0;
    bool again = false;
    do {
      again = false;
      ret = rdAguessedCell( ln, col, val );
      if (ret != 0) {
        PRINT_ERR("rdAguessedCell(ln=%d col=%d val=%d) returned %d\n", (int)ln, (int)col, (int)val, ret);
        return 7;
      }
      sdList.push_back(sudoku);
      ret = sudoku.setKnownValue(ln, col, val, true);
      if (ret < 0) {
        PRINT_ERR("sudoku.setKnownValue(ln=%d col=%d val=%d) returned %d - roll back\n", (int)ln, (int)col, (int)val, ret);
	    sudoku = sdList.back();
	    sdList.pop_back();
	    again = true;
      } else if (ret == 0) {
        printf("Das Setzen von %d in Zeile %d Spalte %d hat nichts gebracht\n", val, ln, col);
	    sudoku = sdList.back();
	    sdList.pop_back();
	    again = true;
      }
    } while(again);
  }while(true);
  return 0;
}
