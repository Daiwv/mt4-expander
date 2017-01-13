#pragma once

#include "stdafx.h"
#include "shared/defines.h"                                                      // shared between DLL and MQL
#include "shared/errors.h"                                                       // ...

#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>

#pragma warning(push)
#pragma warning(disable:4060)          // switch statement contains no 'case' or 'default' labels
#pragma warning(disable:4065)          // switch statement contains 'default' but no 'case' labels
#pragma warning(disable:4101)          // unreferenced local variable
#pragma warning(disable:4996)          // std::string<>::copy: Function call with parameters that may be unsafe


//#include <algorithm>


typedef unsigned   int   uint;
typedef          __int64 int64;
typedef unsigned __int64 uint64;
#define _USE_32BIT_TIME_T
typedef time_t           datetime;     // a 32-bit signed long
typedef std::string      string;


#define INVALID_HWND  ((HWND)-1)


/**
 * Ermittelt die Gr��e eines Elements eines Typs ohne vorhandene Instanz.
 */
#define sizeofMember(type, member) sizeof(((type*)NULL)->member)


/**
 * to_string() replacement for missing C++11 support in VS 2008.
 *
 * @param  <typename T> value
 *
 * @return string
 *
 * Note: Template functions must be declared and defined in the same place.
 */
template <typename T> string to_string(T value) {
   std::ostringstream os;
   os << value;
   return(os.str());
}


/**
 * std::getline() replacement for reading lines with any line endings.
 */
std::istream& getLine(std::istream &is, string &line);


#ifdef EXPANDER_EXPORTS
 //#define EXPANDER_API extern "C" __declspec(dllexport)
   #define EXPANDER_API                                                          // empty define
   #define EXPORT       comment(linker, "/EXPORT:"__FUNCTION__"="__FUNCDNAME__)  // export unmangled names without DEF file
#else
   #define EXPANDER_API extern "C" __declspec(dllimport)
#endif


#define MAX_CHART_DESCRIPTION_LENGTH   ((MAX_SYMBOL_LENGTH) + 1 + 7)             // "SYMBOL,Monthly"


// MQL program types
enum ProgramType {
   PT_INDICATOR = PROGRAMTYPE_INDICATOR,
   PT_EXPERT    = PROGRAMTYPE_EXPERT,
   PT_SCRIPT    = PROGRAMTYPE_SCRIPT
};


// MQL module types                          // implemented as flags
enum ModuleType {
   MT_INDICATOR = MODULETYPE_INDICATOR,      // 1
   MT_EXPERT    = MODULETYPE_EXPERT,         // 2
   MT_SCRIPT    = MODULETYPE_SCRIPT,         // 4
   MT_LIBRARY   = MODULETYPE_LIBRARY         // 8
};


// MQL program launch types
enum LaunchType {
   LT_TEMPLATE  = LAUNCHTYPE_TEMPLATE,       // via template
   LT_PROGRAM   = LAUNCHTYPE_PROGRAM,        // via call of iCustom()
   LT_MANUAL    = LAUNCHTYPE_MANUAL          // manually (by hand)
};


// MQL program root functions
enum RootFunction {
   RF_INIT      = ROOTFUNCTION_INIT,
   RF_START     = ROOTFUNCTION_START,
   RF_DEINIT    = ROOTFUNCTION_DEINIT
};


// MQL program initialize reasons
enum InitializeReason {
   IR_USER              = INITREASON_USER,
   IR_TEMPLATE          = INITREASON_TEMPLATE,
   IR_PROGRAM           = INITREASON_PROGRAM,
   IR_PROGRAM_AFTERTEST = INITREASON_PROGRAM_AFTERTEST,
   IR_PARAMETERS        = INITREASON_PARAMETERS,
   IR_TIMEFRAMECHANGE   = INITREASON_TIMEFRAMECHANGE,
   IR_SYMBOLCHANGE      = INITREASON_SYMBOLCHANGE,
   IR_RECOMPILE         = INITREASON_RECOMPILE
};


// MQL program uninitialize reasons
enum UninitializeReason {
   UR_UNDEFINED         = UNINITREASON_UNDEFINED,
   UR_REMOVE            = UNINITREASON_REMOVE,
   UR_RECOMPILE         = UNINITREASON_RECOMPILE,
   UR_CHARTCHANGE       = UNINITREASON_CHARTCHANGE,
   UR_CHARTCLOSE        = UNINITREASON_CHARTCLOSE,
   UR_PARAMETERS        = UNINITREASON_PARAMETERS,
   UR_ACCOUNT           = UNINITREASON_ACCOUNT,
   UR_TEMPLATE          = UNINITREASON_TEMPLATE,       // since terminal build > 509
   UR_INITFAILED        = UNINITREASON_INITFAILED,     // ...
   UR_CLOSE             = UNINITREASON_CLOSE           // ...
};


// structs
#pragma pack(push, 1)

#include "structs/mt4/FxtHeader.h"
#include "structs/mt4/HistoryBar400.h"
#include "structs/mt4/HistoryBar401.h"
#include "structs/mt4/HistoryHeader.h"
#include "structs/mt4/MqlStr.h"
#include "structs/mt4/Symbol.h"
#include "structs/mt4/SymbolGroup.h"
#include "structs/mt4/SymbolSelected.h"
#include "structs/mt4/Tick.h"

#include "structs/myfx/LogMessage.h"
#include "structs/myfx/ExecutionContext.h"
#include "structs/myfx/Test.h"

#include "structs/win32/FileTime.h"
#include "structs/win32/ProcessInformation.h"
#include "structs/win32/SecurityAttributes.h"
#include "structs/win32/StartupInfo.h"
#include "structs/win32/SystemTime.h"
#include "structs/win32/TimeZoneInformation.h"
#include "structs/win32/Win32FindData.h"


// application functionality
#include "context.h"
#include "utils/math.h"
#include "utils/string.h"


// type definitions
typedef std::vector<EXECUTION_CONTEXT*> ContextChain;                // contexts of a single MQL program (i.e. MQL modules)


// external var declarations for context management
extern std::vector<ContextChain> contextChains;                      // all context chains (i.e. MQL programs, index = program id)
extern std::vector<DWORD>        threads;                            // ID's aller bekannten Threads
extern std::vector<uint>         threadsPrograms;                    // ID's des vom Thread zuletzt ausgef�hrten MQL-Programms
extern uint                      lastUIThreadProgram;                // ID des vom UI-Thread zuletzt ausgef�hrten MQL-Programm
extern CRITICAL_SECTION          terminalLock;                       // application wide lock


// function declarations
#define debug(...)   _debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define  warn(...)   _warn (__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define error(...)   _error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)


int   _debug(const char* fileName, const char* funcName, int line, const char*   format, ...);
int   _debug(const char* fileName, const char* funcName, int line, const string &format, ...);
void __debug(const char* fileName, const char* funcName, int line, const char*   format, const va_list &args);

int   _warn (const char* fileName, const char* funcName, int line, int code, const char*   format, ...);
int   _warn (const char* fileName, const char* funcName, int line, int code, const string &format, ...);
void __warn (const char* fileName, const char* funcName, int line, int code, const char*   format, const va_list &args);

int   _error(const char* fileName, const char* funcName, int line, int code, const char*   format, ...);
int   _error(const char* fileName, const char* funcName, int line, int code, const string &format, ...);
void __error(const char* fileName, const char* funcName, int line, int code, const char*   format, const va_list &args);


const char*   WINAPI BoolToStr               (BOOL value);
const char*   WINAPI DeinitFlagsToStr        (uint flags);
const char*   WINAPI DoubleQuoteStr          (const char* value);
string        WINAPI doubleQuoteStr          (const char* value);
string        WINAPI doubleQuoteStr          (const string &value);
const char*   WINAPI ErrorToStr              (int error);
HWND          WINAPI GetApplicationWindow();
uint          WINAPI GetBoolsAddress         (const BOOL values[]);
uint          WINAPI GetChartDescription     (const char* symbol, uint timeframe, char* buffer, uint bufferSize);
uint          WINAPI GetDoublesAddress       (const double values[]);
datetime      WINAPI GetGmtTime();
uint          WINAPI GetIntsAddress          (const int values[]);
int           WINAPI GetLastWin32Error();
datetime      WINAPI GetLocalTime();
uint          WINAPI GetTerminalBuild();
const string& WINAPI getTerminalPath();
const char*   WINAPI GetTerminalVersion();
BOOL          WINAPI GetTerminalVersion      (uint* major, uint* minor, uint* hotfix, uint* build);
DWORD         WINAPI GetUIThreadId();
HANDLE        WINAPI GetWindowProperty       (HWND hWnd, const char* lpName);
const char*   WINAPI InitFlagsToStr          (uint flags);
const char*   WINAPI InitReasonToStr         (InitializeReason reason);
const char*   WINAPI InitializeReasonToStr   (InitializeReason reason);          // Alias
const char*   WINAPI IntToHexStr             (int value);
BOOL          WINAPI IsCustomTimeframe       (int timeframe);
BOOL          WINAPI IsStdTimeframe          (int timeframe);
BOOL          WINAPI IsUIThread();
const char*   WINAPI ModuleTypeDescription   (ModuleType type);
const char*   WINAPI ModuleTypeToStr         (ModuleType type);
uint          WINAPI MT4InternalMsg();

// format codes:
// @see  http://www.cplusplus.com/reference/cstdio/printf/
// @see  ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/dv_vccrt/html/664b1717-2760-4c61-bd9c-22eee618d825.htm
string        WINAPI numberFormat            (double value, const char* format);
const char*   WINAPI NumberFormat            (double value, const char* format);
string        WINAPI numberToStr             (double value, const char* format); // Alias
const char*   WINAPI NumberToStr             (double value, const char* format); // Alias

const char*   WINAPI OperationTypeToStr      (int type);
const char*   WINAPI OperationTypeDescription(int type);
const char*   WINAPI OrderTypeToStr          (int type);                         // Alias
const char*   WINAPI OrderTypeDescription    (int type);                         // Alias
const char*   WINAPI PeriodDescription       (int period);
const char*   WINAPI PeriodToStr             (int period);
const char*   WINAPI ProgramTypeDescription  (ProgramType type);
const char*   WINAPI ProgramTypeToStr        (ProgramType type);
BOOL          WINAPI RemoveTickTimer         (int timerId);
void          WINAPI RemoveTickTimers();
HANDLE        WINAPI RemoveWindowProperty    (HWND hWnd, const char* lpName);
const char*   WINAPI RootFunctionDescription (RootFunction id);
const char*   WINAPI RootFunctionToStr       (RootFunction id);
uint          WINAPI SetupTickTimer          (HWND hWnd, int millis, DWORD flags=NULL);
BOOL          WINAPI SetWindowProperty       (HWND hWnd, const char* lpName, HANDLE value);
BOOL          WINAPI ShiftIndicatorBuffer    (double buffer[], int bufferSize, int bars, double emptyValue);
const char*   WINAPI ShowWindowCmdToStr      (int cmdShow);
const char*   WINAPI StringToStr             (const char* value);
const char*   WINAPI TimeframeDescription    (int timeframe);                    // Alias
const char*   WINAPI TimeframeToStr          (int timeframe);                    // Alias
VOID        CALLBACK TimerCallback           (HWND hWnd, UINT msg, UINT_PTR timerId, DWORD time);
const char*   WINAPI UninitReasonToStr       (UninitializeReason reason);
const char*   WINAPI UninitializeReasonToStr (UninitializeReason reason);        // Alias


/**
 * Pseudo functions returning fixed values a ccording to their name. All parameters are ignored.
 */
int         WINAPI _CLR_NONE    (...);
int         WINAPI _EMPTY       (...);
const char* WINAPI _EMPTY_STR   (...);
HWND        WINAPI _INVALID_HWND(...);
int         WINAPI _NULL        (...);
bool        WINAPI _true        (...);
bool        WINAPI _false       (...);
BOOL        WINAPI _TRUE        (...);
BOOL        WINAPI _FALSE       (...);


/**
 * Pseudo functions returning variable values according to their name. All parameters except the first one are ignored.
 */
bool   WINAPI _bool  (bool   value, ...);
char   WINAPI _char  (char   value, ...);
int    WINAPI _int   (int    value, ...);
float  WINAPI _float (float  value, ...);
double WINAPI _double(double value, ...);
BOOL   WINAPI _BOOL  (BOOL   value, ...);