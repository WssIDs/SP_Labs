#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "windowsx.h"

//-- Prototypes -------------------

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/* ����������� ������ �������� ���� */
BOOL Register(HINSTANCE);
/* �������� �������� ���� */
HWND Create(HINSTANCE, int);


/* ����������� ��������� */
/*--- WM_CREATE -----------------------------------------------------*/
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct);
/*--- WM_DESTROY -----------------------------------------------------*/
void km_OnDestroy(HWND hWnd);
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
LPTSTR g_lpszClassName = TEXT("sp_pr2-2_class");
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");

HMENU g_lpszMainMenu;


int * pg_nFnCallsCount;
int * pg_nDllCallsCount;

typedef int(WINAPI * LPFN_FUN11)(int, int);
typedef float(*LPFN_FUN12)(int,int,int);
typedef void(*LPFN_FUN13)(double, double *);

LPFN_FUN11 Fun11;
LPFN_FUN12 Fun12;
LPFN_FUN13 Fun13;