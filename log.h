#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

/*! \brief Fuer die Fehler-, Warnungs-, Info- und Debug-Ausgaben
  \details Makros um die Ausgaben zu vereinfachen
  \author Eckhardt Feyhl
  \version 1.0.0.0
  \date 21. Apr. 2024
  \copyright no license
  \file
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef USE_PAUSE
#  define PAUSE system("pause")
#else
#  define PAUSE
#endif

#define PRINT_ERR(fmt,...) {printf("ERROR %s(): " fmt, __func__, ##__VA_ARGS__); \
  PAUSE;}

#endif /* LOG_H_INCLUDED */
