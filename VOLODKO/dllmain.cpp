// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "VOLODKO.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{

	TCHAR moduleName[260];
	GetModuleFileName(hModule, moduleName, 260);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		TCHAR moduleLoadedText[300];
		wsprintf(moduleLoadedText, TEXT("Загружается библиотека %s"), moduleName);

		MessageBox(NULL, moduleLoadedText, moduleName, MB_OK);

		g_nDllCallsCount++;
	}
	return TRUE;
    case DLL_THREAD_ATTACH:
	break;
    case DLL_THREAD_DETACH:
	break;
    case DLL_PROCESS_DETACH:
	{
		TCHAR moduleUnloadedText[300];
		wsprintf(moduleUnloadedText, TEXT("Выгружается библиотека %s"), moduleName);

		MessageBox(NULL, moduleUnloadedText, moduleName, MB_OK);

		g_nDllCallsCount--;
	}
	return TRUE;
        break;
    }
    return TRUE;
}

