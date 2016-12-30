#pragma once

/**
 * MyFX struct LOG_MESSAGE
 *
 * Wrapper f�r eine nach MQL zu �bermittelnde Logmessage.
 *
 *
 * TODO: in EXECUTION_CONTEXT integrieren
 */
struct LOG_MESSAGE {
   int   level;                                    // Loglevel: L_ERROR, L_WARN oder L_INFO
   int   code;
   char* text;                                     // szchar
};


// in EXECUTION_CONTEXT:
// ---------------------
// LOG_MESSAGE**      dllErrors;                   //  4    Array von Logmessages des Typs L_ERROR (variabel)   => welche DLL-Fehler sind aufgetreten
// uint               dllErrorsSize;               //  4    Anzahl von Logmessages (Arraygr��e)    (variabel)   => wieviele DLL-Fehler sind aufgetreten
