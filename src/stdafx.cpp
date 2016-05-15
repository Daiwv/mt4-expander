#include "stdafx.h"


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende feste Werte zur�ckzugeben.
 * Alle Parameter werden ignoriert.
 */
BOOL _TRUE (...) { return(TRUE ); }
BOOL _FALSE(...) { return(FALSE); }


/**
 * Pseudo-Funktionen, die ihrem Namen entsprechende variable Werte zur�ckzugeben.
 * Au�er dem ersten werden alle �bergebenen Parameter ignoriert.
 */
BOOL _BOOL(BOOL value, ...) { return(value); }
