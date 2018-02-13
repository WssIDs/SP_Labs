#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "windowsx.h"
#include <cstring>

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
LPTSTR g_lpszClassName = TEXT("sp_pr2-5_class");
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");


HMENU g_lpszMainMenu;

#define FILENAME "FILEREV.DAT"
#define MAX_LOADSTRING 200
#define MAX_BYTES 500

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE hFile;        // ��� ����������� ������� "����"
HANDLE hFileMap;     // ��� ����������� ������� '������������ ����'
LPVOID lpvFile;      // ��� ������ ������� � �������� ������������
					 // ���� ����� ������������ ����

LPSTR  lpchANSI;     // ��������� �� ANSI ������

DWORD  dwFileSize;   // ��� �������� ������� ����� 
LPTSTR lpszCmdLineT; // ��������� �� ��������� ������ 
					 // � ANSI ��� UNICODE

STARTUPINFO si = { 0 };    // ��������� ��� �������
PROCESS_INFORMATION pi;    // CreateProcess 
