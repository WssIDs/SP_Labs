// VOLODKO.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "VOLODKO.h"

// This is an example of an exported variable
#pragma data_seg (".Shared")

VOLODKO_API int g_nDllCallsCount = 0;

#pragma data_seg ()

#pragma comment(linker, "/SECTION:.Shared,RWS")


VOLODKO_API int g_nFnCallsCount = 0;

VOLODKO_API int WINAPI Fun11(int a, int b)
{
	return a*b;
}

VOLODKO_API float Fun12(int a, int b, int c)
{
	return (a + b - c) / 3.0f;
}

VOLODKO_API void Fun13(double in, double * out)
{
	MessageBox(NULL, TEXT(""), TEXT(""), MB_OK);
}
