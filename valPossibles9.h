#ifndef VALPOSSIBLES9_H_
#define VALPOSSIBLES9_H_

#include "possibles9.h"

/*! \brief Fuer jede Zahl 1..9 die Liste der moeglichen Zellen
 * valPossibles9.h
 *
 *  Created on: 09.04.2024
 *      Author: efeyh
 */

class valPossibles9 {
private:
  possibles9 valPoss[9];

public:
  /*! \brief initialisiert alles mit 0 */
  valPossibles9();

  /*! \brief initialisiert alles mit 0 */
  virtual ~valPossibles9();

  /*! \brief liefert die Anzahl Zellen in der die Zahl val vorkommt
    \returns <=0 wenn die Zahl nicht mehr in den 9 Zellen vorkommt \n
      >0 die Anzahl Vorkommnisse der Zahl in den 9 Zellen */
  int getPossibleCnt(
    char val /*!< Zahl 1..9 */
	) const;

  /*! \brief fuegt die Zeile und Spalte hinzu und inkrementiert possibleCnt
    \returns <0 bei Fehler \n
      0 wenn die Zelle schon eingetragen ist \n
      >0 neues possibleCnt wenn die Zelle neu eingetragen wurde */
  int addPossible(
    char ln, /*!< Zeile 1..9 */
    char col, /*!< Spalte 1..9 */
    char val /*!< Zahl 1..9 */
	);

  /*! \brief fuegt die Zeile und Spalte hinzu und inkrementiert possibleCnt
    \returns <0 bei Fehler \n
      0 wenn die Zelle schon eingetragen ist \n
      >0 neues possibleCnt wenn die Zelle neu eingetragen wurde */
  int addPossible(
    char* pLn, /*!< Zeile 1..9 */
    char* pCol, /*!< Spalte 1..9 */
    char val, /*!< Zahl 1..9 */
	int cnt /*!< Anzahl Vorkommnisse 1..9 */
	);

  /*! \brief liefert in ln und col die Zelle vom Vorkommnis cnt (1..9) zurueck
    \returns <0 bei Fehler
      0 wenn fuer das Vorkommnis cnt keine gueltige Zelle eingetragen ist
      1 wenn ln und col fuer das Vorkommnis cnt gueltig sind */
  int getPossibleCell(
    char& ln, /*!< [out] Zeile 1..9 */
    char& col, /*!< [out] Spalte 1..9 */
    char val, /*!< Zahl 1..9 */
	char cnt /*!< Vorkommnis 1..9 */
	) const;

  /*! \brief prueft alle Zahlen auf Plausibilitaet
    \returns <0 bei Fehler \n
      0 wenn alles ok ist \n
      1..9 Die Zahl bei der etwas nicht stimmt */
  int check() const;
};

#endif /* VALPOSSIBLES9_H_ */
