#ifndef CELLVALUES9_H_INCLUDED
#define CELLVALUES9_H_INCLUDED

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

#include <stddef.h> //NULL

class cellValues9 {
private:
  char cellValue[9]; /*!< WICHTIG! index+1=value. Inhalt=0 die Zahl ist nicht mehr moeglich */

public:
  /*! \brief Default constructor.
    Initialisiert die Zelle so dass alle Zahlen in diser Zelle moeglich sind */
  cellValues9();

  /*! \brief Constructor mit der Initialisierungs-Zahl.
    Initialisiert die Zelle mit dieser Zahl */
  cellValues9(
    char val //!< {-128..+127] die zu initialisierende Zahl
	);

  /*! \brief Virtual destructor */
  virtual ~cellValues9();

  /*! \brief Schreibt in cellValue alle Zahlen 1..9 */
  void initAllPossible();

  /*! \brief fuer allgemeine Listen setzen wir alle Zahlen auf 0 */
  void init(
    char val //!< {-128..+127] die zu initialisierende Zahl
    );

  /*! \brief liefert die Anzahl Zahlen die in dieser Zelle noch moeglich sind
    \returns <0 bei Fehler \n
      0 wenn keine Zahl mehr in dieser Zelle vorkommt \n
      1..9 die Anzahl noch moeglicher Zahlen */
  int getPossibleCnt(
    char* pVal=NULL //!< optional die Liste der noch moeglichen Zahlen
	) const;

  /*! \brief liefert 0, wenn die Zahl val
    in der Zelle nicht moeglich ist.
    Wenn die Zahl val moeglich ist,
    wird val zurueckgegeben */
  char getPossibleValue(
    char val // 1..9 die Zahl
	) const;

  /*! \brief loescht die Zahl val aus dieser Zelle
    \returns <0 bei Fehler
      0 wenn sie schon geloescht war (zum Mitzaehlen wieviele Zahlen geloescht wurden)
      1 wenn sie geloescht wurde (zum Mitzaehlen wieviele Zahlen geloescht wurden) */
  int clearPossibleValue(
    char val //!< 1..9 die zu loeschende Zahl
	);

  /*! \brief loescht alle Zahlen bis auf val aus dieser Zelle
    \returns <0 bei Fehler
      >=0 die Anzahl geloeschter Zahlen (zum Mitzaehlen wieviele Zahlen geloescht wurden) */
  int clearPossibleValueBut(
    char val, //!< 1..9 die Zahl die nicht geloescht werden soll
	char* clearedVal=NULL //!< optional die Liste der geloeschten Zahlen
	);

  /*! \brief loescht alle Zahlen bis auf die in pVal aufgelisteten aus dieser Zelle
    \returns <0 bei Fehler
      >=0 die Anzahl geloeschter Zahlen (zum Mitzaehlen wieviele Zahlen geloescht wurden) */
  int clearPossiblesBut(
    const char* pVal, //!< die Liste der nicht zu loeschenden Zahlen
	char* pCleared=NULL //!< optional die Liste der geloeschten Zahlen
	);
};

#endif /* CELLVALUES9_H_INCLUDED */
