/*! \brief Die Lese- und Schreib-Funktionen
  \details Von Console oder Datei einlesen und auf Console ausgeben
  \author Eckhardt Feyhl
  \version 1.0.0.0
  \date 22. Apr. 2024
  \copyright no license
  \file
*/

#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include <io.h>
#include "in_out.h"
#include "log.h"

int rdLine( char* pLn, FILE* fp/*=NULL*/ ) {
  int numbCnt = 0; //0..8
  int known = 0; //Anzahl bekannter Zahlen
  char ch = 0;
  if (pLn == NULL) {
	PRINT_ERR("ln==NULL\n");
	return -1;
  }
  for (int idx=0; idx < 9; ++idx) {
    pLn[idx] = 0;
  }
  do {
	if (fp == NULL) {
      ch = getch(); //wartet. getchar() braucht <ENTER>
      if (ch == 'e') {
        return -2;
      }
	} else {
	  ch = fgetc( fp );
      if (ch == EOF) {
   	    PRINT_ERR("incomplete line in file\n");
        return -3;
      }
	}
    if (ch == ' ') {
      ch = '0';
    }
    if (('0' <= ch) && (ch <= '9')) {
      pLn[numbCnt] = ch - '0';
      if (fp == NULL) {
        if (pLn[numbCnt] == 0) {
          printf("   |");
        } else {
          printf(" %c |", ch);
        }
      }
      if ((1 <= pLn[numbCnt]) && (pLn[numbCnt] <= 9)) {
        ++known;
      }
      ++numbCnt;
      if ((numbCnt < 9) && (fp == NULL)) {
        if ((numbCnt % 3) == 0) {
          printf( "|");
        }
      }
    } //else ignore ch
  }while (numbCnt < 9);
  if (fp == NULL) {
    printf("\n");
  }
  return known;
}

int saveSudoku( const char* pVal81 ) {
  char fname[L_tmpnam];
  tmpnam( fname );
  if (fname[0] == '\\') {
    fname[0] = '_';
  }
  FILE* fp = fopen( fname, "w");
  if (fp == NULL) {
  	PRINT_ERR("fopen('%s') failed\n", fname);
  	return -1;
  }
  char ln[12];
  for (int lnIdx=0; lnIdx < 9; ++lnIdx) {
    for (int colIdx=0; colIdx < 9; ++colIdx) {
  	  ln[colIdx] = '0'+pVal81[lnIdx*9+colIdx];
  	}
  	ln[9] = 0; // 0-Terminierung
  	fprintf(fp, "%s\n", ln);
  }
  fclose( fp );
  fp = NULL;
  char dirName[MAX_PATH];
  _getcwd( dirName, sizeof(dirName));
  printf("Filename: %s in %s\n", fname, dirName);
  return 0;
}

int readKnownValues(char* pVal81, int argc, char** argv) {
  int known = 0;
  if (pVal81 == NULL) {
    PRINT_ERR("pVal81==NULL\n");
    return -1;
  }
  int ret = 0;
  FILE* fp = NULL;
  if (argc > 1) {
    if ( argv[1] != NULL ) {
      fp = fopen( argv[1], "r");
    }
  }
  if (fp == NULL) {
    printf("\nBitte die bekannten Zahlen eingeben\n");
    printf("Leertaste oder 0 fuer unbekannt\n");
    printf("e=Ende\n");
  }
  char kv[9][9];
  memset( kv, 0, sizeof(kv));
  for (int lnIdx=0; lnIdx < 9; ++lnIdx) {
    ret = rdLine( &kv[lnIdx][0], fp );
    if (ret < 0) {
      PRINT_ERR("rdLine() returned %d\n", ret);
      if (fp != NULL) {
        fclose( fp );
        fp = NULL;
      }
      return -2;
    }
    if (fp == NULL) {
      if (((lnIdx+1) % 3) == 0) {
        printf("===+===+===||===+===+===||===+===+===\n");
      }
    }
    known += ret;
  }
  if (fp == NULL) {
	printf("Speichern (y)es or (n)o? ");
    int ch = getch(); //wartet. getchar() braucht <ENTER>
    printf("%c\n", (char)ch);
    if (ch == 'y') {
      ret = saveSudoku( &kv[0][0] );
      if (ret < 0) {
        PRINT_ERR("saveSudoku() returned %d\n", ret);
        return -3;
      }
    }
  }
  for (int lnIdx=0; lnIdx<9; ++lnIdx) {
    for (int colIdx=0; colIdx<9; ++colIdx) {
      if (kv[lnIdx][colIdx] != 0) {
        pVal81[lnIdx*9+colIdx] = kv[lnIdx][colIdx];
      }
    }
  }
  if (fp != NULL) {
    fclose( fp );
    fp = NULL;
  }
  return known;
}

int rdAguessedCell(char& ln, char& col, char& val ) {
  printf("\nprobieren durch setzen einer Zahl [(y)es/(n)o]?");
  char ch = getch(); //wartet. getchar() braucht <ENTER>
  printf(" %c\n", ch);
  if (ch != 'y') {
    return 1;
  }
  printf("line [1..9] = ");
  ch = getch();
  if ((ch < '1') || ('9' < ch)) {
    PRINT_ERR("line=%c - expected 1..9\n", ch);
    return -1;
  }
  ln = ch - '0';
  printf("%c column [1..9] = ", ch);
  ch = getch();
  if ((ch < '1') || ('9' < ch)) {
    PRINT_ERR("colulmn=%c - expected 1..9\n", ch);
    return -2;
  }
  col = ch - '0';
  printf("%c value [1..9] = ", ch);
  ch = getch();
  if ((ch < '1') || ('9' < ch)) {
    PRINT_ERR("value=%c - expected 1..9\n", ch);
    return -3;
  }
  val = ch - '0';
  printf("%c\n", ch);
  return 0;
}

int printSudoku(const char* pPossibles729) {
  if (pPossibles729 == NULL) {
    PRINT_ERR("pPossibles729 == NULL\n");
    return -1;
  }
  printf("===+===+===||===+===+===||===+===+===\n");
  int valCnt = 0; //Anzahl aller moeglichen Zahlen in allen Zellen zusammen
  for (int ln=1; ln <= 9; ++ln) {
    for (int sIdx=0; sIdx < 3; ++sIdx) {
      for (int col=1; col <= 9; ++col) {
        for (int pos=1; pos <= 3; ++pos) {
//          char val = getPossibleVal(ln, col, (sIdx*3)+pos);
          char val = pPossibles729[(ln-1)*81 + (col-1)*9 + (sIdx*3)+(pos-1)];
          if ((val < 1) || (9 < val)){
            printf(" ");
          } else {
            printf("%c", '0'+val);
            ++valCnt;
          }
        }
        if (col < 9) {
          printf("|");
          if ((col % 3) == 0) {
            printf("|");
          }
        }
      }
      printf("\n");
    }
    if ( (ln % 3) == 0) {
      printf("===+===+===||===+===+===||===+===+===\n");
    } else {
      printf("---+---+---||---+---+---||---+---+---\n");
    }
  }
  PAUSE;
  return valCnt;
}

bool rdRollBack( bool rollBackPossible ) {
  if ( !rollBackPossible ) {
    return false;
  }
  printf("\nLetzten Zustand wiederherstellen [(y)es/(n)o]?");
  char ch = getch(); //wartet. getchar() braucht <ENTER>
  printf(" %c\n", ch);
  if (ch == 'y') {
    return true;
  }
  return false;
}
