#include "sp_sr6.h"

//  Стартовая функция 
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

// Оконная процедура
LRESULT CALLBACK Pr2_WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hWnd, WM_CREATE, km_OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, km_OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, km_OnPaint);
		HANDLE_MSG(hWnd, WM_CLOSE, km_OnClose);					// Перед разрушением
		HANDLE_MSG(hWnd, WM_DESTROY, km_OnDestroy);				// Завершение работы приложения
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


/* Регистрация класса окна */
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
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	return TRUE;
}

/* Создание окна */
HWND Create(HINSTANCE hInstance, int nCmdShow)
{
	DWORD Stl;
	Stl = WS_OVERLAPPEDWINDOW ^ WS_MINIMIZEBOX;

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
		MessageBox(NULL, TEXT("Окно не создано!"),
			TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		return NULL;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

/* ---------- Обработчики сообщений -------------------------------- */


BOOL km_OnCreate(HWND hWnd, LPCREATESTRUCT lpszCreateStruct)
{
	g_hDcMeta = CreateMetaFile(NULL);

	HFONT hFont = NULL;
	// Узнаем, сколько пикселей в одном логическом дюйме
	int pixelsPerInch = GetDeviceCaps(g_hDcMeta, LOGPIXELSY);
	// Узнаем высоту в пикселях шрифта размером Size пунктов
	int fontHeight = -MulDiv(20, pixelsPerInch, 72);
	// Устанавливаем параметр жирности для функции CreateFont()

	int Weight = FW_NORMAL;
	// Удаляем предыдущий экземпляр шрифта -- нельзя дважды 
	// Создание шрифта. Большинство параметров не используются.
	hFont = CreateFont(fontHeight, 0, 0, 0, Weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, "TimesNewRoman");

	SelectObject(g_hDcMeta, hFont);
	SetTextColor(g_hDcMeta, RGB(0, 0, 255));
	TextOut(g_hDcMeta, 20, 20, TEXT("Володько Виталий Иванович"), 25);



	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	HBRUSH hBrush = CreateHatchBrush(HS_HORIZONTAL, RGB(255, 0, 0));
	SelectObject(g_hDcMeta, hPen);
	Ellipse(g_hDcMeta, 100, 100, 160, 160);

	RECT rRect;
	rRect.left = 105;
	rRect.right = 155;
	rRect.top = 160;
	rRect.bottom = 210;

	//FillRect(hDC, &rRect, hBrush);
	SelectObject(g_hDcMeta, hBrush);
	Rectangle(g_hDcMeta, rRect.left, rRect.top, rRect.right, rRect.bottom);
	DeleteObject(hPen);
	DeleteObject(hBrush);

	hmf = CloseMetaFile(g_hDcMeta);

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
	DeleteMetaFile(hmf);
	PostQuitMessage(0); // Посылка WM_QUIT приложению
}


void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна

	PlayMetaFile(hDC, hmf);

	EndPaint(hWnd, &ps); // Завершение обновления окна
}

// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
}



// WM_LBUTTONDOWN
void  FrameDlg_OnLButtonDown(HWND hDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
}

//=== Обработчик сообщения WM_LBUTTONUP ==============================//
void FrameDlg_OnLButtonUp(HWND hDlg, int x, int y, UINT keyFlags)
{
}
