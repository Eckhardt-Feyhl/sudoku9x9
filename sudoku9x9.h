#ifndef SUDOKU9X9_H_INCLUDED
#define SUDOKU9X9_H_INCLUDED

/*! \brief Alle Zellen eines Sudoku 9 mal 9
  \author Eckhardt Feyhl
  \version 1.0.0.0
  \date 14. Apr. 2024
  \copyright no license
  \file
*/

#include <list>
#include "cellValues9.h"
#include "lnColValue.h"

class valCellCollect;
class valPossibles9;

/* \brief liefert die Anzahl Vorkommnisse von val in arr
  \details mit val=0 werden die schon entfernten Zahlen der Zelle gezaehlt
  \returns <0 bei Fehler \n
    0..9 die Anzahl Vorkommnisse von val in arr */
extern int inArray9( const char* arr, char val, char* pOut9=NULL);

/*! \brief Ueberprueft ob alle gueltigen lnCols in der selben Zeile bzw. Spalte liegen.
  \returns true - es liegt alles in der selben Zeile bzw. Spalte \n
    false - mindestens einer liegt ausserhalb
  \details Es werden alle 9 cols ueberprueft.
    Ungueltige werden ignoriert.
    lnCols erhaelt man von valCntQuad() */
bool isSameLnCol( char* lnCols );

/*! \brief Ueberprueft ob alle gueltigen lnCol in einem Quadrant liegen.
  \returns true - es liegt alles im selben Quadrant \n
    false - mindestens einer liegt ausserhalb
  \details Es werden alle 9 lnCol ueberprueft.
    Ungueltige werden ignoriert.
    lnCol erhaelt man von valCntLn() bzw. valCntCol() */
bool isSameQuad( char* lnCol );

class sudoku9x9 {
private:
  cellValues9 errorCell;
  cellValues9 allCells[9][9]; //!< die 9 Zeilen mit ihren 9 Spalten
  std::list<lnColValue> onlyOne; /*!< hier werden die Zellen hinterlegt,
    die durch Loeschen einer Zahl nur noch eine Zahl enthalten */

public:
  /*! \brief default constructor
        Initialisiert alle Zellen mit allen moeglichen Zahlen */
  sudoku9x9();

  /*! \brief virtual destructor */
  virtual ~sudoku9x9();

  /*! \brief Gibt das Sudoku auf dem Bildschirm aus
    \returns eine negative Zahl bei Fehler */
  int print();

  // C E L L ----------------------------------------

  /*! \brief liefert die Zelle der Spalte col zurueck */
  cellValues9& getCell(
    int ln, //!< 1..9 die Zeile im Sudoku
    int col //!< 1..9 die Spalte in der Zeile
	);

  /*! \brief liefert 0, wenn die Zahl val
    in der Zelle ln/col nicht moeglich ist.
    Wenn die Zahl val moeglich ist, wird val zurueckgegeben
    \returns <0 bei Fehler */
  char getPossibleVal(int ln, int col, char val);

  /*! \brief Liefert die Anzahl moeglicher Zahlen der Zelle.
    \details Optional werden die moeglichen Zahlen in pVals eingetragen
    \returns die Anzahl moeglicher Zahlen der Zelle */
  int getPossibleCnt(int ln, int col, char* pVals=NULL);

  /*! \brief loescht die moegliche Zahl val aus der Zelle ln/col
    \returns <0 bei Fehler
      0 wenn sie schon geloescht war (zum Mitzaehlen wieviele Zahlen geloescht wurden)
      1 wenn sie geloescht wurde (zum Mitzaehlen wieviele Zahlen geloescht wurden) */
  int clearPossibleVal(
    int ln, //!< 1..9 die Zeile im Sudoku
    int col, //!< 1..9 die Spalte in der Zeile
    int val, //!< 1..9 die zu loeschende Zahl
	bool show=true //!< false=keine Ausgabe auf dem Bildschirm
    );

  /*! \brief loescht alle moegliche Zahlen in der Zelle mit Zeile ln und Spalte col bis auf die in pVal angegebenen Zahlen */
  int clearPossibleValsBut(int ln, int col, const char* pVal);

  /*! \brief loescht alle andere Zahlen ausser die in vals aus den Zellen die in pLn/pCol angegeben sind
    \details z.B. 2 Zahlen belegen die gleichen beiden Zellen.
      Ergo koennen alle anderen Zahlen aus diesen beiden Zellen entfernt werden */
  int clearOtherValues(const char* ln, const char* col, const char* vals, int maxNums);

  /*! \brief loescht alle andere Zahlen ausser die in vcc aus den Zellen die in vcc angegeben sind
    \details z.B. 2 Zahlen belegen die gleichen beiden Zellen.
      Ergo koennen alle anderen Zahlen aus diesen beiden Zellen entfernt werden */
  int clearOtherValues(const valCellCollect& vcc);

  /*! \brief fuegt alle Zellen von val in valCellCollect vcc ein */
  int insertValCells(valCellCollect& vcc, const valPossibles9& vp, char val);

  /*! \brief fuegt die Zeilen und Spalten der naechsten passenden Zahl in collect ein.
    returns <0 bei Fehler \n
      0 wenn keine weitere Zahl mehr reinpasst \n
      >0 die eingefuegte Zahl */
  int insertNextValIntoLnCol(
    valCellCollect& collect, //!< [inout] Liste der belegten Zellen und die dazugehoerigen Zahlen
    const valPossibles9& valPos, //!< fuer jede Zahl ihr vorkommen in den 9 Zellen
    char startVal //!< startet die Suche mit dieser Zahl
    );

  /*! \brief Wenn n Zellen mit n Zahlen belegt sind,
    koennen alle anderen Zahlen aus diesen n Zellen entfernt werden */
  int clearCellN_v3(const valPossibles9& vp9);

  // L I N E ----------------------------------------

  /*! \brief Loescht die moegliche Zahl val aus der Zeile bis auf die Spalte col.
    \returns Die Anzahl geloeschter Zahlen. */
  int clearLineNum(int ln, int col, char val, bool show);

  /*! \brief Loescht die moegliche Zahl val aus der Zeile bis auf die Stellen in pCol.
    \returns Die Anzahl geloeschter Zahlen. */
  int clearLineButCols(int ln, const char* pCol, char val);

  /*! \brief Liefert die Anzahl Zellen in der die Zahl val in der Zeile ln moeglich ist.
    \returns in pCol (pointer auf 9 x char) den/die Spaltennummern (1..9)
    \details pCol wird, wenn != NULL, mit 0 vorbesetzt. \n
      Mit val=0 werden die noch verdeckten Zellen gezaehlt
      (Zellen mit mehr als einer moeglichen Zahl) */
  int valCntLn( char* pCol, int ln, char val);

  /*! \brief Loest die Zeile
    \details Wenn eine Zahl nur noch einmal in der Zeile vorkommt, wird sie gesetzt \n
      Wenn eine Zahl nur noch 2 oder 3 mal innerhalb eines Quadranten vorkommt,
      wird sie aus den anderen Zeilen des Quadrantes geloescht. \n
      Wenn 2 oder mehr Zahlen die gleichen Zellen belegen,
      werden alle anderen Zahlen aus diesen Zellen geloescht.
    \returns die Anzahl geloeschter moeglicher Zahlen */
  int solveLine( int ln );

  /*! \brief geht alle Zeilen durch um das sudoku zu loesen */
  int solveLines();

  // C O L U M N ----------------------------------------

  /*! \brief Loescht die moegliche Zahl val aus der Spalte bis auf die Stelle ln.
    \returns Die Anzahl geloeschter Zahlen. */
  int clearColNum(
    int ln,
	int col,
	char val,
	bool show=true
	);

  /*! \brief Loescht die moegliche Zahl val aus der Spalte bis auf die Stellen in pLn.
    \returns Die Anzahl geloeschter Zahlen. */
  int clearColButLns(int col, char* pLn, char val);

  /*! \brief Liefert die Anzahl Zellen in der die Zahl val in der Spalte col moeglich ist.
    \returns in pLn (pointer auf 9 x char) den/die Zeilennummern (1..9)
    \details pLn wird, wenn != NULL, mit 0 vorbesetzt. \n
      Mit val=0 werden die noch verdeckten Zellen gezaehlt
      (Zellen mit mehr als einer moeglichen Zahl) */
  int valCntCol( char* pLn, int col, char val);

  /*! \brief Loest die Spalte
    \details Wenn eine Zahl nur noch einmal in der Spalte vorkommt, wird sie gesetzt \n
      Wenn eine Zahl nur noch 2 oder 3 mal innerhalb eines Quadranten vorkommt,
      wird sie aus den anderen Spalten des Quadrantes geloescht. \n
      Wenn n Zahlen in n gleichen Zellen vorkommen, werden andere Zahlen
      aus diesen n Zellen geloescht.
    \returns die Anzahl geloeschter moeglicher Zahlen */
  int solveCol( int col );

  /*! \brief geht alle Spalten durch um das sudoku zu loesen */
  int solveCols();

  // Q U A D R A N T ----------------------------------------

  /*! \brief Liefert den Index 0..8 des Quadranten,
    in dem lnIdx, colIdx liegt.
    \details die Quadranten werden von oben links nach rechts
      und dann nach unten durchindiziert (0..8)
    \returns [0..8] den Index des Quadranten */
  int getQuadIdx( int lnIdx, int colIdx );

  /*! \brief Liefert den Index 0..8 der Zelle im Quadranten,
    in dem lnIdx, colIdx liegt.
    \details die Zellen im Quadranten werden von oben links nach rechts
      und dann nach unten durchindiziert (0..8)
    \returns [0..8] den Index der Zelle im Quadranten */
  int getQuadCellArrIdx( int lnIdx, int colIdx );

  /*! \brief Loescht die moegliche Zahl val aus dem Quadrant bis auf die Zeile ln.
    \returns Die Anzahl geloeschter Zahlen. */
  int clearQuadNumLn(int ln, int col, char val);

  /*! \brief Loescht die moegliche Zahl val aus dem Quadrant bis auf die Spalte col.
    \returns Die Anzahl geloeschter Zahlen. */
  int clearQuadNumCol(int ln, int col, char val);

  /*! \brief Loescht die moegliche Zahl val aus dem Quadrant bis auf die Zelle ln/col.
    \returns Die Anzahl geloeschter Zahlen. */
  int clearQuadNum(
    int ln,
	int col,
	char val,
	bool show=true
	);

  /*! \brief Liefert die Anzahl Zellen in der die Zahl val im Quadrant moeglich ist.
    \returns in pLn und pCol (je pointer auf 9 x char) die Koordinaten
    \details pLn und pCol werden, wenn != NULL, mit 0 vorbesetzt. \n
      Mit val=0 werden die noch verdeckten Zellen gezaehlt
      (Zellen mit mehr als einer moeglichen Zahl) */
  int valCntQuad( char* pLn, char* pCol, int ln, int col, char val);

  /*! \brief Loest den Quadrant
    \details Wenn eine Zahl nur noch einmal im Quadrant vorkommt, wird sie gesetzt \n
      Wenn eine Zahl nur noch 2 oder 3 mal innerhalb einer Zeile oder Spalte des Quadranten vorkommt,
      wird sie aus den Zellen der Zeile bzw. Spalte ausserhalt des Quadrantes geloescht. \n
      Wenn 2 oder mehr Zahlen die gleichen Zellen belegen,
      werden alle anderen Zahlen aus diesen Zellen geloescht.
    \returns die Anzahl geloeschter moeglicher Zahlen */
  int solveQuad( int ln, int col);

  /*! \brief geht alle Quadranten durch und
     versucht eine oder mehrere moegliche Zahlen aus den Zellen zu loeschen
     \returns die Anzahl geloeschter moegliche Zahlen */
  int solveQuads();

  // S U D O K U

  /*! \brief setzt die bekannte Zahl in das Sudoku ein
    \details fuer die bekannte Zahl werden: \n
      - in der Zelle ln/col alle anderen Zahlen bis auf val geloescht \n
      - in allen anderen Zellen der Zeile die Zahl val geloescht \n
      - in allen anderen Zellen der Spalte die Zahl val geloescht \n
      - in allen anderen Zellen des Quadrants die Zahl val geloescht
    \returns die Anzahl geloeschter Zahlen */
  int setKnownValue(
    int ln, //!< 1..9 die Zeile
    int col, //!< 1..9 die Spalte
    char val, //!< 1..9 die Zahl
	bool show=true //!< false = keine Ausgabe des sudokus
    );

  /*! \brief setzt die bekannten Zahlen in das Sudoku ein
    \details fuer jede bekannte Zahl werden: \n
      - in allen anderen Zellen der Zeile die Zahl geloescht \n
      - in allen anderen Zellen der Spalte die Zahl geloescht \n
      - in allen anderen Zellen des Quadrants die Zahl geloescht */
  int setKnownValues(const char* pVal81);

  /*! \brief Loest das Sudoku
    \returns 0 wenn es geloest wurde
             ansonsten die Anzahl ungeloester Zellen */
  int solve();
};

#endif /* SUDOKU9X9_H_INCLUDED */
