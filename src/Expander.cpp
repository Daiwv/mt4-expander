#include "stdafx.h"


/**
 *
 */
int GetInteger(int value) { 
   return(value);

   #pragma EXPORT
}


/**
 *
 */
double GetDouble(double value) {
   return(value);

   #pragma EXPORT
}


/**
 * Gibt den �bergebenen Zeiger auf einen String zur�ck. Kann in MQL zum Lesen eines Strings von einer Adresse 
 * verwendet werden.
 *
 * @param  char* str - Zeiger
 *
 * @return char* - derselbe Zeiger
 *
 * NOTE: MQL-Importdeklaration:  string GetString(int address);
 */
char* GetString(char* str) {
   return(str);

   #pragma EXPORT
}
