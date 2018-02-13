#pragma once

#include <Windows.h>
#include <tchar.h>
#include "resource.h"
#include "windowsx.h"
#include <cstring>

//-- Prototypes -------------------

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
LPTSTR g_lpszClassName = TEXT("sp_pr2-5_class");
LPTSTR g_lpszAplicationTitle = TEXT("Разработчик: Володько Виталий, 60331-1");


HMENU g_lpszMainMenu;

#define FILENAME "FILEREV.DAT"
#define MAX_LOADSTRING 200
#define MAX_BYTES 500

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HANDLE hFile;        // Для дескриптора объекта "файл"
HANDLE hFileMap;     // Для дескриптора объекта 'проецируемый файл'
LPVOID lpvFile;      // Для адреса региона в адресном пространстве
					 // куда будет отображаться файл

LPSTR  lpchANSI;     // Указатель на ANSI строку

DWORD  dwFileSize;   // Для значения размера файла 
LPTSTR lpszCmdLineT; // Указатель на командную строку 
					 // в ANSI или UNICODE

STARTUPINFO si = { 0 };    // Структуры для функции
PROCESS_INFORMATION pi;    // CreateProcess 
