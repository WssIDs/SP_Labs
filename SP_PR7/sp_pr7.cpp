#include "sp_pr7.h"

//  ��������� ������� 
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;
	HWND hWnd;

	if (!Register(hInstance)) return FALSE;

	hWnd = Create(hInstance, nCmdShow);

	if (hWnd == NULL) return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

// ������� ���������
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, km_OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, km_OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, km_OnPaint);
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// ����� �����������
		HANDLE_MSG(hWnd, WM_DESTROY, km_OnDestroy);				// ���������� ������ ����������
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


/* ����������� ������ ���� */
BOOL Register(HINSTANCE hInst)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = g_lpszClassName;
	wc.lpfnWndProc = Pr2_WndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("������ ����������� ������ ����!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

/* �������� ���� */
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW^WS_MINIMIZEBOX;

	HWND hWnd = CreateWindowEx(NULL, g_lpszClassName,
		g_lpszAplicationTitle,
		Stl,
		200,
		200,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		MessageBox(NULL, TEXT("���� �� �������!"),
			TEXT("������"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/* ---------- ����������� ��������� -------------------------------- */


BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct)
{
	return TRUE;
}

// WM_ONCLOSE
void km_OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
}

// WM_DESTROY
void km_OnDestroy(HWND hWnd)
{
	PostQuitMessage(0); // ������� WM_QUIT ����������
}


void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // ��������� ��������� ��� ���������� ����
	HFONT hFont = NULL;
									 // ������, ������� �������� � ����� ���������� �����
	int pixelsPerInch = GetDeviceCaps(hDC,LOGPIXELSY);
	// ������ ������ � �������� ������ �������� Size �������
	int fontHeight = -MulDiv(20, pixelsPerInch, 72);
	// ������������� �������� �������� ��� ������� CreateFont()

	int Weight = FW_NORMAL;
	// ������� ���������� ��������� ������ -- ������ ������ 
	// �������� ������. ����������� ���������� �� ������������.
	hFont = CreateFont(fontHeight, 0, 0, 0, Weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, "TimesNewRoman");

	SelectObject(hDC, hFont);
	SetTextColor(hDC, RGB(0, 0, 255));
	TextOut(hDC, 20, 20, TEXT("�������� ������� ��������"), 25);



	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HBRUSH hBrush = CreateHatchBrush(HS_HORIZONTAL, RGB(255, 0, 0));
	SelectObject(hDC, hPen);
	Ellipse(hDC, 100, 100, 160, 160);

	RECT rRect;
	rRect.left = 105;
	rRect.right = 155;
	rRect.top = 160;
	rRect.bottom = 210;

	//FillRect(hDC, &rRect, hBrush);
	SelectObject(hDC, hBrush);
	Rectangle(hDC,rRect.left,rRect.top,rRect.right,rRect.bottom);
	DeleteObject(hPen);
	DeleteObject(hBrush);

	EndPaint(hWnd, &ps); // ���������� ���������� ����
}

// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
}



// WM_LBUTTONDOWN
void  FrameDlg_OnLButtonDown(HWND hDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
}

//=== ���������� ��������� WM_LBUTTONUP ==============================//
void FrameDlg_OnLButtonUp(HWND hDlg, int x, int y, UINT keyFlags)
{
}
