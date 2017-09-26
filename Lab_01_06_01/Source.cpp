// ��������� ����� ���� � ���������� 
// ���������� ����������
// Platform SDK/User Interface Services/User Input/Keyboard Input/
// Keyboard Input Reference/Keyboard Input Messages

#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include  <stdio.h>
#include <tchar.h>

// ���������� ����������
int     nPosX = 10;
int     nPosY = 30;
LPCSTR  lpszAppName = "������������ ��������� ����� ���� � ����������";
LPCSTR lpszClassName = "Key And Mause Demo Class";
HWND    hMainWnd;
HDC g_HDC;
BOOL g_fDraw;


//========��������������� ���������� �������=========================
/* ��������� �������� ���� */
LRESULT WINAPI WndProc(HWND, UINT, WPARAM, LPARAM);
/* ����������� ������ �������� ���� */
BOOL Register(HINSTANCE);
/* �������� �������� ���� */
HWND Create(HINSTANCE, int);

/* ����������� �������� */
/*--- WM_DESTROY -----------------------------------------------------*/
void  km_OnDestroy(HWND hwnd);
/*--- WM_CHAR --------------------------------------------------------*/
void  km_OnChar(HWND hwnd, UINT ch, int cRepeat);
/*--- WM_KEYUP,WM_KEYDOWN --------------------------------------------*/
void  km_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
/*--- WM_SYSKEYDOWN, WM_SYSKEYUP  ------------------------------------*/
void  km_OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
/*--- WM_LBUTTONDOWN, WM_LBUTTONDBLCLK -------------------------------*/
void  km_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
/*--- WM_LBUTTONUP ---------------------------------------------------*/
void  km_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
/*--- WM_MOUSEMOVE ---------------------------------------------------*/
void  km_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
/*--- WM_PAINT -------------------------------------------------------*/
void  km_OnPaint(HWND hwnd);
/*--------------------------------------------------------------------*/
//====================================================================//


// ����� ����� � ��������� ===========================================//
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	MSG Msg;
	BOOL ok;

	ok = Register(hInst);
	if (!ok) return FALSE;

	hMainWnd = Create(hInst, nCmdShow);
	if (!hMainWnd) return FALSE;
	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

//== ������� ��� ����������� ������ ���� =============================//
BOOL Register(HINSTANCE hinst)
{
	WNDCLASSEX WndClass;
	BOOL fRes;

	memset(&WndClass, 0, sizeof(WNDCLASSEX));
	WndClass.cbSize = sizeof(WNDCLASSEX);
	WndClass.lpszClassName = lpszClassName;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hinst;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.lpszMenuName = NULL;

	fRes = (BOOL)RegisterClassEx(&WndClass);
	return fRes;
}

//== ������� �������� ���� ===========================================//
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
	HWND hwnd = CreateWindowEx(0,
		lpszClassName, lpszAppName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) return hwnd;

	g_HDC = GetDC(hwnd);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	return hwnd;
}

//== ������� ��������� �������� ���� =================================//
LRESULT WINAPI
WndProc(HWND hwnd, UINT MesId, WPARAM wParam, LPARAM lParam)
{
	static BOOL fDraw;

	switch (MesId)
	{
		HANDLE_MSG(hwnd, WM_DESTROY, km_OnDestroy);
		HANDLE_MSG(hwnd, WM_CHAR, km_OnChar);
		HANDLE_MSG(hwnd, WM_KEYDOWN, km_OnKey);
		HANDLE_MSG(hwnd, WM_KEYUP, km_OnKey);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, km_OnMouseMove);
		HANDLE_MSG(hwnd, WM_LBUTTONDBLCLK, km_OnLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, km_OnLButtonDown);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, km_OnLButtonUp);
		HANDLE_MSG(hwnd, WM_PAINT, km_OnPaint);
		HANDLE_MSG(hwnd, WM_SYSKEYUP, km_OnSysKey);
		HANDLE_MSG(hwnd, WM_SYSKEYDOWN, km_OnSysKey);
	default:
		return DefWindowProc(hwnd, MesId, wParam, lParam);
	}
}

//====================================================================//
//====================================================================//

//=== ���������� ��������� WM_DESTROY ================================//
void km_OnDestroy(HWND hwnd)
{
	ReleaseDC(hwnd, g_HDC);
	PostQuitMessage(0);
}

//=== ���������� ��������� WM_CHAR ===================================//
void km_OnChar(HWND hwnd, UINT ch, int cRepeat)
{
	char S[100];//����� ��� ������������ ��������� ������(100 ����)
	HDC DC = GetDC(hwnd);//�������� �������� ���������� ������������ ������

	wsprintf(S, "WM_CHAR ==> Ch = %c   cRepeat = %d    ", ch, cRepeat);
	SetBkColor(DC, GetSysColor(COLOR_WINDOW));//������ ���� ����
	TextOut(DC, nPosX, nPosY + 20, S, strlen(S));//������� ������

	ReleaseDC(hwnd, DC);//����������� ��������
}

//=== ���������� ��������� WM_KEYUP,WM_KEYDOWN =======================//
void km_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	char S[100];//����� ��� ������������ ��������� ������(100 ����)
	HDC DC = GetDC(hwnd);//�������� �������� ���������� ������������ ������

	if (fDown) { //������� ������
		wsprintf(S, "WM_KEYDOWN ==> vk = %d fDown = %d cRepeat = %d flags = %d    ",
			vk, fDown, cRepeat, flags);
	}
	else {//������� ��������
		wsprintf(S, "WM_KEYUP ==> vk = %d fDown = %d cRepeat = %d flags = %d      ",
			vk, fDown, cRepeat, flags);
	}
	SetBkColor(DC, GetSysColor(COLOR_WINDOW));//������ ���� ����
	TextOut(DC, nPosX, nPosY + 40, S, strlen(S));//������� ������
	ReleaseDC(hwnd, DC);//����������� ��������
}


//=== ���������� ��������� WM_LBUTTONDOWN, WM_LBUTTONDBLCLK ==========//
void km_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x,
	int y, UINT keyFlags)
{
	char S[100];//����� ��� ������������ ��������� ������(100 ����)
	HDC DC = GetDC(hwnd);//�������� �������� ���������� ������������ ������

	if (fDoubleClick) //������� ������
		wsprintf(S, "WM_LBUTTONDBLCLK ==> Db = %d x = %d y = %d Flags = %d ",
			fDoubleClick, x, y, keyFlags);
	else // ��������� ������ 
	{
		g_fDraw = TRUE;
		MoveToEx(g_HDC, x, y, NULL);
		SetCapture(hwnd);

		wsprintf(S, "WM_LBUTTONDOWN ==> Db = %d x = %d y = %d Flags = %d ",
			fDoubleClick, x, y, keyFlags);
	}

	SetBkColor(DC, GetSysColor(COLOR_WINDOW));//������ ���� ����
	TextOut(DC, nPosX, nPosY + 100, S, strlen(S));//������� ������
	ReleaseDC(hwnd, DC);//����������� ��������
}

//=== ���������� ��������� WM_LBUTTONUP ==============================//
void km_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	char S[100];//����� ��� ������������ ��������� ������(100 ����)
	HDC  DC = GetDC(hwnd);//�������� �������� ���������� ������������ ������

	wsprintf(S, "MM LBUTTONUP ==> x = %d y = %d F = %d   ",
		x, y, keyFlags);

	g_fDraw = FALSE;
	ReleaseCapture();

	SetBkColor(DC, GetSysColor(COLOR_WINDOW));
	TextOut(DC, nPosX, nPosY + 120, S, strlen(S));
	ReleaseDC(hwnd, DC);//����������� ��������
}

//=== ���������� ��������� WM_MOUSEMOVE ==============================//
void km_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{

	char S[100];//����� ��� ������������ ��������� ������(100 ����)
	HDC DC = GetDC(hwnd);//�������� �������� ���������� ������������ ������

	wsprintf(S, "WM_MOUSEMOVE ==> x = %d y = %d keyFlags = %d    ",
		x, y, keyFlags);
	//������ ���� � ����������� �� ������� ������ ���� � ���������� 
	if ((keyFlags & MK_CONTROL) == MK_CONTROL) SetTextColor(DC, RGB(0, 0, 255));
	if ((keyFlags & MK_LBUTTON) == MK_LBUTTON) SetTextColor(DC, RGB(0, 255, 0));
	if ((keyFlags & MK_RBUTTON) == MK_RBUTTON) SetTextColor(DC, RGB(255, 0, 0));
	if ((keyFlags & MK_SHIFT) == MK_SHIFT)   SetTextColor(DC, RGB(255, 0, 255));
	
	if(g_fDraw)
	LineTo(g_HDC, x, y);
	
	SetBkColor(DC, GetSysColor(COLOR_WINDOW)); //������������� ���� ����
	TextOut(DC, nPosX, nPosY + 80, S, strlen(S));// ������� ������ ������

	//MoveToEx(DC, x, y, NULL);


	ReleaseDC(hwnd, DC);//����������� ��������
}



//=== ���������� ��������� WM_SYSKEYDOWN � WM_SYSKEYUP ===============//
void km_OnSysKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	char S[100];//����� ��� ������������ ��������� ������(100 ����)
	HDC DC = GetDC(hwnd); //�������� �������� ���������� ������������ ������
	SetBkColor(DC, GetSysColor(COLOR_WINDOW)); //������ ����

	if (fDown)
	{// ��������� ������� ������
		wsprintf(S, "WM_SYSKEYDOWN ==> vk = %d fDown = %d cRepeat = %d flags = %d     ",
			vk, fDown, cRepeat, flags);
		TextOut(DC, nPosX, nPosY + 60, S, strlen(S));
		FORWARD_WM_SYSKEYDOWN(hwnd, vk, cRepeat, flags, DefWindowProc);
	}
	else
	{// ��������� ������� ��������
		wsprintf(S, "WM_SYSKEYUP == > vk = %d fDown = %d cRepeat = %d flags = %d      ",
			vk, fDown, cRepeat, flags);
		TextOut(DC, nPosX, nPosY + 60, S, strlen(S));
		FORWARD_WM_SYSKEYUP(hwnd, vk, cRepeat, flags, DefWindowProc);
	}
	ReleaseDC(hwnd, DC);//����������� �������� ����������
}

//=== ���������� ��������� WM_PAINT ==================================//
void km_OnPaint(HWND hwnd)
{
	PAINTSTRUCT PaintStruct;
	RECT Rect;
	//������ 1 � 2 ������ ������
	static char* lpszTitle1 = "������������ ��������� ����� ���� � ����������";
	static char* lpszTitle2 = "������������������ � ����� � �����������";

	//������ ���������� �� ������ ������������� ������ 
	static char* SList[] =
	{
		"WM_CHAR     ",
		"WM_KEY      ",
		"WM_SYSKEY   ",
		"WM_MOUSEMOVE",
		"WM_MOUSEDOWN",
		"WM_MOUSEUP  "
	};
	char S[100], S1[100]; //������ ��� ������������ ������ ������������� ������
						  //��������� ������ S1 87 ��������� � ��������� �����
	memset(&S1[0], ' ', 87);
	S1[87] = '\0';

	HDC PaintDC = BeginPaint(hwnd, &PaintStruct);//�������� ��������
	SetBkColor(PaintDC, GetSysColor(COLOR_WINDOW));//������ ����
	GetClientRect(hwnd, &Rect);//�������� ���������� ��������� �������
	DrawText(PaintDC, lpszTitle1, -1, &Rect, DT_CENTER);//������� ������ 1

	Rect.top = 20; //������ ���������� � ����� ������ ������
	Rect.bottom = 40; //������ ���������� � ���� ������ ������
	DrawText(PaintDC, lpszTitle2, -1, &Rect, DT_CENTER); //������� ������ 2 

	for (int i = 0; i < 6; i++)
	{// ������� 6 ����� � ����������� �� SList � ����������� 87 ���������
		strcpy_s(S, SList[i]);//�������� � S ������ � ���������� �� SList[i]
		strcat_s(S, S1);// ���������� ������ S �� ������� S1    
		TextOut(PaintDC, nPosX, nPosY + (20 * (i + 1)), S, strlen(S));
	}
	EndPaint(hwnd, &PaintStruct);//����������� �������� ����������

}

//====================================================================//\