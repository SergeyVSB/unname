// unname.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "unname.h"
#include <windows.h>
#include <stdlib.h> 
#include <malloc.h>
#include <memory>


typedef char *        pchar_t;
typedef const char *  pcchar_t;

typedef void * (__cdecl * Alloc_t)(size_t);
typedef void   (__cdecl * Free_t )(void *);
typedef char * (__cdecl * GetParameter_t)(long);

extern "C"
{
	/*
	*	This description is from MS CRT sources
	*
	*	This function will undecorate a name, returning the string corresponding to
	*	the C++ declaration needed to produce the name.  Its has a similar interface
	*	to 'strncpy'.
	*
	*	If the target string 'outputString' is specified to be NULL, a string of
	*	suitable length will be allocated and its address returned.  If the returned
	*	string is allocated by 'unDName', then it is the programmers responsibility
	*	to deallocate it.  It will have been allocated on the far heap.
	*
	*	If the target string is not NULL, then the parameter 'maxStringLength' will
	*	specify the maximum number of characters which may be placed in the string.
	*	In this case, the returned value is the same as 'outputString'.
	*
	*	Both the input parameter 'name' and the returned string are NULL terminated
	*	strings of characters.
	*
	*	If the returned value is NULL, it indicates that the undecorator ran out of
	*	memory, or an internal error occurred, and was unable to complete its task.
	*/
	
	pchar_t _VCRTIMP __unDName(_Out_opt_z_cap_(maxStringLength) pchar_t outputString,
		pcchar_t name,
		int maxStringLength,	// Note, COMMA is leading following optional arguments
		Alloc_t pAlloc,
		Free_t pFree,
		unsigned short disableFlags
		);
}


namespace __cxxabiv1
{
	extern "C"
		char*
		__cxa_demangle(const char* mangled_name, char* buf, size_t* n, int* status);
}


extern "C" DWORD UNNAME_API UnDecorateSymbolName(
	_In_ PCSTR const name,
	_Out_writes_(maxStringLength) PSTR const outputString,
	_In_ DWORD const maxStringLength,
	_In_ DWORD const flags)
{
	if (maxStringLength == 0 || outputString == nullptr)
	{
		return NULL;
	}
	size_t undnameLength = 0;
	auto pResult = std::unique_ptr<char[], decltype(free)*>{ __unDName(nullptr, name, 0, &malloc, &free, static_cast<unsigned short>(flags)), &free };

	auto cmpres = strcmp(pResult.get(), name);
	if (pResult && (cmpres != 0))
	{
		undnameLength = strlen(pResult.get());
		if (undnameLength < maxStringLength)
		{
			memcpy(outputString, pResult.get(), undnameLength);
			outputString[undnameLength] = '\0';
		}
		else
		{
			memcpy(outputString, pResult.get(), maxStringLength);
		}
		outputString[maxStringLength - 1] = '\0';

		return static_cast<DWORD>(strlen(outputString));
	}

	int status = 0;
	pResult.reset(__cxxabiv1::__cxa_demangle(name, nullptr, &undnameLength, &status));
	if (pResult && (status == 0))
	{
		if (undnameLength < maxStringLength)
		{
			memcpy(outputString, pResult.get(), undnameLength);
			outputString[undnameLength] = '\0';
		}
		else
		{
			memcpy(outputString, pResult.get(), maxStringLength);
		}
		outputString[maxStringLength - 1] = '\0';

		return static_cast<DWORD>(strlen(outputString));
	}

	undnameLength = strlen(name);
	if (undnameLength < maxStringLength)
	{
		memcpy(outputString, name, undnameLength);
		outputString[undnameLength] = '\0';
	}
	else
	{
		memcpy(outputString, name, maxStringLength);
	}
	outputString[maxStringLength - 1] = '\0';

	return static_cast<DWORD>(strlen(outputString));
}
#ifndef _WIN64
//#pragma comment(linker, "/EXPORT:UnDecorateSymbolName=_UnDecorateSymbolName@16")
#endif // !_WIN64

//imagehlp.dll
//unname32



