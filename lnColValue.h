#ifndef LNCOLVALUE_H_INCLUDED
#define LNCOLVALUE_H_INCLUDED

/*! \brief Eine Zahl mit ihren Koordinaten
  \details Hilfsklasse fuer alles moegliche
  \author Eckhardt Feyhl
  \version 1.0.0.0
  \date 14. Apr. 2024
  \copyright no license
  \file
*/

class lnColValue {
public:
  char line;
  char column;
  char value;

public:
  /*! \brief Default constructor.
    Initialisiert alles mit 0 */
  lnColValue();

  /*! \brief copy constructor.
    setzt die Werte */
  lnColValue( const lnColValue& lcv );

  /*! \brief constructor.
    setzt die Werte */
  lnColValue(char ln, char col, char val);

  /*! \brief Virtual destructor */
  virtual ~lnColValue();
};

#endif /* LNCOLVALUE_H_INCLUDED */
