#include "sp_pr6_2.h"

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
		HANDLE_MSG(hWnd, WM_KEYDOWN, km_OnKeyDown);
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
	wc.hbrBackground = CreateSolidBrush(RGB(255,255,255));

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
	g_hFrameWnd = CreateDialog((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_FRAMEDLG), hWnd, (DLGPROC)FrameDlgProc);
	MoveWindow(g_hFrameWnd, 10, 110, 150, 100, TRUE);

	RECT wi;

	GetWindowRect(g_hFrameWnd, &wi);

	ptClientUpL.x = wi.left;
	ptClientUpL.y = wi.top;
	ptClientLowR.x = wi.right;
	ptClientLowR.y = wi.bottom;
	ScreenToClient(hWnd, &ptClientUpL);
	ScreenToClient(hWnd, &ptClientLowR);


	g_hDC = GetDC(g_hFrameWnd);

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
	PostQuitMessage(0); // Посылка WM_QUIT приложению
}


void km_OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps); // Получение контекста для обновления окна

	char S[100];//Буфер для формирования выводимой строки(100 байт)

	RECT rc;
	GetClientRect(g_hFrameWnd, &rc);
	wsprintf(S, "Позиция рамки x = %d y = %d ", ptClientUpL.x, ptClientLowR.y);
	SetTextColor(hDC, RGB(255, 0, 0));		
	GetClientRect(hWnd, &rc);
	TextOut(hDC, 10, rc.bottom - 30, S, strlen(S));// Выводим строку текста

	EndPaint(hWnd, &ps); // Завершение обновления окна
}

// WM_COMMAND
void km_OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
}

// WM_KEYDOWN
void km_OnKeyDown(HWND hWnd, UINT nVirtKey, BOOL fDown, int cRepeat, UINT flags)
{
	static RECT wi;

	GetWindowRect(g_hFrameWnd, &wi);
	ptClientUpL.x = wi.left;
	ptClientUpL.y = wi.top;
	ptClientLowR.x = wi.right;
	ptClientLowR.y = wi.bottom;
	ScreenToClient(hWnd, &ptClientUpL);
	ScreenToClient(hWnd, &ptClientLowR);

	switch (nVirtKey)
	{
		case VK_LEFT:
		{
			MoveWindow(g_hFrameWnd, ptClientUpL.x - 5, ptClientUpL.y, ptClientLowR.x - ptClientUpL.x, ptClientLowR.y - ptClientUpL.y, TRUE);
		}
		break;
		case VK_RIGHT:
		{
			MoveWindow(g_hFrameWnd,ptClientUpL.x + 5,	ptClientUpL.y,	ptClientLowR.x - ptClientUpL.x,	ptClientLowR.y - ptClientUpL.y,	TRUE);
		}
		break;
		case VK_UP:
		{
			MoveWindow(g_hFrameWnd, ptClientUpL.x, ptClientUpL.y - 5, ptClientLowR.x - ptClientUpL.x, ptClientLowR.y - ptClientUpL.y, TRUE);
		}
		break;
		case VK_DOWN:
		{
			MoveWindow(g_hFrameWnd, ptClientUpL.x, ptClientUpL.y + 5, ptClientLowR.x - ptClientUpL.x, ptClientLowR.y - ptClientUpL.y, TRUE);
		}
		break;
		case VK_F2:
		{
			g_fDraw = TRUE;
		}
		break;
		case VK_F3:
		{
			g_fDraw = FALSE;
		}
		break;
	}

	HDC hDC = GetDC(hWnd);

	char S[100];//Буфер для формирования выводимой строки(100 байт)

	RECT rc;
	GetClientRect(g_hFrameWnd, &rc);
	wsprintf(S, "Позиция рамки x = %d y = %d ", ptClientUpL.x, ptClientLowR.y);
	SetTextColor(hDC, RGB(255, 0, 0));
	GetClientRect(hWnd, &rc);
	TextOut(hDC, 10, rc.bottom - 30, S, strlen(S));// Выводим строку текста

	ReleaseDC(hWnd, hDC);
}


// Процедура диалогового окна
BOOL CALLBACK FrameDlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		HANDLE_MSG(hDlg, WM_LBUTTONDOWN, FrameDlg_OnLButtonDown);		// Нажата левая клавиша мыши
		HANDLE_MSG(hDlg, WM_LBUTTONUP, FrameDlg_OnLButtonUp);			// Опущена левая клавига мыши
		HANDLE_MSG(hDlg, WM_MOUSEMOVE, FrameDlg_OnMouseMove);
	}

	return FALSE;
}

// WM_LBUTTONDOWN
void  FrameDlg_OnLButtonDown(HWND hDlg, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	// Захват ввода мыши 
	SetCapture(hDlg);

	// Получение аппаратных координат дисплея для клиентской области,
	// и преобразование их в клиентские координаты.. 
	GetClientRect(hDlg, &g_rcClient);
	g_ptClientUL.x = g_rcClient.left;
	g_ptClientUL.y = g_rcClient.top;

	// Добавляем по единице к правой и нижней сторонам,
	// потому что координаты, полученные через  GetClientRect 
	// не включают самый левый и самый нижний пикселы.

	g_ptClientLR.x = g_rcClient.right + 1;
	g_ptClientLR.y = g_rcClient.bottom + 1;
	ClientToScreen(hDlg, &g_ptClientUL);
	ClientToScreen(hDlg, &g_ptClientLR);

	// Копируем клиентные координаты клиентной области в структуру rcClient 
	// Передавая  структуру rcClient  функции ClipCursor ограничим 
	// перемешения курсора мыши клиентской областью.
	SetRect(&g_rcClient, g_ptClientUL.x, g_ptClientUL.y, g_ptClientLR.x, g_ptClientLR.y);
	ClipCursor(&g_rcClient);

	MoveToEx(g_hDC, x, y, NULL);
}

//=== Обработчик сообщения WM_LBUTTONUP ==============================//
void FrameDlg_OnLButtonUp(HWND hDlg, int x, int y, UINT keyFlags)
{
	ClipCursor(NULL);
	ReleaseCapture(); //Освобождает захват мыши окном в текущем потоке и восстанавливает обычную обработку ввода данных от мыши
}

// WM_MOUSEMOVE
void  FrameDlg_OnMouseMove(HWND hDlg, int x, int y, UINT keyFlags)
{

	if (g_fDraw && keyFlags == MK_LBUTTON)
	{
		if (g_count >= 5)
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			SelectObject(g_hDC, hPen);
			MoveToEx(g_hDC, x, y-10, NULL);
			LineTo(g_hDC, x, y + 10);
			DeleteObject(hPen);
			g_count = 0;
		}
		else
		{
			g_count += 1;
		}
	}
}


BOOL DrawLine(HDC hdc, int x0, int y0, int x, int y)
{

	return LineTo(hdc, x, y);
}
