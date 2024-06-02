#ifndef POSSIBLES9_H_
#define POSSIBLES9_H_

/*
 * possibles9.h
 *
 *  Created on: 09.04.2024
 *      Author: efeyh
 */

class possibles9 {
private:
  char possibleCnt; /*!< Anzahl Vorkommnisse der Zahl in den 9 moeglichen Zellen */
  char possibleLn[9]; /*!< Fuer jeden possibleCnt die Zeilen 1..9 in der die Zahl vorkommt.
    0=Die Zahl ist fuer diesen possibleCnt nicht in der Zeile enthalten */
  char possibleCol[9]; /*!< Fuer jeden possibleCnt die Spalte 1..9 in der die Zahl vorkommt.
    0=Die Zahl ist fuer diesen possibleCnt nicht in der Spalte enthalten */

  /*! \brief initialisiert alles mit 0 */
  void possibleInit0();

public:
  /*! \brief initialisiert alles mit 0 */
  possibles9();

  /*! \brief initialisiert alles mit 0 */
  virtual ~possibles9();

  /*! \brief liefert die Anzahl Zellen in der die Zahl vorkommt
    \returns <=0 wenn die Zahl nicht mehr in den 9 Zellen vorkommt \n
      >0 die Anzahl Vorkommnisse der Zahl in den 9 Zellen */
  int getPossibleCnt() const;

  /*! \brief fuegt die Zeile und Spalte hinzu und inkrementiert possibleCnt
    \returns <0 bei Fehler \n
      0 wenn die Zelle schon eingetragen ist \n
      >0 neues possibleCnt wenn die Zelle neu eingetragen wurde */
  int addPossible(
    char ln, /*!< Zeile 1..9 */
    char col /*!< Spalte 1..9 */
	);

  /*! \brief liefert in ln und col die Zelle vom Vorkommnis cnt (1..9) zurueck
    \returns <0 bei Fehler
      0 wenn fuer das Vorkommnis cnt keine gueltige Zelle eingetragen ist
      1 wenn ln und col fuer das Vorkommnis cnt gueltig sind */
  int getPossibleCell(
    char& ln, /*!< [out] Zeile 1..9 */
    char& col, /*!< [out] Spalte 1..9 */
	char cnt /*!< [in] Vorkommnis 1..9 */
	) const;

  /*! \brief prueft alle Eintraege auf Plausibilitaet
    \returns <0 bei Fehler \n
      0 wenn alles ok ist */
  int check() const;
};

#endif /* POSSIBLES9_H_ */
