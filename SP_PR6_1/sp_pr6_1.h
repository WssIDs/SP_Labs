#pragma once

#include <Windows.h>
#include <tchar.h>
#include "windowsx.h"

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);
/* Регистрация класса главного окна */
BOOL Register(HINSTANCE);
/* Создание главного окна */
HWND Create(HINSTANCE, int);


/* Обработчики сообщений */
/*--- WM_DESTROY -----------------------------------------------------*/
void km_OnDestroy(HWND hWnd);
/*--- WM_LBUTTONDOWN , WM_LBUTTONDBCLK -------------------------------*/
void km_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_LBUTTONUP */
void km_OnLButtonUp(HWND hWnd, int x, int y, UINT keyFlags);
/*--- WM_MOUSEMOVE ---------------------------------------------------*/
void  km_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
/*--- WM_CLOSE -------------------------------------------------------*/
void km_OnClose(HWND hWnd);
/*--------------------------------------------------------------------*/


//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr6_class");
LPTSTR g_lpszAplicationTitle = TEXT("Разработчик: Володько Виталий, 60331-1");

// Глобальные переменные
int     g_nPosX = 10;
int     g_nPosY = 30;

int g_xMousePos;
int g_yMousePos;

HDC g_HDC;			//Глобальный контекст для рисования следа мыши 
BOOL g_fDraw;		// Разрешает = TRUE /запрещает = FALSE режим рисования