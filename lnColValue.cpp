/*! \brief Eine Zahl mit ihren Koordinaten
  \details Hilfsklasse fuer alles moegliche
  \author Eckhardt Feyhl
  \version 1.0.0.0
  \date 14. Apr. 2024
  \copyright no license
  \file
*/

#include "lnColValue.h"

lnColValue::lnColValue() {
  line = 0;
  column = 0;
  value = 0;
}

lnColValue::lnColValue( const lnColValue& lcv ) {
  line = lcv.line;
  column = lcv.column;
  value = lcv.value;
}

lnColValue::lnColValue(char ln, char col, char val) {
  line = ln;
  column = col;
  value = val;
}

//virtual
lnColValue::~lnColValue() {
}
