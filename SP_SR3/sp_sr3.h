#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CreateMenuItem(HMENU, char*, UINT, UINT, HMENU, BOOL, UINT);

//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszlistboxClassName = TEXT("Button");
LPTSTR g_lpszAplicationTitle = TEXT("Разработчик: Володько Виталий, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("Вариант 1");
LPTSTR g_lpszDestroyMessage = TEXT("Данный вывод выполнен в обработчике сообщения WM_DESTROY. Сообщение поступило от Windows в связи с разрушением окна.");

// Global variables
HINSTANCE	g_hInst;
HMENU		g_lpszMainMenu;
HMENU		g_lpszFileMenu;
HMENU		g_lpszEditMenu;

#define IDC_LIST							402		// ListBox
#define IDM_FILE_CLOSEDOC                   40100  // Элемент меню

#define CM_EDIT_SELECT						40101
#define CM_EDIT_COPY						40102
