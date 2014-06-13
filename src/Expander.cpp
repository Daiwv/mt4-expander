#define WIN32_LEAN_AND_MEAN             
#include <windows.h>

#define EXPORT_FUNCTION comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)


/**
 *
 */
struct MqlStr {
   int   length;
   char* string;
};


/**
 * DLL entry point 
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved) {
	return(TRUE);
}


/**
 * Gibt die Speicheradresse eines MQL-Bool-Arrays zur�ck.
 *
 * @param  int array[] - MQL-Bool-Array (in MetaTrader als Integer-Array implementiert)
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetBoolsAddress(bool array[]);
 */
int GetBoolsAddress(const int array[]) {
   return((int) array);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-Integer-Arrays zur�ck.
 *
 * @param  int array[] - MQL-Integer-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetIntsAddress(int array[]);
 */
int GetIntsAddress(const int array[]) {
   return((int) array);

   #pragma EXPORT_FUNCTION
}


/**
 * Alias f�r GetIntsAddress()
 *
 * MQL-Importdeklaration:  int GetBufferAddress(int buffer[]);
 */
int GetBufferAddress(const int array[]) {
   return(GetIntsAddress(array));

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-Double-Arrays zur�ck.
 *
 * @param  double array[] - MQL-Double-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetDoublesAddress(double array[]);
 */
int GetDoublesAddress(const double array[]) {
   return((int) array);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines MQL-String-Arrays zur�ck.
 *
 * @param  MqlStr array[] - MQL-String-Array
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetStringsAddress(string array[]);
 */
int GetStringsAddress(const MqlStr array[]) {
   return((int) array);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt die Speicheradresse eines Strings zur�ck.
 *
 * @param  char* value - String (MetaTrader �bergibt f�r einen MQL-String das Element MqlStr.string)
 *
 * @return int - Speicheradresse
 *
 *
 * MQL-Importdeklaration:  int GetStringAddress(string value);
 */
int GetStringAddress(const char* value) {
   return((int) value);

   #pragma EXPORT_FUNCTION
}


/**
 * Gibt den �bergebenen Zeiger auf einen String selbst zur�ck. Kann in MQL zum Lesen eines Strings von einer 
 * Adresse verwendet werden, da MetaTrader den Zeiger automatisch in einen MQL-String konvertiert.
 *
 * @param  char* value - String, @see struct MqlStr.string
 *
 * @return char* - derselbe Zeiger
 *
 *
 * MQL-Importdeklaration:  string GetString(int address);
 */
char* GetString(const char* value) {
   return((char*) value);

   #pragma EXPORT_FUNCTION
}
