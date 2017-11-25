#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "windowsx.h"

//-- Prototypes -------------------
BOOL CALLBACK ProcessDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ThreadDlgProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/* ����������� ������ �������� ���� */
BOOL Register(HINSTANCE);
/* �������� �������� ���� */
HWND Create(HINSTANCE, int);


DWORD WINAPI ThreadFunc1(PVOID);
DWORD WINAPI ThreadFuncWait(PVOID);

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
HMENU g_lpszThread1Menu;

struct PROCSTRUCT
{
	HANDLE	ProcHandle;
	DWORD	ProcId;
	HANDLE	ThreadHandle;
	DWORD	ThreadId;
	LPTSTR	ProcImage;
	TCHAR   CmdParam[260];
};

struct THREAD_STRUCT
{
	HANDLE ThreadHandle;
	DWORD ThreadId;
};

PROCSTRUCT g_lpProcess[4];
THREAD_STRUCT g_lpThread[2];

UINT g_uXPos = 10;
UINT g_uYPos = 50;

struct THREAD_PARAM
{
	int Num;  	//  ����� ������
	UINT XPos; 	//  ������� X ������� ������
	UINT YPos;	//  ������� Y ������� ������
	HWND hWnd; 	//  ��������� ���� ������
};

THREAD_PARAM ThrParam1 = { 1, g_uXPos, g_uYPos, NULL };
THREAD_PARAM ThrParam2 = { 1, g_uXPos, g_uYPos+50, NULL };

