#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "windowsx.h"

//-- Prototypes -------------------
BOOL CALLBACK ModAboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ModLoadDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK LoadBmpDlgProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);
/* ����������� ������ �������� ���� */
BOOL Register(HINSTANCE);
/* �������� �������� ���� */
HWND Create(HINSTANCE, int);


/* ����������� ��������� */
/*--- WM_CREATE -----------------------------------------------------*/
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct);
/*--- WM_DESTROY -----------------------------------------------------*/
void km_OnDestroy(HWND hWnd);
/*--- WM_LBUTTONDOWN , WM_LBUTTONDBCLK -------------------------------*/
void km_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_RBUTTONDOWN , WM_RBUTTONDBCLK -------------------------------*/
void km_OnRButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_PAINT -------------------------------------------------------*/
void km_OnPaint(HWND hWnd);
/*--- WM_CLOSE -------------------------------------------------------*/
void km_OnClose(HWND hWnd);
/*--- WM_MENUSELECT -------------------------------------------------------*/
void km_OnMenuSelect(HWND hWnd, HMENU hmenu, int item, HMENU hmenuPopup, UINT flags);
/*--- WM_COMMAND ----------------------------------------------------------*/
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
/*--------------------------------------------------------------------*/


//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr2_class");
LPTSTR g_lpszlistboxClassName = TEXT("Button");
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("������� 1");
LPTSTR g_lpszDestroyMessage = TEXT("������ ����� �������� � ����������� ��������� WM_DESTROY. ��������� ��������� �� Windows � ����� � ����������� ����.");


#define MAX_BYTES  10000

TCHAR lpszFileSpec[256];
TCHAR Buffer[MAX_BYTES] = TEXT("��� 1. ������ ������");

HWND g_hwndDlg; // ���������� ���� ����������� ���������� ������
HANDLE g_hFile = NULL;

// Global variables
HMENU		g_lpszMainMenu;
HMENU		g_lpszFileMenu;
HMENU		g_lpszViewMenu;

HWND g_hEdit;

#define IDC_EDIT		40100 
