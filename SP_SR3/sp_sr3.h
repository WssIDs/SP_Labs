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
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("������� 1");
LPTSTR g_lpszDestroyMessage = TEXT("������ ����� �������� � ����������� ��������� WM_DESTROY. ��������� ��������� �� Windows � ����� � ����������� ����.");

// Global variables
HINSTANCE g_hInst;
HMENU g_hMainMenu;
HMENU g_hFileMenu;
HMENU g_hEditMenu;

HMENU g_hPopupMenu;

#define IDC_LIST 402

#define IDM_FILE_CLOSEDOC                   40100  // ������� ����

#define IDM_POPUP_SELECT
#define IDM_POPUP_COPY