#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "Windowsx.h"

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);


//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszlistboxClassName = TEXT("Button");
LPTSTR g_lpszAplicationTitle = TEXT("Разработчик: Володько Виталий, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("Вариант 1");
LPTSTR g_lpszDestroyMessage = TEXT("Данный вывод выполнен в обработчике сообщения WM_DESTROY. Сообщение поступило от Windows в связи с разрушением окна.");
HINSTANCE g_hInst;


#define IDC_LIST 402