#pragma once

#include <Windows.h>
#include <tchar.h>
#include "windowsx.h"

//-- Prototypes -------------------
LRESULT CALLBACK Pr2_WndProc(HWND, UINT, WPARAM, LPARAM);
/* ����������� ������ �������� ���� */
BOOL Register(HINSTANCE);
/* �������� �������� ���� */
HWND Create(HINSTANCE, int);


/* ����������� ��������� */
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
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");

// ���������� ����������
int     g_nPosX = 10;
int     g_nPosY = 30;

int g_xMousePos;
int g_yMousePos;

HDC g_HDC;			//���������� �������� ��� ��������� ����� ���� 
BOOL g_fDraw;		// ��������� = TRUE /��������� = FALSE ����� ���������