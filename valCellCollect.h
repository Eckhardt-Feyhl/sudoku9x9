#ifndef VALCELLCOLLECT_H_
#define VALCELLCOLLECT_H_

/*
 * valCellCollect.h
 *
 *  Created on: 09.04.2024
 *      Author: efeyh
 */

class valCellCollect {
private:
  char values[9]; /*!< Die schon eingesammelten Zahlen */
  char valCnt; /*!< Die Anzahl Zahlen */
  char lines[9]; /*!< Die Zeilen der belegten Zelle. lines[n]/colums[n] ist die belegte Zelle. */
  char columns[9]; /*!< Die Spalten der belegten Zelle. lines[n]/colums[n] ist die belegte Zelle. */
  char cellCnt; /*!< Die Anzahl belegter Zellen */

public:
  /*! \brief initialisiert alles mit 0 */
  valCellCollect();

  /*! \brief initialisiert alles mit 0 */
  virtual ~valCellCollect();

  /*! \brief initialisiert alles mit 0 */
  void valCellCollectInit0();

  /*! \brief liefert true wenn die Anzahl Zahlen genau die gleiche Anzahl Zellen belegen */
  bool isValCellCntEqual() const;

  /*! \brief fuegt die Zeile, Spalte und Zahl hinzu
    \returns <0 bei Fehler \n
      0 wenn die Anzahl Zellen gleich der Anzahl Zahlen ist \n
      >0 die Anzahl belegter Zellen */
  int addValCell(
    char ln, /*!< Zeile 1..9 */
    char col, /*!< Spalte 1..9 */
	char val /*!< Zahl 1..9 */
	);

  char getCellCnt() const;

  char getValCnt() const;
  const char* getPossibleLnsPtr() const;
  const char* getPossibleColsPtr() const;
  const char* getPossibleValsPtr() const;
};

#endif /* POSSIBLES9_H_ */
