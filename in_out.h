#ifndef IN_OUT_H_INCLUDED
#define IN_OUT_H_INCLUDED

/*! \brief Lese- und Schreib-Funktionen fuer ein sudoku
  \details Von Console oder Datei einlesen und auf Console ausgeben
  \author Eckhardt Feyhl
  \version 1.0.0.0
  \date 21. Apr. 2024
  \copyright no license
  \file
*/

#include <stdio.h>

/*! \brief Aufforderung eine Zelle zu erraten
  \returns 0 = user hat eine Zahl geraten \n
    in ln, col und val die geratene Zahl fuer die Zelle ln/col */
int rdAguessedCell(char& ln, char& col, char& val );

/*! \brief Liest die gesetzten Zahlen einer Zeile in pLn ein
  \returns die Anzahl bekannter Zahlen und
    in pLn die eingelesenen Zahlen der Zeile */
int rdLine(
  char* pLn, /*!< [out] die eingelesenen Zahlen (0=unbekannt) */
  FILE* fp=NULL /*!< optional wenn von Datei eingelesen werden soll */
  );

/*! \brief Liest ein Sudoku von Datei oder Console in pVal81 ein
  \details wenn argc>=2 und argv[1] eine vorhandene Datei ist,
    wird das Sudoku von dieser Datei eingelesen.
    Ist argc<2 oder argv[1] keine vorhandene Datei,
    dann wird von Console eingelesen */
int readKnownValues(
  char* pVal81, //!< [out] pointer auf die 81 Zellen
  int argc, //!< von main()
  char** argv //!< NULL oder von main() der Dateiname
  );

/*! \brief Speichert die Werte von pVal81 in eine Datei */
int saveSudoku( const char* pVal81 );

/*! \brief Gibt das Sudoku auf dem Bildschirm aus
  \details pro Zelle werden alle noch moeglichen Zahlen ausgegeben */
int printSudoku(
  const char* pPossibles729 //!< die 9x9x9 Zahlen
  );

bool rdRollBack(bool rollBackPossible);

#endif /* IN_OUT_H_INCLUDED */
