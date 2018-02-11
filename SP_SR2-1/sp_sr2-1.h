#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "windowsx.h"
#include "Psapi.h"

//-- Prototypes -------------------
BOOL CALLBACK ProcessDlgProc(HWND, UINT, WPARAM, LPARAM);

// Загрузка текста
BOOL CALLBACK ModLoadDlgProc(HWND, UINT, WPARAM, LPARAM);


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/* Регистрация класса главного окна */
BOOL Register(HINSTANCE);
/* Создание главного окна */
HWND Create(HINSTANCE, int);


/* Обработчики сообщений */
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
LPTSTR g_lpszClassName = TEXT("sp_pr2-1_class");
LPTSTR g_lpszlistboxClassName = TEXT("Button");
LPTSTR g_lpszAplicationTitle = TEXT("Разработчик: Володько Виталий, 60331-1");
LPTSTR g_lpszDestroyTitle = TEXT("Вариант 1");

HMENU g_lpszMainMenu;


struct PROCSTRUCT
{
	HANDLE	ProcHandle;
	DWORD	ProcId;
	HANDLE	ThreadHandle;
	DWORD	ThreadId;
	LPTSTR	ProcImage;
	TCHAR   CmdParam[260];
};


PROCSTRUCT g_lpProcess[4];
TCHAR lpszFileSpec[256];

#define MAX_BYTES  10000
#define IDC_EDIT 200
static HWND hEditMainWin;
