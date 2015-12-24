//This is replacement for UnDecorateSymbolName function from imagehlp/dbghelp
//library. It supports latest MSVC mangling and CLANG mangling

#include <windows.h>

#ifdef UNNAME_EXPORTS
	#define UNNAME_API __declspec(dllexport) WINAPI
#else
	#define UNNAME_API __declspec(dllimport) WINAPI
	#ifdef _WIN64
		#pragma comment(lib, "unname64")
	#else
		#pragma comment(lib, "unname32")
	#endif // WIN64
#endif


#ifdef __cplusplus
extern "C" {
#endif
	DWORD UNNAME_API UnDecorateSymbolName(
		_In_ PCSTR name,
		_Out_writes_(maxStringLength) PSTR outputString,
		_In_ DWORD maxStringLength,
		_In_ DWORD flags);
#ifdef __cplusplus
}
#endif
