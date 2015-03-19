/**
 * Der Expander des Fortschritts
 *
 *
 * @author  Peter Walther
 */
#include "Expander.h"


/**
 * DLL entry point
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
   switch (reason) {
      case DLL_PROCESS_ATTACH: onProcessAttach(); break;
      case DLL_THREAD_ATTACH : onThreadAttach();  break;
      case DLL_THREAD_DETACH : onThreadDetach();  break;
      case DLL_PROCESS_DETACH: onProcessDetach(); break;
   }
   return(TRUE);
}


// Liste des letzten EXECUTION_CONTEXT je Thread
CRITICAL_SECTION    threadsSection;             // Lock
DWORD*              threads;                    // Array
size_t              threadsSize;                // Arraygr��e
EXECUTION_CONTEXT** lastContext;                // Array


/**
 *
 */
BOOL onProcessAttach() {
   //debug("");
   InitializeCriticalSection(&threadsSection);
   return(TRUE);
}


/**
 *
 */
BOOL onThreadAttach() {
   //debug("threadId=%d", GetCurrentThreadId());
   return(TRUE);
}


/**
 *
 */
BOOL onThreadDetach() {
   //debug("threadId=%d", GetCurrentThreadId());
   ResetCurrentThreadData();
   return(TRUE);
}


/**
 *
 */
BOOL onProcessDetach() {
   //debug("threadId=%d", GetCurrentThreadId());
   onThreadDetach();
   DeleteCriticalSection(&threadsSection);
   return(TRUE);
}


/**
 *
 */
BOOL WINAPI Expander_onInit(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));
   //debug("  thread=%d, type=%s, name=%s", GetCurrentThreadId(), ModuleTypeDescription((ModuleType)ec->programType), ec->programName);
   return(TrackContext(ec));
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Expander_onStart(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));
   return(TrackContext(ec));
   #pragma EXPORT
}


/**
 *
 */
BOOL WINAPI Expander_onDeinit(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));
   return(TrackContext(ec));
   #pragma EXPORT
}


/**
 * Aktualisiert den letzten bekannten EXECUTION_CONTEXT eines Threads. Wird indirekt von jeder MQL-Rootfunktion aufgerufen.
 * MQL-Libraries k�nnen dadurch den aktuellen EXECUTION_CONTEXT und dar�ber das sie aufrufende MQL-Programm ermitteln.
 *
 * @param EXECUTION_CONTEXT* ec - aktueller Context eines MQL-Programms
 *
 * @return bool - Erfolgsstaus
 */
BOOL TrackContext(EXECUTION_CONTEXT* ec) {
   if (!ec) return(debug("invalid parameter ec = (null)"));

   DWORD currentThread = GetCurrentThreadId();


   // (1) ThreadId im EXECUTION_CONTEXT setzen bzw. aktualisieren
   if (ec->hThreadId != currentThread)
      ec->hThreadId = currentThread;


   // (2) Thread in den bekannten Threads suchen
   size_t i;
   for (i=0; i < threadsSize; i++) {
      if (currentThread == threads[i])
         break;
   }
   if (i < threadsSize) {                                // Thread gefunden: letzten Context aktualisieren
      if (lastContext[i] != ec) {
         lastContext[i] = ec;
         //debug("switching context of thread at index %d", i);
      }
   }
   else {                                                // Thread nicht gefunden
      // synchronize() start
      EnterCriticalSection(&threadsSection);

      for (i=0; i < threadsSize; i++) {                  // ersten freien Slot suchen
         if (!threads[i]) break;
      }
      if (i == threadsSize) {                            // kein freier Slot mehr vorhanden, Arrays vergr��ern (Startwert: 8)
         size_t new_size = 2 * (threadsSize ? threadsSize : 4);
         debug("increasing arrays to %d", new_size);

         DWORD* tmp1 = (DWORD*) realloc(threads,     new_size * sizeof(DWORD)             ); if (!tmp1) return(debug("->realloc(threads) failed"));
         void** tmp2 = (void**) realloc(lastContext, new_size * sizeof(EXECUTION_CONTEXT*)); if (!tmp2) return(debug("->realloc(lastContext) failed"));

         for (size_t n=threadsSize; n < new_size; n++) { // neuen Speicherbereich initialisieren
            tmp1[n] = NULL;
            tmp2[n] = NULL;
         }
         threads     =                       tmp1;       // und Zuweisung �ndern
         lastContext = (EXECUTION_CONTEXT**) tmp2;
         threadsSize = new_size;

         //debug("adding thread at index %d", i);
      }
      //else debug("inserting thread at index %d", i);

      threads    [i] = currentThread;                    // Thread und letzten Context an Index i einf�gen
      lastContext[i] = ec;

		LeaveCriticalSection(&threadsSection);
		// synchronize() end
   }

   return(TRUE);
}


/**
* Setzt die EXECUTION_CONTEXT-Daten des aktuellen Threads zur�ck.
 *
 * @return bool - Erfolgsstaus
 */
BOOL ResetCurrentThreadData() {
   DWORD currentThread = GetCurrentThreadId();

   // (1) aktuellen Thread in den Threads mit EXECUTION_CONTEXT suchen
   size_t i;
   for (i=0; i < threadsSize; i++) {
      if (currentThread == threads[i])
         break;
   }

   // (2) bei Sucherfolg Daten l�schen, der Slot ist danach wieder verf�gbar
   if (i < threadsSize) {
      EnterCriticalSection(&threadsSection);             // synchronize() start
      //debug("dropping thread at index %d", i);
      threads    [i] = NULL;
      lastContext[i] = NULL;
		LeaveCriticalSection(&threadsSection);             // synchronize() end
   }
   return(TRUE);
}


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zur�ck.
 *
 * @param  BOOL lpValues[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetBoolsAddress(bool array[]);
 */
int WINAPI GetBoolsAddress(BOOL lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zur�ck.
 *
 * @param  int lpValues[] - MQL-Integer-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetIntsAddress(int array[]);
 */
int WINAPI GetIntsAddress(int lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Alias
 */
int WINAPI GetBufferAddress(int values[]) {
   return(GetIntsAddress(values));
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zur�ck.
 *
 * @param  double lpValues[] - MQL-Double-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetDoublesAddress(double array[]);
 */
int WINAPI GetDoublesAddress(double lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zur�ck.
 *
 * @param  MqlStr lpValues[] - MQL-String-Array
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringsAddress(string values[]);
 */
int WINAPI GetStringsAddress(MqlStr lpValues[]) {
   if (lpValues && (int)lpValues < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValues = 0x%p (not a valid pointer)", lpValues));
   return((int) lpValues);
   #pragma EXPORT
}


/**
 * Gibt die Speicheradresse eines Strings zur�ck.
 *
 * @param  char* lpValue - String (MetaTrader �bergibt f�r einen MQL-String das Element MqlStr.string)
 *
 * @return int - Speicheradresse oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  int GetStringAddress(string value);
 */
int WINAPI GetStringAddress(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER)
      return(debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue));
   return((int) lpValue);
   #pragma EXPORT
}


/**
 * Gibt den �bergebenen Zeiger auf einen String selbst zur�ck. Kann in MQL zum Lesen eines Strings von einer Adresse
 * verwendet werden, da MetaTrader den Zeiger mit der entsprechenden Importdeklaration in einen MQL-String konvertiert.
 *
 * @param  char* lpValue - String
 *
 * @return char* - derselbe Zeiger oder NULL, falls ein Fehler auftrat
 *
 *
 * @mql-import:  string GetString(int address);
 */
char* WINAPI GetString(const char* lpValue) {
   if (lpValue && (int)lpValue < MIN_VALID_POINTER)
      return((char*) debug("invalid parameter lpValue = 0x%p (not a valid pointer)", lpValue));
   return((char*) lpValue);
   #pragma EXPORT
}


/**
 * Gibt den letzten aufgetretenen Windows-Fehler des aktuellen Threads zur�ck. Wrapper f�r kernel32::GetLastError(),
 * da MQL eine Funktion desselben Namens definiert.
 *
 * @return int - Fehlercode
 *
 *
 * @mql-import:  int GetLastWin32Error();
 */
int WINAPI GetLastWin32Error() {
   return(GetLastError());
   #pragma EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen eingebauten MetaTrader-Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 *
 *
 * @mql-import:  bool IsBuiltinTimeframe(int timeframe);
 */
BOOL WINAPI IsBuiltinTimeframe(int timeframe) {
   switch (timeframe) {
      case PERIOD_M1 : return(TRUE);
      case PERIOD_M5 : return(TRUE);
      case PERIOD_M15: return(TRUE);
      case PERIOD_M30: return(TRUE);
      case PERIOD_H1 : return(TRUE);
      case PERIOD_H4 : return(TRUE);
      case PERIOD_D1 : return(TRUE);
      case PERIOD_W1 : return(TRUE);
      case PERIOD_MN1: return(TRUE);
   }
   return(FALSE);
   #pragma EXPORT
}


/**
 * Ob die angegebene Timeframe-ID einen benutzerdefinierten Timeframe darstellt.
 *
 * @param  int timeframe - Timeframe-ID
 *
 * @return BOOL
 *
 *
 * @mql-import:  bool IsCustomTimeframe(int timeframe);
 */
 BOOL WINAPI IsCustomTimeframe(int timeframe) {
   if (timeframe <= 0)
      return(FALSE);
   return(!IsBuiltinTimeframe(timeframe));
   #pragma EXPORT
}


/**
 * Gibt die hexadezimale Repr�sentation eines Integers zur�ck.
 * z.B.: IntToHexStr(13465610) => "00CD780A"
 *
 * @param  int value - Integer (4 Byte)
 *
 * @return char* - hexadezimaler String mit 8 Zeichen
 *
 *
 * @mql-import:  string IntToHexStr(int value);
 */
char* WINAPI IntToHexStr(int value) {
   int   size = 9;
   char* str  = new char[size];
   sprintf_s(str, size, "%p", value);

   return(str);
   #pragma EXPORT
}


/**
 * Gibt die lesbare Beschreibung eines ModuleTypes zur�ck.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ung�ltig ist
 */
const char* ModuleTypeDescription(ModuleType type) {
   switch (type) {
      case MT_EXPERT   : return("Expert"   );
      case MT_SCRIPT   : return("Script"   );
      case MT_INDICATOR: return("Indicator");
      case MT_LIBRARY  : return("Library"  );
   }
   debug("unknown module type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante eines ModuleTypes zur�ck.
 *
 * @param  ModuleType type
 *
 * @return char* - Beschreibung oder NULL, falls der Type ung�ltig ist
 */
const char* ModuleTypeToStr(ModuleType type) {
   switch (type) {
      case MT_EXPERT   : return("MT_EXPERT"   );
      case MT_SCRIPT   : return("MT_SCRIPT"   );
      case MT_INDICATOR: return("MT_INDICATOR");
      case MT_LIBRARY  : return("MT_LIBRARY"  );
   }
   debug("unknown module type = "+ to_string(type));
   return(NULL);
}


/**
 * Gibt die lesbare Beschreibung einer RootFunction-ID zur�ck.
 *
 * @param  RootFunction id
 *
 * @return char* - Beschreibung oder NULL, falls die ID ung�ltig ist
 */
const char* RootFunctionDescription(RootFunction id) {
   switch (id) {
      case RF_INIT  : return("init()"  );
      case RF_START : return("start()" );
      case RF_DEINIT: return("deinit()");
   }
   debug("unknown MQL root function id = "+ to_string(id));
   return(NULL);
}


/**
 * Gibt die lesbare Konstante einer RootFunction-ID zur�ck.
 *
 * @param  RootFunction id
 *
 * @return char* - Beschreibung oder NULL, falls die ID ung�ltig ist
 */
const char* RootFunctionToStr(RootFunction id) {
   switch (id) {
      case RF_INIT  : return("init()"  );
      case RF_START : return("start()" );
      case RF_DEINIT: return("deinit()");
   }
   debug("unknown MQL root function id = "+ to_string(id));
   return(NULL);
}


/**
 *
 */
int WINAPI Test() {

   debug("sizeof(LaunchType)=%d  sizeof(EXECUTION_CONTEXT)=%d", sizeof(LaunchType), sizeof(EXECUTION_CONTEXT));
   return(0);

   std::string str("Hello world");
   int len = str.length();
   char* test = new char[len+1];
   str.copy(test, len);
   test[len] = '\0';

   debug("sizeof(EXECUTION_CONTEXT) = "+ to_string(sizeof(EXECUTION_CONTEXT)));

   /*
   debug("error.code=%d  error.message=%s", error->code, error->message);
   error->code    = 200;
   char* msg      = "200: OK";
   int   bufSize  = strlen(msg)+1;
   char* buffer   = new char[bufSize];
   strcpy_s(buffer, bufSize, msg);
   error->message = buffer;
   */

   /*
   auto_ptr<char> p(new char(10));
   int len = strlen(p.get());
   */

   return(0);
   #pragma EXPORT
}
