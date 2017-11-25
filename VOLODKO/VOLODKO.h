#pragma once

// define VOLODKO_EXPORTS for export
#ifdef VOLODKO_EXPORTS
#define VOLODKO_API __declspec(dllexport)
#else 
#define VOLODKO_API __declspec(dllimport)
#endif 


extern VOLODKO_API int g_nDllCallsCount;

extern VOLODKO_API int g_nFnCallsCount;

VOLODKO_API int WINAPI Fun11(int, int);
VOLODKO_API float Fun12(int, int, int);
VOLODKO_API void Fun13(double in, double *out);