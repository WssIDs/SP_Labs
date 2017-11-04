#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "windowsx.h"

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);
/* ����������� ������ �������� ���� */
BOOL Register(HINSTANCE);
/* �������� �������� ���� */
HWND Create(HINSTANCE, int);


/* ����������� �������� */
/*--- WM_DESTROY -----------------------------------------------------*/
BOOL  km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct);
/*--- WM_DESTROY -----------------------------------------------------*/
void  km_OnDestroy(HWND hwnd);
/*--- WM_LBUTTONDOWN , WM_LBUTTONDBCLK -------------------------------*/
void  km_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_RBUTTONDOWN , WM_RBUTTONDBCLK -------------------------------*/
void  km_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_PAINT -------------------------------------------------------*/
void  km_OnPaint(HWND hwnd);
/*--- WM_CLOSE -------------------------------------------------------*/
void  km_OnClose(HWND hwnd);
/*--------------------------------------------------------------------*/


/* �������� �������� ���� */
BOOL CreateMenuItem(HMENU, char*, UINT, UINT, HMENU, BOOL, UINT);

// -- Handle Messages ----------


//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszlistboxClassName = TEXT("Button");
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("������� 1");
LPTSTR g_lpszDestroyMessage = TEXT("������ ����� �������� � ����������� ��������� WM_DESTROY. ��������� ��������� �� Windows � ����� � ����������� ����.");

HWND g_hList;

// Global variables
HMENU		g_lpszMainMenu;
HMENU		g_lpszFileMenu;
HMENU		g_lpszEditMenu;

#define IDC_LIST							402		// ListBox
#define IDM_FILE_CLOSEDOC                   40100  // ������� ����
