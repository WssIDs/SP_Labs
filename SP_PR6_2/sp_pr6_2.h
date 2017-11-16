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

BOOL CALLBACK FrameDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL DrawLine(HDC hdc, int x0, int y0, int x, int y);

/* ����������� ��������� */
/*--- WM_CREATE -----------------------------------------------------*/
BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct);
/*--- WM_DESTROY -----------------------------------------------------*/
void km_OnDestroy(HWND hWnd);
/*--- WM_PAINT -------------------------------------------------------*/
void km_OnPaint(HWND hWnd);
/*--- WM_CLOSE -------------------------------------------------------*/
void km_OnClose(HWND hWnd);
/*--- WM_COMMAND ----------------------------------------------------------*/
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
/*--- WM_KEYDOWN --------------------------------------------------------*/
void km_OnKeyDown(HWND hWnd, UINT nVirtKey, BOOL fDown, int cRepeat, UINT flags);


/*--- WM_LBUTTONDOWN , WM_LBUTTONDBCLK -------------------------------*/
void FrameDlg_OnLButtonDown(HWND hDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_LBUTTONUP */
void FrameDlg_OnLButtonUp(HWND hDlg, int x, int y, UINT keyFlags);
/*--- WM_MOUSEMOVE ---------------------------------------------------*/
void  FrameDlg_OnMouseMove(HWND hDlg, int x, int y, UINT keyFlags);

//-- Global Variables ------------ 
LPTSTR g_lpszClassName = TEXT("sp_pr6_class");
LPTSTR g_lpszAplicationTitle = TEXT("�����������: �������� �������, 60331-1");


HWND g_hFrameWnd = NULL;
HDC g_hDC;

BOOL g_fDraw;			// ��������/��������� ����� ���������

RECT g_rcClient;                 // ������������� ���������� ������� 
POINT g_ptClientUL;              // ���������� ������� ����� ����
POINT g_ptClientLR;              // ���������� ������ ������ ���� 

POINT ptClientUpL;              // ���������� ������� ����� ����
POINT ptClientLowR;              // ���������� ������ ������ ���� 

int g_count;


